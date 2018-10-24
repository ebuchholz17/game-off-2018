#include <stdio.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <stdlib.h>

#include "ebrt.h"

static int boxPosition = 0;

bool renderingFinished = false;
bool sceneInited = false;

int imageWidth = 1280;
int imageHeight = 720;

int currentRow = 0;
int currentCol = 0;
int maxDepth = 4;
int numSuperSamples = 4;
//int maxDepth = 1;
float epsilon = 0.0000001f; // TODO(ebuchholz): Pick a better number
float shadowBias = 0.0001f;  // TODO(ebuchholz): Pick a better number here also
//float shadowBias = 0.000035f;  
//float targetMSPerFrame = 16.6667f;

// TODO(ebuchholz): better data structures/scene
#define NUM_SPHERES 1
sphere spheres[NUM_SPHERES];

#define NUM_PLANES 1
plane planes[NUM_PLANES];

// Free-standingg triangles, not part of any mesh
#define NUM_TRIANGLES 2
triangle triangles[NUM_TRIANGLES];

#define NUM_MESHES 0
mesh meshes[1];

#define NUM_OBJECTS 5
object objects[NUM_OBJECTS];

#define NUM_MATERIALS 6
material materials[NUM_MATERIALS];

kdtree theOneKDTreeInTheScene;

// TODO(ebuchholz): bundle up all these parameters?
unsigned int addKDTreeNode (kdtree *tree, aabb *triangleAABBs, 
                            unsigned int *treeTriangles, unsigned int numTriangles, 
                            unsigned int *leftTriangles, unsigned int *rightTriangles,
                            bound_edge **boundEdges,
                            aabb nodeBounds, int depth) {
    unsigned int nodeIndex = tree->numNodes++;
    if (nodeIndex >= tree->nodeCapacity) {
        // reallocate nodes
        unsigned int newCapacity = tree->nodeCapacity * 2;
        kdtree_node *newNodes = (kdtree_node *)malloc(newCapacity * sizeof(kdtree_node));
        memcpy(newNodes, tree->nodes, tree->nodeCapacity * sizeof(kdtree_node));
        free(tree->nodes);
        tree->nodes = newNodes;
        tree->nodeCapacity = newCapacity;
    }
    kdtree_node newNode = {};

    bool isLeaf = false;
    // It's a leaf if we've gone too deep
    aabb leftBounds = {}; 
    aabb rightBounds = {};
    int numLeftTriangles = 0;
    int numRightTriangles = 0;
    if (depth > tree->maxDepth || numTriangles <= KDTREE_MIN_OBJECTS_PER_NODE) {
        isLeaf = true;
    }
    else {
#ifdef KDTREE_USE_SURFACE_AREA_HEURISTIC
        // Determine splitting plane, which objects go on which side, etc... If there is no
        // good plane I guess it's a leaf
        int bestAxis = -1;
        int bestOffset = -1;
        float bestCost = INFINITY;  
        float oldCost = KDTREE_INTERSECTION_COST * (float)numTriangles;
        float totalSurfaceArea = aabbSurfaceArea(nodeBounds);
        float inverseTotalSurfaceArea = 1.0f / totalSurfaceArea;
        vector3 d = nodeBounds.max - nodeBounds.min;
        int axis = aabbAxisWithMaximumExtent(nodeBounds);

        int retries = 0;
retrySplit:
        unsigned int numEdges = 2 * numTriangles;
        for (unsigned int i = 0; i < numTriangles; ++i) {
            int triangleIndex = treeTriangles[i];
            aabb triangleAABB = triangleAABBs[triangleIndex];
            boundEdges[axis][2*i] = BoundEdge(triangleAABB.min.c[axis], triangleIndex, true);
            boundEdges[axis][2*i + 1] = BoundEdge(triangleAABB.max.c[axis], triangleIndex, false);
        }
        // TODO(ebuchholz): Should make my own sort
        qsort(&boundEdges[axis][0], numEdges, sizeof(bound_edge), &compareBoundEdges);
        
        unsigned int numBelowTriangles = 0, numAboveTriangles = numTriangles;
        for (unsigned int i = 0; i < numEdges; ++i) {
            bound_edge edge = boundEdges[axis][i];
            if (edge.type == BOUND_EDGE_END) { --numAboveTriangles; }
            float edgeT = edge.t;
            if (edgeT > nodeBounds.min.c[axis] && edgeT < nodeBounds.max.c[axis]) {
                int otherAxis = (axis + 1) % 3, otherOtherAxis = (axis + 2) % 3;
                float belowSurfaceArea = 2.0f * (d.c[otherAxis] * d.c[otherOtherAxis] +
                                                 (edgeT - nodeBounds.min.c[axis]) *
                                                 (d.c[otherAxis] + d.c[otherOtherAxis]));
                float aboveSurfaceArea = 2.0f * (d.c[otherAxis] * d.c[otherOtherAxis] +
                                                 (nodeBounds.max.c[axis] - edgeT) *
                                                 (d.c[otherAxis] + d.c[otherOtherAxis]));
                float percentBelow = belowSurfaceArea * inverseTotalSurfaceArea;
                float percentAbove = aboveSurfaceArea * inverseTotalSurfaceArea;
                float emptyBonus = (numAboveTriangles == 0 || numBelowTriangles == 0) ? 
                                   KDTREE_EMPTY_BONUS : 0.0f;         
                float cost = KDTREE_TRAVERSAL_COST + 
                             KDTREE_INTERSECTION_COST * (1.0f - emptyBonus) * 
                                                        (percentBelow * numBelowTriangles + 
                                                         percentAbove * numAboveTriangles);
                if (cost < bestCost) {
                    bestCost = cost;
                    bestAxis = axis;
                    bestOffset = i;
                }
            }
            if (edge.type == BOUND_EDGE_START) { ++numBelowTriangles; }
        }

        if (bestAxis == -1 && retries < 2) {
            ++retries;
            axis = (axis + 1) % 3;
            goto retrySplit;
        }
        // TODO(ebuchholz): implement "bad refines" from pbrt
        if ((bestCost > 4.0f * oldCost && numTriangles < 16) || bestAxis == -1) {
            isLeaf = true;
        }
        else {
            // TODO(ebuchholz): fix it so these i's are the same type
            for (int i = 0; i < bestOffset; ++i) {
                bound_edge edge = boundEdges[bestAxis][i];
                if (edge.type == BOUND_EDGE_START) {
                    leftTriangles[numLeftTriangles++] = edge.objectIndex;
                }
            } 
            for (unsigned int i = bestOffset + 1; i < numEdges; ++i) {
                bound_edge edge = boundEdges[bestAxis][i];
                if (edge.type == BOUND_EDGE_END) {
                    rightTriangles[numRightTriangles++] = edge.objectIndex;
                }
            } 
            
            newNode.splitDirection = (kdtree_split_direction)bestAxis;
            leftBounds = rightBounds = nodeBounds;
            float split = boundEdges[bestAxis][bestOffset].t;
            leftBounds.max.c[newNode.splitDirection] = split;
            rightBounds.min.c[newNode.splitDirection] = split;
            newNode.splitPos = split;
        }

#else
        // Temporary way: split down the middle, alternate the direction each call
        // TODO(ebuchholz): bad, remove
        // TODO(ebuchholz): make the split amount not half
        static kdtree_split_direction splitDirection = KDTREE_SPLIT_X;
        splitDirection = (kdtree_split_direction)((splitDirection + 1) % 3);
        newNode.splitDirection = splitDirection;
        newNode.splitPos = nodeBounds.min.c[splitDirection] + 
                           0.5f * 
                           (nodeBounds.max.c[splitDirection] - nodeBounds.min.c[splitDirection]);
        leftBounds = rightBounds = nodeBounds;
        leftBounds.max.c[splitDirection] = newNode.splitPos;
        rightBounds.min.c[splitDirection] = newNode.splitPos;

        // divy up triangles into the bounds they intersect
        for (unsigned int i = 0; i < numTriangles; ++i) {
            if (aabbIntersection(triangleAABBs[treeTriangles[i]], leftBounds)) {
                leftTriangles[numLeftTriangles++] = treeTriangles[i];
            }
            if (aabbIntersection(triangleAABBs[treeTriangles[i]], rightBounds)) {
                rightTriangles[numRightTriangles++] = treeTriangles[i];
            }
        }
#endif
    }

    if (isLeaf) {
        newNode.splitDirection = KDTREE_SPLIT_LEAF;
        // allocate objects
        unsigned int newNumObjects = tree->numObjects + numTriangles;
        while (newNumObjects > tree->objectCapacity) {
            // reallocate object indices
            unsigned int newObjectCapacity = tree->objectCapacity * 2;
            unsigned int *newObjectIndices = 
                (unsigned int *)malloc(newObjectCapacity * sizeof(unsigned int));
            memcpy(newObjectIndices, tree->objectIndices, 
                   tree->objectCapacity * sizeof(unsigned int));
            free(tree->objectIndices); 
            tree->objectIndices = newObjectIndices;
            tree->objectCapacity = newObjectCapacity;
        }
        newNode.numObjects = numTriangles;
        newNode.objectArrayIndex = tree->numObjects;
        for (unsigned int i = 0; i < numTriangles; ++i) {
            tree->objectIndices[tree->numObjects++] = treeTriangles[i];
        }
    }
    else {
        if (numLeftTriangles > 0) {
            newNode.firstChildIndex = addKDTreeNode(tree, triangleAABBs, 
                                                    leftTriangles, numLeftTriangles, 
                                                    leftTriangles, 
                                                    rightTriangles + numRightTriangles,  
                                                    boundEdges,
                                                    leftBounds, depth + 1);
        }
        else {
            newNode.firstChildIndex = KDTREE_NODE_EMPTY;
        }
        if (numRightTriangles > 0) {
            newNode.secondChildIndex = addKDTreeNode(tree, triangleAABBs, 
                                                     rightTriangles, numRightTriangles, 
                                                     leftTriangles, 
                                                     rightTriangles + numRightTriangles,  
                                                     boundEdges,
                                                     rightBounds, depth + 1);
        }
        else {
            newNode.secondChildIndex = KDTREE_NODE_EMPTY;
        }
    }
    tree->nodes[nodeIndex] = newNode;
    return nodeIndex;
}

// TODO(ebuchholz): make this operate on all objects, instead of one mesh
kdtree buildKDTree (object targetObject) {
    if (targetObject.shapeType != MESH) {
        // TODO(ebuchholz): some kind of assert statement
        printf("Tried to make a kd tree from something that wasn't a triangle mesh\n");
        // error
    }
    kdtree result;
    mesh targetMesh = meshes[targetObject.shapeIndex];
    int numTriangles = targetMesh.numTriangles;
    // This tree depth taken from pbrt
    int treeDepth = (int)(8 + 1.3f * (logf((float)numTriangles) / logf(2.0f)));
    treeDepth = treeDepth < KDTREE_MAX_DEPTH ? treeDepth : KDTREE_MAX_DEPTH;
    printf("Tree depth: %d", treeDepth);
    result.maxDepth = treeDepth; 
    //result.maxDepth = 14;

    result.materialIndex = targetObject.materialIndex;

    // allocate some space for object indices and kdtree nodes, which will be resized when 
    // it fills up
    result.numNodes = 0;
    result.nodeCapacity = 512;
    result.nodes = (kdtree_node *)malloc(result.nodeCapacity * sizeof(kdtree_node));

    result.numObjects = 0;
    result.objectCapacity = 256;
    result.objectIndices = (unsigned int *)malloc(result.objectCapacity * sizeof(unsigned int));

    // break down mesh into individual triangles, applying the object's transform
    matrix4x4 transform = targetObject.transform;
    matrix4x4 inverseTranspose = transpose(targetObject.inverseTransform);
    result.refinedTriangles = (mesh_triangle *)malloc(numTriangles * sizeof(mesh_triangle));
    for (int i = 0; i < numTriangles; ++i) {
        mesh_triangle transformedTriangle;
        transformedTriangle.p0 = transform * targetMesh.positions[targetMesh.indices[(i*3)]];
        transformedTriangle.p1 = transform * targetMesh.positions[targetMesh.indices[(i*3)+1]];
        transformedTriangle.p2 = transform * targetMesh.positions[targetMesh.indices[(i*3)+2]];
        transformedTriangle.n0 = normalize(inverseTranspose * targetMesh.normals[targetMesh.indices[(i*3)]]);
        transformedTriangle.n1 = normalize(inverseTranspose * targetMesh.normals[targetMesh.indices[(i*3)+1]]);
        transformedTriangle.n2 = normalize(inverseTranspose * targetMesh.normals[targetMesh.indices[(i*3)+2]]);
        result.refinedTriangles[i] = transformedTriangle;
    }

    // calculate AABB's for all of the triangles ahead of time
    aabb *triangleAABBs = (aabb *)malloc(numTriangles * sizeof(aabb));
    aabb kdtreeBounds = {};
    for (int i = 0; i < numTriangles; ++i) {
        triangleAABBs[i] = getTriangleBounds(result.refinedTriangles[i].p0, 
                                             result.refinedTriangles[i].p1,
                                             result.refinedTriangles[i].p2);
        kdtreeBounds = unionAABB(kdtreeBounds, triangleAABBs[i]);
    }
    result.bounds = kdtreeBounds;

    // allocate working space for splitting up which objects belong to which nodes
    // TODO(ebuchholz): make sure this is the right amount of space
    unsigned int *leftTriangles = (unsigned int *)malloc(numTriangles * sizeof(unsigned int));
    unsigned int *rightTriangles =
        (unsigned int *)malloc((result.maxDepth + 1) *  numTriangles * sizeof(unsigned int));

    // working space for bound edges, enough to put all objects along any axis
    // need two bound edges per triangle (one for the start and one for the end)
    bound_edge *boundEdges[3];
    for (int i = 0; i < 3; ++i) {
        boundEdges[i] = (bound_edge *)malloc(numTriangles * 2 * sizeof(bound_edge));
    }

    for (int i = 0; i < numTriangles; ++i) {
        leftTriangles[i] = i;
        rightTriangles[i] = i;
    }

    addKDTreeNode(&result, triangleAABBs, leftTriangles, numTriangles, 
                  leftTriangles, rightTriangles, boundEdges, kdtreeBounds, 0);

    for (int i = 0; i < 3; ++i) {
        free(boundEdges[i]);
    }
    free(rightTriangles);
    free(leftTriangles);
    free(triangleAABBs);
    return result;
}

void initScene () {
    spheres[0] = Sphere(Vector3(0.0f, 0.0f, 0.0f), 0.5f);

    planes[0] = Plane(Vector3(0.0f, 1.0f, 0.0f), 0.0f);

    triangles[0] = Triangle(Vector3(-3.0f, 0.0f, 4.0f),
                            Vector3(-3.0f, 2.0f, 4.0f),
                            Vector3(3.0f, 2.0f, 4.0f));

    triangles[1] = Triangle(Vector3(3.0f, 2.0f, 4.0f),
                            Vector3(3.0f, 0.0f, 4.0f),
                            Vector3(-3.0f, 0.0f, 4.0f));

    materials[0].color = Vector3(0.2f, 0.05f, 0.05f);
    materials[0].roughness = 1.0f;

    materials[1].color = Vector3(0.05f, 0.05f, 0.2f);
    materials[1].roughness = 0.75f;

    materials[2].color = Vector3(0.51f, 0.51f, 0.51f);
    materials[2].roughness = 0.001f;
    
    materials[3].color = Vector3(0.03f, 0.17f, 0.03f);
    materials[3].roughness = 0.9f;

    materials[4].color = Vector3(0.19f, 0.13f, 0.05f);
    materials[4].roughness = 0.1f;

    materials[5].color = Vector3(0.5f, 0.5f, 0.5f);
    materials[5].roughness = 1.0f;

    matrix4x4 identityMatrix = identityMatrix4x4();

    objects[0].shapeType = SPHERE;
    objects[0].shapeIndex = 0;
    objects[0].materialIndex = 0;
    objects[0].transform = translationMatrix(-2.0f, 0.5f, 0.0f);
    objects[0].inverseTransform = inverse(objects[0].transform);

    matrix4x4 fatSphereTransform = 
        translationMatrix(2.0f, 0.5f, 0.0f) * 
        rotationMatrixFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), PI * 0.25f) *
        rotationMatrixFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), PI * 0.25f) *
        scaleMatrix(2.0f, 0.5f, 2.0f);

    objects[1].shapeType = SPHERE;
    objects[1].shapeIndex = 0;
    objects[1].materialIndex = 1;
    objects[1].transform = fatSphereTransform;
    objects[1].inverseTransform = inverse(objects[1].transform);

    objects[2].shapeType = PLANE;
    objects[2].shapeIndex = 0;
    objects[2].materialIndex = 3;
    objects[2].transform = identityMatrix;
    objects[2].inverseTransform = identityMatrix;

    objects[3].shapeType = TRIANGLE;
    objects[3].shapeIndex = 0;
    objects[3].materialIndex = 2;
    objects[3].transform = identityMatrix;
    objects[3].inverseTransform = identityMatrix;

    objects[4].shapeType = TRIANGLE;
    objects[4].shapeIndex = 1;
    objects[4].materialIndex = 2;
    objects[4].transform = identityMatrix;
    objects[4].inverseTransform = identityMatrix;
}

// TODO(ebuchholz): Look up a better version of this, maybe in RTCD
inline bool sphereIntersection (vector3 rayPos, vector3 rayDir, sphere testSphere, 
                                float *t, vector3 *normal, vector3 *intersectionPoint) {
    vector3 centerToRayPos = rayPos - testSphere.pos;
    float b = dotProduct(centerToRayPos, rayDir); // b part of quadratic equation
    // c part of quadratic equation
    float c = dotProduct(centerToRayPos, centerToRayPos) - square(testSphere.radius); 
    if (c > 0.0f && b > 0.0f) {
        // center outside sphere and pointing away
        return false;
    }
    else {
        float discriminant = square(b) - c;
        if (discriminant < 0.0f) {
            // no roots, no intersection points
            return false;
        }
        else {
            float squareRtDiscriminant = sqrtf(discriminant);
            *t = -b - squareRtDiscriminant;
            if (*t < 0.0f) {
                // TODO(ebuchholz): check that this is the right way to get the other 
                // intersection
                *t = -b + squareRtDiscriminant;
            }
            *intersectionPoint = rayPos + *t * rayDir;
            *normal = normalize(*intersectionPoint - testSphere.pos);
            return true;
        }
    }
}

// TODO(ebuchholz): plane intersection
inline bool planeIntersection (vector3 rayPos, vector3 rayDir, plane testPlane,
                               float *t, vector3 *normal, vector3 *intersectionPoint) {
    // from rtcd
    float newT = (testPlane.distance - dotProduct(testPlane.normal, rayPos)) /
                 dotProduct(testPlane.normal, rayDir);
    if (newT >= 0.0f) {
        *t = newT;
        *normal = testPlane.normal;
        *intersectionPoint = rayPos + *t * rayDir;
        return true;
    }
    else {
        return false;
    }
}

inline bool triangleIntersection (vector3 rayPos, vector3 rayDir, 
                                  vector3 p0, vector3 p1, vector3 p2,
                                  float *t, vector3 *normal, vector3 *intersectionPoint) {
    vector3 ab = p1 - p0;
    vector3 ac = p2 - p0;
    vector3 qp = -rayDir; 

    vector3 n = crossProduct(ab, ac); // triangle normal
    float d = dotProduct(qp, n);
    //if (d <= 0.0f) { // backface culling
    //    return false;
    //}
    float ood = 1.0f / d;
    vector3 ap = rayPos - p0;
    float newT = dotProduct(ap, n);
    newT *= ood;
    if (newT < 0.0f) {
        return false;
    }

    vector3 e = crossProduct(qp, ap);
    float v = dotProduct(ac, e);
    v *= ood;
    if( v < 0.0f || v > 1.0f) {
        return false;
    }
    float w = -dotProduct(ab,e);
    w *= ood;
    if (w < 0.0f || v + w > 1.0f) {
        return false;
    } 

    float u = 1.0f - v - w;

    *t = newT;
    *normal = normalize(crossProduct(ab, ac));
    if (d < 0.0f) {
        *normal = -*normal;
    }
    *intersectionPoint = u * p0 + v * p1 + w * p2;
    return true;
}

// from RTCD, modified so that we can hit the backfaces of triangles
inline bool triangleMeshIntersection (vector3 rayPos, vector3 rayDir, 
                                      vector3 p0, vector3 p1, vector3 p2,
                                      vector3 n0, vector3 n1, vector3 n2,
                                      float *t, vector3 *normal, vector3 *intersectionPoint) {
    vector3 ab = p1 - p0;
    vector3 ac = p2 - p0;
    vector3 qp = -rayDir; // TODO(ebuchholz): vector3 negation operator

    vector3 n = crossProduct(ab, ac); // triangle normal
    float d = dotProduct(qp, n);
    //if (d <= 0.0f) { // backface culling
    //    return false;
    //}
    float ood = 1.0f / d;
    vector3 ap = rayPos - p0;
    float newT = dotProduct(ap, n);
    newT *= ood;
    if (newT < 0.0f) {
        return false;
    }

    vector3 e = crossProduct(qp, ap);
    float v = dotProduct(ac, e);
    v *= ood;
    if( v < 0.0f || v > 1.0f) {
        return false;
    }
    float w = -dotProduct(ab,e);
    w *= ood;
    if (w < 0.0f || v + w > 1.0f) {
        return false;
    } 

    float u = 1.0f - v - w;

    *t = newT;
    *normal = normalize(u * n0 + v * n1 + w * n2);
    if (d < 0.0f) {
        *normal = -*normal;
    }
    *intersectionPoint = u * p0 + v * p1 + w * p2;
    return true;
}

inline bool meshIntersection (vector3 rayPos, vector3 rayDir, mesh testMesh,
                              float *t, vector3 *normal, vector3 *intersectionPoint) {
    float newT = INFINITY;
    vector3 newNormal = {};
    vector3 newIntersection = {};
    bool intersection = false;
    for (unsigned int i = 0; i < testMesh.numTriangles; ++i) {
        if (triangleMeshIntersection(rayPos, rayDir, 
                                 testMesh.positions[testMesh.indices[(i*3)]],
                                 testMesh.positions[testMesh.indices[(i*3)+1]],
                                 testMesh.positions[testMesh.indices[(i*3)+2]],
                                 testMesh.normals[testMesh.indices[(i*3)]],
                                 testMesh.normals[testMesh.indices[(i*3)+1]],
                                 testMesh.normals[testMesh.indices[(i*3)+2]],
                                 &newT, &newNormal, &newIntersection)) {
            intersection = true;
            if (newT < *t) {
                *t = newT;
                *normal = newNormal;
                *intersectionPoint = newIntersection;
            }
        }
    }
    return intersection;
}

inline float maxFloat(float a, float b) {
    if (a > b) {
        return a;
    }
    return b;
}

inline float minFloat(float a, float b) {
    if (a < b) {
        return a;
    }
    return b;
}

inline bool rayAABBIntersection (vector3 rayPos, vector3 rayDir, aabb b, 
                                 float *tMin, float *tMax) 
{
    for (int i = 0; i < 3; ++i) {
        if (fabs(rayDir.c[i]) < epsilon) {
            if (rayPos.c[i] < b.min.c[i] || rayPos.c[i] > b.max.c[i]) return false;
        }
        else {
            float ood = 1.0f / rayDir.c[i];
            float t1 = (b.min.c[i] - rayPos.c[i]) * ood;
            float t2 = (b.max.c[i] - rayPos.c[i]) * ood;
            if (t1 > t2) {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }
            *tMin = maxFloat(*tMin, t1);
            *tMax = minFloat(*tMax, t2);
            if (*tMin > *tMax) return false;
        }
    }
    // q = p + d * tmin
    return true;
}

// doesn't tell where the ray hit
inline bool rayAABBIntersection (vector3 rayPos, vector3 rayDir, aabb b) {
    float tMin = 0.0f;
    float tMax = INFINITY;

    bool intersected = rayAABBIntersection(rayPos, rayDir, b, &tMin, &tMax);

    return intersected;
}

inline bool kdTreeNodeIntersection (vector3 rayPos, vector3 rayDir, kdtree *tree, 
                                    unsigned int nodeIndex, aabb nodeBounds, float *t, 
                                    vector3 *normal, vector3 *intersectionPoint) {
    kdtree_node currentNode = tree->nodes[nodeIndex];
    kdtree_split_direction splitDirection = currentNode.splitDirection;
    bool intersected = false;
    if (splitDirection == KDTREE_SPLIT_LEAF) {
        unsigned int numObjects = currentNode.numObjects;
        for (unsigned int i = 0; i < numObjects; ++i) {
            float newT = INFINITY;
            vector3 newNormal = {};
            vector3 newIntersection = {};
            mesh_triangle nodeTriangle = 
                tree->refinedTriangles[tree->objectIndices[currentNode.objectArrayIndex + i]];
            if(triangleMeshIntersection(rayPos, rayDir, nodeTriangle.p0, nodeTriangle.p1, nodeTriangle.p2, 
                                    nodeTriangle.n0, nodeTriangle.n1, nodeTriangle.n2, 
                                    &newT, &newNormal, &newIntersection)) {
                intersected = true;
                if (newT < *t) {
                    *t = newT;
                    *normal = newNormal;
                    *intersectionPoint = newIntersection;
                }
            }
        }
    }
    else {
        if (currentNode.firstChildIndex != KDTREE_NODE_EMPTY) {
            aabb leftBounds = nodeBounds;
            leftBounds.max.c[splitDirection] = currentNode.splitPos;
            if (rayAABBIntersection(rayPos, rayDir, leftBounds)) {
                float newT = INFINITY;
                vector3 newNormal = {};
                vector3 newIntersection = {};
                if (kdTreeNodeIntersection(rayPos, rayDir, tree, 
                                           currentNode.firstChildIndex,
                                           leftBounds, &newT, &newNormal, &newIntersection)) {
                    intersected = true;
                    if (newT < *t) {
                        *t = newT;
                        *normal = newNormal;
                        *intersectionPoint = newIntersection;
                    }
                }
            }
        }
        if (currentNode.secondChildIndex != KDTREE_NODE_EMPTY) {
            aabb rightBounds = nodeBounds;
            rightBounds.min.c[splitDirection] = currentNode.splitPos;
            if (rayAABBIntersection(rayPos, rayDir, rightBounds)) {
                float newT = INFINITY;
                vector3 newNormal = {};
                vector3 newIntersection = {};
                if (kdTreeNodeIntersection(rayPos, rayDir, tree, 
                                           currentNode.secondChildIndex,
                                           rightBounds, &newT, &newNormal, &newIntersection)) {
                    intersected = true;
                    if (newT < *t) {
                        *t = newT;
                        *normal = newNormal;
                        *intersectionPoint = newIntersection;
                    }
                }
            }
        }
    }

    return intersected;
}

// TODO(ebuchholz): pack together t/normal/intersection data - maybe add reset function for it
// TODO(ebuchholz): better kd tree testing with early out, etc.
inline bool kdTreeIntersection (vector3 rayPos, vector3 rayDir, kdtree *tree, float *t,
                                vector3 *normal, vector3 *intersectionPoint) {
    // t's for the interval of the tree that we are checking
    // the t passed in is the intersection point of the ray
    float tMin = 0.0f;
    float tMax = INFINITY;
    if (rayAABBIntersection(rayPos, rayDir, tree->bounds, &tMin, &tMax)) {
        //return kdTreeNodeIntersection(rayPos, rayDir, tree, 0, tree->bounds, t, normal, 
        //                              intersectionPoint);
        // initialize work
        vector3 inverseRayDir = Vector3(1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z);
        kdtree_work_record kdTreeWork[KDTREE_MAX_DEPTH];
        int workIndex = 0;

        bool hit = false;
        unsigned int nodeIndex = 0;
        while (true) {
            if (nodeIndex == KDTREE_NODE_EMPTY) {
                if (workIndex > 0) {
                    --workIndex;
                    nodeIndex = kdTreeWork[workIndex].nodeIndex;
                    tMin = kdTreeWork[workIndex].tMin;
                    tMax = kdTreeWork[workIndex].tMax;
                }
                else {
                    break;
                }
            }
            // if we found a hit closer than the current node, exit
            if (*t < tMin) { break; }

            kdtree_node node = tree->nodes[nodeIndex];
            
            // If it is not a leaf, process inner nodes
            if (node.splitDirection != KDTREE_SPLIT_LEAF) {
                int axis = (int)node.splitDirection;
                float tPlane = (node.splitPos - rayPos.c[axis]) * inverseRayDir.c[axis];

                // get the next nodes if the ray passes through them
                unsigned int firstChildIndex, secondChildIndex;
                bool belowFirst = (rayPos.c[axis] < node.splitPos) ||
                                  (rayPos.c[axis] == node.splitPos && rayDir.c[axis] <= 0);
                if (belowFirst) {
                    firstChildIndex = node.firstChildIndex;
                    secondChildIndex = node.secondChildIndex;
                }
                else {
                    firstChildIndex = node.secondChildIndex;
                    secondChildIndex = node.firstChildIndex;
                }

                // queue up the new nodes if the ray passes through them
                // These two cases let us skip one of the nodes
                if (tPlane > tMax || tPlane < 0) {
                    nodeIndex = firstChildIndex;
                }
                else if (tPlane < tMin) {
                    nodeIndex = secondChildIndex;
                }
                // In this case we have to process both
                else {
                    if (secondChildIndex != KDTREE_NODE_EMPTY) {
                        kdTreeWork[workIndex].nodeIndex = secondChildIndex;
                        kdTreeWork[workIndex].tMin = tPlane;
                        kdTreeWork[workIndex].tMax = tMax;
                        ++workIndex;
                    }
                    nodeIndex = firstChildIndex;
                    tMax = tPlane;
                }

            }
            // If it is, test for intersections inside node
            else {
                unsigned int numObjects = node.numObjects;
                for (unsigned int i = 0; i < numObjects; ++i) {
                    float newT = INFINITY;
                    vector3 newNormal = {};
                    vector3 newIntersection = {};
                    mesh_triangle nodeTriangle = 
                        tree->refinedTriangles[tree->objectIndices[node.objectArrayIndex + i]];
                    if(triangleMeshIntersection(rayPos, rayDir, nodeTriangle.p0, nodeTriangle.p1, nodeTriangle.p2, 
                                            nodeTriangle.n0, nodeTriangle.n1, nodeTriangle.n2, 
                                            &newT, &newNormal, &newIntersection)) {
                        hit = true;
                        if (newT < *t) {
                            *t = newT;
                            *normal = newNormal;
                            *intersectionPoint = newIntersection;
                        }
                    }
                }
                if (workIndex > 0) {
                    --workIndex;
                    nodeIndex = kdTreeWork[workIndex].nodeIndex;
                    tMin = kdTreeWork[workIndex].tMin;
                    tMax = kdTreeWork[workIndex].tMax;
                }
                else {
                    break;
                }
            }
        }


        return hit;
    }
    else {
        return false;
    }
}

bool castRay (vector3 rayPos, vector3 rayDir, float *t, vector3 *normal, 
                 vector3 *intersectionPoint, material *objectMaterial) {
    bool intersection = false;
    float newT = INFINITY;
    vector3 newNormal = {};
    vector3 newIntersectionPoint = {};
    for (int i = 0; i < NUM_OBJECTS; ++i) {
        object object = objects[i];
        newT = INFINITY;
        newNormal = Vector3();
        newIntersectionPoint = Vector3();
        // transform the ray into the space of the object
        vector3 localRayPos = object.inverseTransform * rayPos;
        vector3 localRayDir = normalize(transformDirection(object.inverseTransform, rayDir));
        bool newIntersection = false;
        switch(object.shapeType) {
            case SPHERE: {
                if (sphereIntersection(localRayPos, localRayDir, spheres[object.shapeIndex], 
                                       &newT, &newNormal, &newIntersectionPoint)) {
                    newIntersection = true;
                }
            } break;
            case PLANE: {
                if (planeIntersection(localRayPos, localRayDir, planes[object.shapeIndex], 
                                      &newT, &newNormal, &newIntersectionPoint)) {
                    newIntersection = true;
                }
            } break;
            case TRIANGLE: {
                triangle testTriangle = triangles[object.shapeIndex];
                if (triangleIntersection(localRayPos, localRayDir, 
                                         testTriangle.p0, testTriangle.p1, testTriangle.p2,
                                         &newT, &newNormal, &newIntersectionPoint)) {
                    newIntersection = true;
                }
            } break;
            case MESH: {
                // QQQ skipping meshes
                //mesh testMesh = meshes[object.shapeIndex];
                //if (meshIntersection(localRayPos, localRayDir, testMesh, 
                //                     &newT, &newNormal, &newIntersectionPoint)) {
                //    newIntersection = true;
                //}
            } break;
        }
        if (newIntersection) {
            intersection = true;
            newIntersectionPoint = object.transform * newIntersectionPoint;
            newT = length(newIntersectionPoint - rayPos);
            if (newT < *t) {
                *t = newT;
                matrix4x4 inverseTranspose = transpose(object.inverseTransform);
                // TODO(ebuchholz): do we need to normalize this guy
                *normal = normalize(inverseTranspose * newNormal);
                *objectMaterial = materials[object.materialIndex];
                *intersectionPoint = newIntersectionPoint;
            }
        }
    }

    // test kdtree
    //newT = INFINITY;
    //newNormal = {};
    //newIntersectionPoint = {};
    //if (kdTreeIntersection(rayPos, rayDir, &theOneKDTreeInTheScene, &newT, &newNormal, 
    //                       &newIntersectionPoint)) {
    //    intersection = true;
    //    if (newT < *t) {
    //        *t = newT;
    //        *normal = newNormal;
    //        *objectMaterial = materials[theOneKDTreeInTheScene.materialIndex];
    //        *intersectionPoint = newIntersectionPoint;
    //    }
    //}

    return intersection;
}

vector3 calcColor (vector3 rayPos, vector3 rayDir, int depth) {
    float t = INFINITY;
    vector3 normal = {};
    vector3 intersectionPoint = {};
    material objectMaterial = {};

    vector3 testLightDir = normalize(Vector3(-1.0f, 1.0f, -1.0f));
    vector3 lightColor = Vector3(2.0f, 1.6f, 0.9f);

    vector3 testReverseLight = normalize(Vector3(0.5f, 1.0f, 0.5f));
    vector3 reverseLightColor = Vector3(0.2f, 0.16f, 0.09f);

    vector3 skyColor = Vector3(0.05f, 0.05f, 0.2f);

    bool intersected = castRay(rayPos, rayDir, &t, &normal, &intersectionPoint, &objectMaterial);

    // TODO(ebuchholz): Make lighting way better
    if (intersected)  {
        // push this guy out a little bit
        vector3 newOrigin = intersectionPoint + normal * shadowBias;

        material shadowMaterial;

        vector3 dummyNormal = {};
        bool shadowRay = castRay(newOrigin, testLightDir, &t, &dummyNormal, &intersectionPoint, &shadowMaterial);
        if (shadowRay) {
            return skyColor * objectMaterial.color;
        }
        else {
            float lightness = dotProduct(normal, testLightDir);
            if (lightness < 0.0f) {
                lightness = 0.0f;
            }
            vector3 mainLightContribution = lightness * lightColor;
            vector3 ambientLightContribution = skyColor;
            float reverseLightness = dotProduct(normal, testReverseLight);
            if (reverseLightness < 0.0f) {
                reverseLightness = 0.0f;
            }
            vector3 reverseLightContribution = reverseLightness * reverseLightColor;

            vector3 totalLight = mainLightContribution + reverseLightContribution;

            vector3 totalColor = objectMaterial.color 
                                 * (objectMaterial.roughness * totalLight + ambientLightContribution);

            if (depth < maxDepth) {
                vector3 reflectedRayDir = normalize(rayDir - 2 * dotProduct(normal, rayDir) * normal);

                vector3 reflectedColor = calcColor(newOrigin, reflectedRayDir, depth + 1);

                totalColor += objectMaterial.color * reflectedColor 
                              * (1.0f - objectMaterial.roughness);

                return totalColor;
            }
            else {
                return totalColor;
            }
        }
    }
    else {
        // TODO(ebuchholz): better sky
        float lightness = dotProduct(rayDir, testLightDir);
        if (lightness < 0.0f) {
            lightness = 0.0f;
        }
        vector3 floatColor = lightColor * lightness;

        float reverseLightness = dotProduct(rayDir, testReverseLight);
        if (reverseLightness < 0.0f) {
            reverseLightness = 0.0f;
        }
        floatColor += reverseLightColor * reverseLightness;

        floatColor += skyColor;

        return floatColor;
    }

}

// TODO(ebuchholz): bresenhamify
void drawLine (unsigned int *pixels, int startX, int startY, int endX, int endY) {
    // TODO(ebuchholz): clip lines
    if (startX < 0) { startX = 0; }
    if (startX >= imageWidth) { startX = imageWidth - 1; }
    if (startY < 0) { startY = 0; }
    if (startY >= imageHeight) { startY = imageHeight - 1; }

    if (endX < 0) { endX = 0; }
    if (endX >= imageWidth) { endX = imageWidth - 1; }
    if (endY < 0) { endY = 0; }
    if (endY >= imageHeight) { endY = imageHeight - 1; }

    if (startX > endX) {
        int temp = startX;
        startX = endX;
        endX = temp;

        temp = startY;
        startY = endY;
        endY = temp;
    }

    float dx = (float)(endX - startX);
    float dy = (float)(endY - startY);

    vector3 color = {};
    color.r = 1.0;
    color.g = 1.0;
    color.b = 1.0;
    unsigned int finalColor = ((int)(255.0f * 1.0f)) << 24 |
                              ((int)(255.0f * color.r)) << 16 |
                              ((int)(255.0f * color.g)) << 8 |
                              ((int)(255.0f * color.b));

    if (dx == 0) {
        if(startY > endY) {
            int temp = startY;
            startY = endY;
            endY = temp;
        }
        for (int y = startY; y <= endY; ++y) {
            pixels[y * imageWidth + startX] = finalColor; 
        }
    }
    else if (dy == 0) {
        for (int x = startX; x <= endX; ++x) {
            pixels[startY * imageWidth + x] = finalColor; 
        }
    }
    else {
        float dxdy = dx / dy;
        float dydx = dy / dx;
        float pixelX, pixelY;
        if (dy > 0) {
            if (dydx > 1.0f) {
                pixelX = (float)startX;
                for (int y = startY; y <= endY; ++y) {
                    pixelY = (float)y;
                    pixelX = pixelX + dxdy;

                    pixels[(int)pixelY * imageWidth + (int)pixelX] = finalColor; 
                }
            }
            else {
                pixelY = (float)startY;
                for (int x = startX; x <= endX; ++x) {
                    pixelX = (float)x;
                    pixelY = pixelY + dydx;

                    pixels[(int)pixelY * imageWidth + (int)pixelX] = finalColor; 
                }
            }
        }
        else {
            if (dydx <= -1.0f) {
                pixelX = (float)startX;
                for (int y = startY; y >= endY; --y) {
                    pixelY = (float)y;
                    pixelX = pixelX - dxdy;

                    pixels[(int)pixelY * imageWidth + (int)pixelX] = finalColor; 
                }
            }
            else {
                pixelY = (float)startY;
                for (int x = startX; x <= endX; ++x) {
                    pixelX = (float)x;
                    pixelY = pixelY + dydx;

                    pixels[(int)pixelY * imageWidth + (int)pixelX] = finalColor; 
                }

            }
        }
    }
}

void draw3DLine (unsigned int* pixels, float startX, float startY, float startZ, float endX, float endY, float endZ, 
                 matrix4x4 viewMat, matrix4x4 projMat) {
    int newStartX, newStartY, newEndX, newEndY;

    float w, oneOverW;
    vector3 startPoint = transformPoint(viewMat, Vector3(startX, startY, startZ), &w);
    startPoint = transformPoint(projMat, startPoint, &w);
    oneOverW = 1.0f / w;
    startX = (startPoint.x * oneOverW);
    startY = (startPoint.y * oneOverW);

    vector3 endPoint = transformPoint(viewMat, Vector3(endX, endY, endZ), &w);
    endPoint = transformPoint(projMat, endPoint, &w);
    oneOverW = 1.0f / w;
    endX = (endPoint.x * oneOverW);
    endY = (endPoint.y * oneOverW);

    newStartX = int((startX + 1.0f) * imageWidth * 0.5f);
    newStartY = int((1.0f - (startY + 1.0f) * 0.5f) * imageHeight);
    newEndX = int((endX + 1.0f) * imageWidth * 0.5f);
    newEndY = int((1.0f - (endY + 1.0f) * 0.5f) * imageHeight);

    drawLine(pixels, newStartX, newStartY, newEndX, newEndY);
}

inline void drawAABB (unsigned int *pixels, aabb box, matrix4x4 viewMat, matrix4x4 projMat) {
    draw3DLine(pixels, box.min.x, box.min.y, box.min.z, box.max.x, box.min.y, box.min.z, viewMat, projMat);
    draw3DLine(pixels, box.max.x, box.min.y, box.min.z, box.max.x, box.min.y, box.max.z, viewMat, projMat);
    draw3DLine(pixels, box.max.x, box.min.y, box.max.z, box.min.x, box.min.y, box.max.z, viewMat, projMat);
    draw3DLine(pixels, box.min.x, box.min.y, box.max.z, box.min.x, box.min.y, box.min.z, viewMat, projMat);

    draw3DLine(pixels, box.min.x, box.max.y, box.min.z, box.max.x, box.max.y, box.min.z, viewMat, projMat);
    draw3DLine(pixels, box.max.x, box.max.y, box.min.z, box.max.x, box.max.y, box.max.z, viewMat, projMat);
    draw3DLine(pixels, box.max.x, box.max.y, box.max.z, box.min.x, box.max.y, box.max.z, viewMat, projMat);
    draw3DLine(pixels, box.min.x, box.max.y, box.max.z, box.min.x, box.max.y, box.min.z, viewMat, projMat);

    draw3DLine(pixels, box.min.x, box.min.y, box.min.z, box.min.x, box.max.y, box.min.z, viewMat, projMat);
    draw3DLine(pixels, box.max.x, box.min.y, box.min.z, box.max.x, box.max.y, box.min.z, viewMat, projMat);
    draw3DLine(pixels, box.max.x, box.min.y, box.max.z, box.max.x, box.max.y, box.max.z, viewMat, projMat);
    draw3DLine(pixels, box.min.x, box.min.y, box.max.z, box.min.x, box.max.y, box.max.z, viewMat, projMat);
}

inline void drawKDTreeNode (unsigned int *pixels, kdtree *tree, unsigned int nodeIndex, aabb bounds, matrix4x4 viewMat, matrix4x4 projMat) {
    drawAABB(pixels, bounds, viewMat, projMat);
    kdtree_node currentNode = tree->nodes[nodeIndex];
    kdtree_split_direction splitDirection = currentNode.splitDirection;
    bool intersected = false;
    if (splitDirection != KDTREE_SPLIT_LEAF) {
        if (currentNode.firstChildIndex != KDTREE_NODE_EMPTY) {
            aabb leftBounds = bounds;
            leftBounds.max.c[splitDirection] = currentNode.splitPos;
            drawKDTreeNode(pixels, tree, currentNode.firstChildIndex, leftBounds, viewMat, projMat);
        }
        if (currentNode.secondChildIndex != KDTREE_NODE_EMPTY) {
            aabb rightBounds = bounds;
            rightBounds.min.c[splitDirection] = currentNode.splitPos;
            drawKDTreeNode(pixels, tree, currentNode.secondChildIndex, rightBounds, viewMat, projMat);
        }
    }
}

inline void drawKDTree (unsigned int *pixels, kdtree *tree, matrix4x4 viewMat, matrix4x4 projMat) {
    if (tree->numNodes > 0) {
        drawKDTreeNode(pixels, tree, 0, tree->bounds, viewMat, projMat);
    }
}

void drawMeshWireframe (unsigned int *pixels, object *object, matrix4x4 viewMat, matrix4x4 projMat) {
    matrix4x4 transform = object->transform;
    mesh mesh = meshes[object->shapeIndex];

    for (unsigned int i = 0; i < mesh.numTriangles; ++i) {
        vector3 p0 = mesh.positions[mesh.indices[i * 3]];
        vector3 p1 = mesh.positions[mesh.indices[i * 3 + 1]];
        vector3 p2 = mesh.positions[mesh.indices[i * 3 + 2]];
        p0 = transform * p0;
        p1 = transform * p1;
        p2 = transform * p2;

        draw3DLine(pixels, p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, viewMat, projMat);
        draw3DLine(pixels, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, viewMat, projMat);
        draw3DLine(pixels, p2.x, p2.y, p2.z, p0.x, p0.y, p0.z, viewMat, projMat);
    }
}


void renderScene (unsigned int *pixels) {
    float filmWidth  = 0.035f;
    float filmHeight  = filmWidth / (16.0f / 9.0f);
    float focalLength = 0.050f;

    float xStep = filmWidth / imageWidth;
    float yStep = filmHeight / imageHeight;
    float halfPixelWidth = xStep * 0.5f;
    float halfPixelHeight = yStep * 0.5f;

    vector3 rayPos = Vector3(-6.0f, 6.5f, -6.0f);
    quaternion cameraRotation = quaternionFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), 
                                                        PI * 0.25f);
    cameraRotation *= quaternionFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 
                                              PI * 0.1875f);
    // roll
    //cameraRotation *= quaternionFromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), 
    //                                          PI * -0.5f);

    // straight on view
    //vector3 rayPos = Vector3(0.0f, 0.75f, -10.0f);
    //quaternion cameraRotation = quaternionFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 0.0f);

    // straight on view
    //vector3 rayPos = Vector3(0.0f, 0.75f, -300.0f);
    //quaternion cameraRotation = quaternionFromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), 0.0f);

    int iterations = 0;
    while (iterations < 1000) {
        iterations++;
        vector3 color = {};
        int numSamples = numSuperSamples;
        float sampleStep = 1.0f / numSamples;
        float sampleReciprocal = 1.0f / (numSamples * numSamples);
        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < numSamples; ++j) {
                vector3 rayDir = Vector3(-filmWidth * 0.5f + currentCol * xStep,
                                          filmHeight * 0.5f - currentRow * yStep,
                                          focalLength);

                rayDir.x += (-halfPixelWidth + j * sampleStep * xStep);
                rayDir.y += (halfPixelHeight - i * sampleStep * yStep);
                rayDir = normalize(rayDir);

                rayDir = rotateVectorByQuaternion(rayDir, cameraRotation);

                color += sampleReciprocal * calcColor(rayPos, rayDir, 1);
//                vector3 newRayPos = rayPos;
//                newRayPos.x += ((float)((float)rand() / RAND_MAX) - 0.5f) * 0.05f;
//                newRayPos.y += ((float)((float)rand() / RAND_MAX) - 0.5f) * 0.05f;
//                rayDir = normalize(rayDir);
//
//                rayDir = rotateVectorByQuaternion(rayDir, cameraRotation);
//
//                color += sampleReciprocal * calcColor(newRayPos, rayDir, 1);
            }
        }

        // Clamp colors so they don't overflow
        // TODO(ebuchholz): clamp/max for vector3
        color.r = color.r > 1.0f ? 1.0f : color.r;
        color.g = color.g > 1.0f ? 1.0f : color.g;
        color.b = color.b > 1.0f ? 1.0f : color.b;

        // gamma correction or something
        // TODO(ebuchholz): powf for vector3
        float gammaCurve = 1.0f / 2.2f;
        color.r = powf(color.r, gammaCurve);
        color.g = powf(color.g, gammaCurve);
        color.b = powf(color.b, gammaCurve);

        // copy to buffer
        unsigned int finalColor = ((int)(255.0f * 1.0f)) << 24 |
                                  ((int)(255.0f * color.b)) << 16 |
                                  ((int)(255.0f * color.g)) << 8 |
                                  ((int)(255.0f * color.r));

        pixels[currentRow * imageWidth + currentCol] = finalColor; 
        ++currentCol;
        if (currentCol == imageWidth) {
            currentCol = 0;
            ++currentRow;
            if (currentRow == imageHeight) {
                renderingFinished = true;
            }
        }
    }

    // Draw debug rasterized things
    // KD-tree visualization
    float aspectRatio = 16.0f / 9.0f;
    float fov = (2.0f * atan2f((filmWidth / 2.0f), focalLength)) * (180.0f / PI);
    matrix4x4 viewMatrix = createViewMatrix(cameraRotation, rayPos.x, rayPos.y, rayPos.z);
    matrix4x4 perspMatrix = createPerspectiveMatrix(0.001f, 100.0f, aspectRatio, fov);

    //drawMeshWireframe(&objects[2], viewMatrix, perspMatrix);
    //drawKDTree(&theOneKDTreeInTheScene, viewMatrix, perspMatrix);
}

extern "C" void updateGame (int gameWidth, int gameHeight, unsigned int *pixels) { 
    //unsigned int value;
    // clear
    //for (int i = 0; i < gameHeight; ++i) {
    //    value = 0xff000000;
    //    //value = 0;
    //    for (int j = 0; j < gameWidth; ++j) {
    //        //pixels[i * gameWidth + j] = value << 8;
    //        //++value;
    //        pixels[i * gameWidth + j] = value;
    //    }
    //}

    //int startX = boxPosition;
    //int endX = startX + 100;
    //for (int i = 100; i < 200; ++i) {
    //    value = 0xffff00ff;
    //    //value = 0;
    //    for (int j = startX; j < endX; ++j) {
    //        //pixels[i * gameWidth + j] = value << 8;
    //        //++value;
    //        pixels[i * gameWidth + j] = value;
    //    }
    //}
    //if (endX < 1000) { boxPosition++; }

    if (!sceneInited) {
        initScene();
        sceneInited = true;

    }
    renderScene(pixels);
}
