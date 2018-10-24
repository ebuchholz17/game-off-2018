#ifndef EBRT_H
#define EBRT_H

// TODO(ebuchholz): Better constants
#define PI 3.14159265359f

union matrix3x3 {
    float m[9];
    // TODO(ebuchholz): see if we can add some other ways to access the elements
};

inline matrix3x3 operator* (matrix3x3 a, matrix3x3 b) {
    matrix3x3 result;

    result.m[0] = a.m[0]*b.m[0] + a.m[1]*b.m[3] + a.m[2]*b.m[6];
    result.m[1] = a.m[0]*b.m[1] + a.m[1]*b.m[4] + a.m[2]*b.m[7];
    result.m[2] = a.m[0]*b.m[2] + a.m[1]*b.m[5] + a.m[2]*b.m[8];

    result.m[3] = a.m[3]*b.m[0] + a.m[4]*b.m[3] + a.m[5]*b.m[6]; 
    result.m[4] = a.m[3]*b.m[1] + a.m[4]*b.m[4] + a.m[5]*b.m[7]; 
    result.m[5] = a.m[3]*b.m[2] + a.m[4]*b.m[5] + a.m[5]*b.m[8];

    result.m[6] = a.m[6]*b.m[0] + a.m[7]*b.m[3] + a.m[8]*b.m[6]; 
    result.m[7] = a.m[6]*b.m[1] + a.m[7]*b.m[4] + a.m[8]*b.m[7]; 
    result.m[8] = a.m[6]*b.m[2] + a.m[7]*b.m[5] + a.m[8]*b.m[8];

    return result;
}

inline matrix3x3 operator* (float a, matrix3x3 b) {
    matrix3x3 result;

    result.m[0] = a*b.m[0];
    result.m[1] = a*b.m[1];
    result.m[2] = a*b.m[2];
    result.m[3] = a*b.m[3];
    result.m[4] = a*b.m[4];
    result.m[5] = a*b.m[5];
    result.m[6] = a*b.m[6];
    result.m[7] = a*b.m[7];
    result.m[8] = a*b.m[8];

    return result;
}

inline matrix3x3 transpose (matrix3x3 a) {
    matrix3x3 result;

    result.m[0] = a.m[0];
    result.m[1] = a.m[3];
    result.m[2] = a.m[6];
    result.m[3] = a.m[1];
    result.m[4] = a.m[4];
    result.m[5] = a.m[7];
    result.m[6] = a.m[2];
    result.m[7] = a.m[5];
    result.m[8] = a.m[8];

    return result;
}
// TODO(ebuchholz): maybe a 3x3 inverse transpose for tranforming normals

// TODO(ebuchholz): implement
inline matrix3x3 inverse (matrix3x3 a) {
    // 0 1 2
    // 3 4 5
    // 6 7 8

    matrix3x3 minors = {};

    minors.m[0] = a.m[4]*a.m[8] - a.m[5]*a.m[7];
    minors.m[1] = -(a.m[3]*a.m[8] - a.m[5]*a.m[6]);
    minors.m[2] = a.m[3]*a.m[7] - a.m[4]*a.m[6];

    minors.m[3] = -(a.m[1]*a.m[8] - a.m[2]*a.m[7]);
    minors.m[4] = a.m[0]*a.m[8] - a.m[2]*a.m[6];
    minors.m[5] = -(a.m[0]*a.m[7] - a.m[1]*a.m[6]);

    minors.m[6] = a.m[1]*a.m[5] - a.m[2]*a.m[4];
    minors.m[7] = -(a.m[0]*a.m[5] - a.m[2]*a.m[3]);
    minors.m[8] = a.m[0]*a.m[4] - a.m[1]*a.m[3];

    float determinant = a.m[0]*minors.m[0] + a.m[1]*minors.m[1] + a.m[2]*minors.m[2]; // Check for 0?
    float ood = 1.0f / determinant;

    // need * operator for float * matri3x3
    matrix3x3 result = ood * transpose(minors);

    return result;
}

union matrix4x4 {
    float m[16];

    // TODO(ebuchholz): 2 dimensional array? struct with individual members? [] operator?
};


inline matrix4x4 operator* (matrix4x4 a, matrix4x4 b) {
    matrix4x4 result;

    result.m[0] = a.m[0]*b.m[0] + a.m[1]*b.m[4] + a.m[2]*b.m[8]  + a.m[3]*b.m[12];
    result.m[1] = a.m[0]*b.m[1] + a.m[1]*b.m[5] + a.m[2]*b.m[9]  + a.m[3]*b.m[13];
    result.m[2] = a.m[0]*b.m[2] + a.m[1]*b.m[6] + a.m[2]*b.m[10] + a.m[3]*b.m[14];
    result.m[3] = a.m[0]*b.m[3] + a.m[1]*b.m[7] + a.m[2]*b.m[11] + a.m[3]*b.m[15];

    result.m[4] = a.m[4]*b.m[0] + a.m[5]*b.m[4] + a.m[6]*b.m[8]  + a.m[7]*b.m[12];
    result.m[5] = a.m[4]*b.m[1] + a.m[5]*b.m[5] + a.m[6]*b.m[9]  + a.m[7]*b.m[13];
    result.m[6] = a.m[4]*b.m[2] + a.m[5]*b.m[6] + a.m[6]*b.m[10] + a.m[7]*b.m[14];
    result.m[7] = a.m[4]*b.m[3] + a.m[5]*b.m[7] + a.m[6]*b.m[11] + a.m[7]*b.m[15];

    result.m[8]  = a.m[8]*b.m[0] + a.m[9]*b.m[4] + a.m[10]*b.m[8]  + a.m[11]*b.m[12];
    result.m[9]  = a.m[8]*b.m[1] + a.m[9]*b.m[5] + a.m[10]*b.m[9]  + a.m[11]*b.m[13];
    result.m[10] = a.m[8]*b.m[2] + a.m[9]*b.m[6] + a.m[10]*b.m[10] + a.m[11]*b.m[14];
    result.m[11] = a.m[8]*b.m[3] + a.m[9]*b.m[7] + a.m[10]*b.m[11] + a.m[11]*b.m[15];

    result.m[12] = a.m[12]*b.m[0] + a.m[13]*b.m[4] + a.m[14]*b.m[8]  + a.m[15]*b.m[12];
    result.m[13] = a.m[12]*b.m[1] + a.m[13]*b.m[5] + a.m[14]*b.m[9]  + a.m[15]*b.m[13];
    result.m[14] = a.m[12]*b.m[2] + a.m[13]*b.m[6] + a.m[14]*b.m[10] + a.m[15]*b.m[14];
    result.m[15] = a.m[12]*b.m[3] + a.m[13]*b.m[7] + a.m[14]*b.m[11] + a.m[15]*b.m[15];

    return result;
}

inline matrix4x4 operator* (float a, matrix4x4 b) {
    matrix4x4 result;

    result.m[0] = a*b.m[0];
    result.m[1] = a*b.m[1];
    result.m[2] = a*b.m[2];
    result.m[3] = a*b.m[3];
    result.m[4] = a*b.m[4];
    result.m[5] = a*b.m[5];
    result.m[6] = a*b.m[6];
    result.m[7] = a*b.m[7];
    result.m[8] = a*b.m[8];
    result.m[9] = a*b.m[9];
    result.m[10] = a*b.m[10];
    result.m[11] = a*b.m[11];
    result.m[12] = a*b.m[12];
    result.m[13] = a*b.m[13];
    result.m[14] = a*b.m[14];
    result.m[15] = a*b.m[15];

    return result;
}

inline matrix4x4 transpose (matrix4x4 a) {
    matrix4x4 result;

    result.m[0] = a.m[0];
    result.m[1] = a.m[4];
    result.m[2] = a.m[8];
    result.m[3] = a.m[12];
    result.m[4] = a.m[1];
    result.m[5] = a.m[5];
    result.m[6] = a.m[9];
    result.m[7] = a.m[13];
    result.m[8] = a.m[2];
    result.m[9] = a.m[6];
    result.m[10] = a.m[10];
    result.m[11] = a.m[14];
    result.m[12] = a.m[3];
    result.m[13] = a.m[7];
    result.m[14] = a.m[11];
    result.m[15] = a.m[15];

    return result;
}

// TODO(ebuchholz): fix this travesty
inline matrix4x4 inverse (matrix4x4 a) {
    matrix4x4 minors = {};

    float a0m0 = a.m[10]*a.m[15] - a.m[11]*a.m[14];
    float a0m1 = a.m[9]*a.m[15] - a.m[11]*a.m[13];
    float a0m2 = a.m[9]*a.m[14] - a.m[10]*a.m[13];
    minors.m[0] = a.m[5]*a0m0 - a.m[6]*a0m1 + a.m[7]*a0m2;

    float a1m0 = a.m[10]*a.m[15] - a.m[11]*a.m[14];
    float a1m1 = a.m[8]*a.m[15] - a.m[11]*a.m[12];
    float a1m2 = a.m[8]*a.m[14] - a.m[10]*a.m[12];
    minors.m[1] = -(a.m[4]*a1m0 - a.m[6]*a1m1 + a.m[7]*a1m2);

    float a2m0 = a.m[9]*a.m[15] - a.m[11]*a.m[13];
    float a2m1 = a.m[8]*a.m[15] - a.m[11]*a.m[12];
    float a2m2 = a.m[8]*a.m[13] - a.m[9]*a.m[12];
    minors.m[2] = a.m[4]*a2m0 - a.m[5]*a2m1 + a.m[7]*a2m2;

    float a3m0 = a.m[9]*a.m[14] - a.m[10]*a.m[13];
    float a3m1 = a.m[8]*a.m[14] - a.m[10]*a.m[12];
    float a3m2 = a.m[8]*a.m[13] - a.m[9]*a.m[12];
    minors.m[3] = -(a.m[4]*a3m0 - a.m[5]*a3m1 + a.m[6]*a3m2);


    float a4m0 = a.m[10]*a.m[15] - a.m[11]*a.m[14];
    float a4m1 = a.m[9]*a.m[15] - a.m[11]*a.m[13];
    float a4m2 = a.m[9]*a.m[14] - a.m[10]*a.m[13];
    minors.m[4] = -(a.m[1]*a4m0 - a.m[2]*a4m1 + a.m[3]*a4m2);

    float a5m0 = a.m[10]*a.m[15] - a.m[11]*a.m[14];
    float a5m1 = a.m[8]*a.m[15] - a.m[11]*a.m[12];
    float a5m2 = a.m[8]*a.m[14] - a.m[10]*a.m[12];
    minors.m[5] = a.m[0]*a5m0 - a.m[2]*a5m1 + a.m[3]*a5m2;

    float a6m0 = a.m[9]*a.m[15] - a.m[11]*a.m[13];
    float a6m1 = a.m[8]*a.m[15] - a.m[11]*a.m[12];
    float a6m2 = a.m[8]*a.m[13] - a.m[9]*a.m[12];
    minors.m[6] = -(a.m[0]*a6m0 - a.m[1]*a6m1 + a.m[3]*a6m2);

    float a7m0 = a.m[9]*a.m[14] - a.m[10]*a.m[13];
    float a7m1 = a.m[8]*a.m[14] - a.m[10]*a.m[12];
    float a7m2 = a.m[8]*a.m[13] - a.m[9]*a.m[12];
    minors.m[7] = a.m[0]*a7m0 - a.m[1]*a7m1 + a.m[2]*a7m2;


    float a8m0 = a.m[6]*a.m[15] - a.m[7]*a.m[14];
    float a8m1 = a.m[5]*a.m[15] - a.m[7]*a.m[13];
    float a8m2 = a.m[5]*a.m[14] - a.m[6]*a.m[13];
    minors.m[8] = a.m[1]*a8m0 - a.m[2]*a8m1 + a.m[3]*a8m2;

    float a9m0 = a.m[6]*a.m[15] - a.m[7]*a.m[14];
    float a9m1 = a.m[4]*a.m[15] - a.m[7]*a.m[12];
    float a9m2 = a.m[4]*a.m[14] - a.m[6]*a.m[12];
    minors.m[9] = -(a.m[0]*a9m0 - a.m[2]*a9m1 + a.m[3]*a9m2);

    float a10m0 = a.m[5]*a.m[15] - a.m[7]*a.m[13];
    float a10m1 = a.m[4]*a.m[15] - a.m[7]*a.m[12];
    float a10m2 = a.m[4]*a.m[13] - a.m[5]*a.m[12];
    minors.m[10] = a.m[0]*a10m0 - a.m[1]*a10m1 + a.m[3]*a10m2;

    float a11m0 = a.m[5]*a.m[14] - a.m[6]*a.m[13];
    float a11m1 = a.m[8]*a.m[14] - a.m[6]*a.m[12];
    float a11m2 = a.m[8]*a.m[13] - a.m[5]*a.m[12];
    minors.m[11] = -(a.m[0]*a11m0 - a.m[1]*a11m1 + a.m[2]*a11m2);

    //  0  1  2  3
    //  4  5  6  7
    //  8  9 10 11
    // 12 13 14 15

    float a12m0 = a.m[6]*a.m[11] - a.m[7]*a.m[10];
    float a12m1 = a.m[5]*a.m[11] - a.m[7]*a.m[9];
    float a12m2 = a.m[5]*a.m[10] - a.m[6]*a.m[9];
    minors.m[12] = -(a.m[1]*a12m0 - a.m[2]*a12m1 + a.m[3]*a12m2);

    float a13m0 = a.m[6]*a.m[11] - a.m[7]*a.m[10];
    float a13m1 = a.m[4]*a.m[11] - a.m[7]*a.m[8];
    float a13m2 = a.m[4]*a.m[10] - a.m[6]*a.m[8];
    minors.m[13] = a.m[0]*a13m0 - a.m[2]*a13m1 + a.m[3]*a13m2;

    float a14m0 = a.m[5]*a.m[11] - a.m[7]*a.m[9];
    float a14m1 = a.m[4]*a.m[11] - a.m[7]*a.m[8];
    float a14m2 = a.m[4]*a.m[9] - a.m[5]*a.m[8];
    minors.m[14] = -(a.m[0]*a14m0 - a.m[1]*a14m1 + a.m[3]*a14m2);

    float a15m0 = a.m[5]*a.m[10] - a.m[6]*a.m[9];
    float a15m1 = a.m[4]*a.m[10] - a.m[6]*a.m[8];
    float a15m2 = a.m[4]*a.m[9] - a.m[5]*a.m[8];
    minors.m[15] = a.m[0]*a15m0 - a.m[1]*a15m1 + a.m[2]*a15m2;

    float determinant = a.m[0]*minors.m[0] + a.m[1]*minors.m[1] +
                        a.m[2]*minors.m[2] + a.m[3]*minors.m[3];
    float ood = 1.0f / determinant;

    matrix4x4 result = ood * transpose(minors);

    return result;
}

inline matrix4x4 identityMatrix4x4 () {
    matrix4x4 result = {};

    result.m[0] = 1.0f;
    result.m[5] = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;

    return result;
}

inline matrix4x4 translationMatrix (float x, float y, float z) {
    matrix4x4 result = identityMatrix4x4();

    result.m[3] = x;
    result.m[7] = y;
    result.m[11] = z;

    return result;
}

inline matrix4x4 scaleMatrix (float s) {
    matrix4x4 result = {};

    result.m[0] = s;
    result.m[5] = s;
    result.m[10] = s;
    result.m[15] = 1.0f;

    return result;
}

inline matrix4x4 scaleMatrix (float x, float y, float z) {
    matrix4x4 result = {};

    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    result.m[15] = 1.0f;

    return result;
}
//public static createViewMatrix (rotation: Quaternion, x: number, y: number, z: number): number[] {
//    let viewMatrix = MathUtil.quaternionMatrix(rotation);
//    viewMatrix = MathUtil.matrixTranspose4x4(viewMatrix);
//
//    let newCamX = -MathUtil.dotProduct([x, y, z], [viewMatrix[0], viewMatrix[4], viewMatrix[8]]);
//    let newCamY = -MathUtil.dotProduct([x, y, z], [viewMatrix[1], viewMatrix[5], viewMatrix[9]]);
//    let newCamZ = -MathUtil.dotProduct([x, y, z], [viewMatrix[2], viewMatrix[6], viewMatrix[10]]);
//    viewMatrix[12] = newCamX;
//    viewMatrix[13] = newCamY;
//    viewMatrix[14] = newCamZ;
//
//    return viewMatrix;
//}



inline float square (float a) {
    return a * a;
}

//inline float max (float a, float b) {
//    return a > b ? a : b;
//}
//
//inline float min (float a, float b) {
//    return a < b ? a : b;
//}

union vector3 {
    struct {
        float x;
        float y;
        float z;
    };
    struct {
        float r;
        float g;
        float b;
    };
    float c[3];
};

inline vector3 Vector3 (float x = 0.0f, float y = 0.0f, float z = 0.0f) {
    vector3 result;

    result.x = x;
    result.y = y;
    result.z = z;

    return result;
};

inline vector3 operator+ (vector3 a, vector3 b) {
    vector3 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

inline vector3 & operator+= (vector3 &a, vector3 b) {
    a = a + b;
    return a;
}

inline vector3 operator- (vector3 a, vector3 b) {
    vector3 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

inline vector3 operator- (vector3 a) {
    vector3 result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;

    return result;
}


inline vector3 & operator-= (vector3 &a, vector3 b) {
    a = a - b;
    return a;
}

inline vector3 operator* (float a, vector3 b) {
    vector3 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return result;
}

inline vector3 operator* (vector3 a, float b) {
    vector3 result = b * a;
    return result;
}

inline vector3 & operator*= (vector3 &a, float b) {
    a = b * a;
    return a;
}

// think this is called the hadamard product
inline vector3 operator* (vector3 a, vector3 b) {
    vector3 result;

    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;

    return result;
}

inline vector3 & operator*= (vector3 &a, vector3 b) {
    a = a * b;
    return a;
}

inline float dotProduct (vector3 a, vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vector3 crossProduct (vector3 a, vector3 b) {
    vector3 result;

    result.x = -b.y * a.z + a.y * b.z;
    result.y = -b.z * a.x + a.z * b.x;
    result.z = -b.x * a.y + a.x * b.y;

    return result; // TODO(ebuchholz): double check
}

inline vector3 operator* (matrix3x3 m, vector3 v){
    vector3 result;

    result.x = m.m[0]*v.x + m.m[1]*v.y + m.m[2]*v.z;
    result.y = m.m[3]*v.x + m.m[4]*v.y + m.m[5]*v.z;
    result.z = m.m[6]*v.x + m.m[7]*v.y + m.m[8]*v.z;

    return result;
}

// Multiplies the vector like a point
inline vector3 operator* (matrix4x4 m, vector3 v){
    vector3 result;

    result.x = m.m[0]*v.x + m.m[1]*v.y +  m.m[2]*v.z +  m.m[3]*1.0f;
    result.y = m.m[4]*v.x + m.m[5]*v.y +  m.m[6]*v.z +  m.m[7]*1.0f;
    result.z = m.m[8]*v.x + m.m[9]*v.y + m.m[10]*v.z + m.m[11]*1.0f;
    // w component would be here
    // TODO(ebuchholz): check if this is a good idea

    return result;
}

inline vector3 transformPoint (matrix4x4 m, vector3 v, float* w){
    vector3 result;

    result.x = m.m[0]*v.x + m.m[1]*v.y +  m.m[2]*v.z +  m.m[3]*1.0f;
    result.y = m.m[4]*v.x + m.m[5]*v.y +  m.m[6]*v.z +  m.m[7]*1.0f;
    result.z = m.m[8]*v.x + m.m[9]*v.y + m.m[10]*v.z + m.m[11]*1.0f;

    // w as an out parameter so we don't have to make a vector4
    *w = m.m[12]*v.x + m.m[13]*v.y + m.m[14]*v.z + m.m[15]*1.0f;

    return result;
}

// Multiplies the vector like a direction
// TODO(ebuchholz): make a separate point data type?
inline vector3 transformDirection (matrix4x4 m, vector3 v) {
    vector3 result;

    result.x = m.m[0]*v.x + m.m[1]*v.y +  m.m[2]*v.z +  m.m[3]*0.0f;
    result.y = m.m[4]*v.x + m.m[5]*v.y +  m.m[6]*v.z +  m.m[7]*0.0f;
    result.z = m.m[8]*v.x + m.m[9]*v.y + m.m[10]*v.z + m.m[11]*0.0f;
    // w component would be here
    // TODO(ebuchholz): check if this is a good idea

    return result;
}

inline matrix4x4 translationMatrix (vector3 position) {
    return translationMatrix(position.x, position.y, position.z);
}

inline float lengthSquared (vector3 a) {
    return dotProduct(a, a);
}

inline float length (vector3 a) {
    return sqrtf(lengthSquared(a));
}

inline vector3 normalize (vector3 a) {
    return a * (1.0f / length(a));
}

// TODO(ebuchholz): double check all of this quaternion stuff, it's ported from nfl thrower 
// typescript code
struct quaternion {
    float w;
    float x;
    float y;
    float z;
};

inline quaternion Quaternion (float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) {
    quaternion result;

    result.w = w;
    result.x = x;
    result.y = y;
    result.z = z;

    return result;
}

inline quaternion conjugate (quaternion a) {
    quaternion result;

    result.w = a.w;
    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;

    return result;
}

inline quaternion normalize (quaternion a) {
    quaternion result;

    float magnitude = sqrtf(a.w * a.w + a.x * a.x + a.y * a.y + a.z * a.z);
    float oneOverMag = 1.0f / magnitude;
    result.w = a.w * oneOverMag;
    result.x = a.x * oneOverMag;
    result.y = a.y * oneOverMag;
    result.z = a.z * oneOverMag;

    return result;
}

// TODO(ebuchholz): triple check this one
inline quaternion operator* (quaternion a, quaternion b) {
    quaternion result;

    result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    result.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    result.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;

    return result;
}

inline quaternion & operator*= (quaternion &a, quaternion b) {
    a = a * b;
    return a;
}

// TODO(ebuchholz): quadruple check this one
inline vector3 rotateVectorByQuaternion (vector3 v, quaternion q) {
    quaternion qConjugate = conjugate(q);

    quaternion quaternionVersionOfVector;
    quaternionVersionOfVector.w = 0.0f;
    quaternionVersionOfVector.x = v.x;
    quaternionVersionOfVector.y = v.y;
    quaternionVersionOfVector.z = v.z;

    quaternion newQ = q * quaternionVersionOfVector * qConjugate;

    return Vector3(newQ.x, newQ.y, newQ.z);
}

inline quaternion quaternionFromAxisAngle (vector3 axis, float angle) {
    quaternion result;
    float angleOverTwo = angle / 2.0f;
    float sinAngleOverTwo = sinf(angleOverTwo);

    result.w = cosf(angleOverTwo);
    result.x = axis.x * sinAngleOverTwo;
    result.y = axis.y * sinAngleOverTwo;
    result.z = axis.z * sinAngleOverTwo;

    return result;
}

inline float dotProduct (quaternion a, quaternion b) {
    return a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
}

//TODO(ebuchholz): quintuple-check this one, may need to transpose
inline matrix4x4 matrix4x4FromQuaternion (quaternion q) {
    matrix4x4 result;

    result.m[0] = 1.0f - (2.0f*square(q.y)) - (2.0f*square(q.z));
    result.m[1] = (2.0f*q.x*q.y) - (2.0f*q.w*q.z);
    result.m[2] = (2.0f*q.x*q.z) + (2.0f*q.w*q.y);
    result.m[3] = 0.0f;

    result.m[4] = (2.0f*q.x*q.y) + (2.0f*q.w*q.z);
    result.m[5] = 1.0f - (2.0f*square(q.x)) - (2.0f*square(q.z));
    result.m[6] = (2.0f*q.y*q.z) - (2.0f*q.w*q.x);
    result.m[7] = 0.0f;

    result.m[8] = (2.0f*q.x*q.z) - (2.0f*q.w*q.y);
    result.m[9] = (2.0f*q.y*q.z) + (2.0f*q.w*q.x);
    result.m[10] = 1.0f - (2.0f*square(q.x)) - (2.0f*square(q.y));
    result.m[11] = 0.0f;

    result.m[12] = 0.0f;
    result.m[13] = 0.0f;
    result.m[14] = 0.0f;
    result.m[15] = 1.0f;

    return result;
}

inline matrix4x4 rotationMatrixFromAxisAngle (vector3 axis, float angle) {
    matrix4x4 result;

    quaternion axisAngleQuaternion = quaternionFromAxisAngle(axis, angle);
    result = matrix4x4FromQuaternion(axisAngleQuaternion);

    return result;
}

// TODO(ebuchholz): check this one a few times
inline quaternion lerp (quaternion a, quaternion b, float t) {
    quaternion result;
    float oneMinusT = 1.0f - t;

    result.w = oneMinusT*a.w + t*b.w;
    result.x = oneMinusT*a.x + t*b.x;
    result.y = oneMinusT*a.y + t*b.y;
    result.z = oneMinusT*a.z + t*b.z;

    result = normalize(result);
    return result;
}

inline matrix4x4 createViewMatrix (quaternion rotation, float x, float y, float z) {
    matrix4x4 result = matrix4x4FromQuaternion(rotation);
    result = transpose(result);

    float newCamX = dotProduct(Vector3(x, y, z), Vector3(result.m[0], result.m[1], result.m[2]));
    float newCamY = dotProduct(Vector3(x, y, z), Vector3(result.m[4], result.m[5], result.m[6]));
    float newCamZ = dotProduct(Vector3(x, y, z), Vector3(result.m[8], result.m[9], result.m[10]));
    result.m[3] = -newCamX;
    result.m[7] = -newCamY;
    result.m[11] = -newCamZ;

    return result;
}

inline matrix4x4 createPerspectiveMatrix (float nearPlane, float farPlane, float aspectRatio, float fov) {
    float fovy = 2.0f * atanf(tanf(fov * (PI / 180.0f) / 2.0f) * (1.0f / aspectRatio));
    float f = 1.0f / tanf(fovy / 2.0f);
    float nf = 1.0f / (farPlane - nearPlane);

    matrix4x4 result = identityMatrix4x4();

    result.m[0] = f / aspectRatio;
    result.m[5] = f;
    result.m[10] = -(farPlane) * nf;
    result.m[11] = -(farPlane * nearPlane) * nf;
    result.m[14] = 1.0f;
    result.m[15] = 0;

    return result;
}

struct aabb {
    vector3 min;
    vector3 max;
};

aabb getTriangleBounds (vector3 p0, vector3 p1, vector3 p2) {
    aabb result;

    result.min.x = p0.x < p1.x ? p0.x : p1.x;
    result.min.y = p0.y < p1.y ? p0.y : p1.y;
    result.min.z = p0.z < p1.z ? p0.z : p1.z;

    result.max.x = p0.x > p1.x ? p0.x : p1.x;
    result.max.y = p0.y > p1.y ? p0.y : p1.y;
    result.max.z = p0.z > p1.z ? p0.z : p1.z;

    result.min.x = result.min.x < p2.x ? result.min.x : p2.x;
    result.min.y = result.min.y < p2.y ? result.min.y : p2.y;
    result.min.z = result.min.z < p2.z ? result.min.z : p2.z;

    result.max.x = result.max.x > p2.x ? result.max.x : p2.x;
    result.max.y = result.max.y > p2.y ? result.max.y : p2.y;
    result.max.z = result.max.z > p2.z ? result.max.z : p2.z;

    return result;
}

aabb unionAABB (aabb a, aabb b) {
    aabb result;

    result.min.x = a.min.x < b.min.x ? a.min.x : b.min.x;
    result.min.y = a.min.y < b.min.y ? a.min.y : b.min.y;
    result.min.z = a.min.z < b.min.z ? a.min.z : b.min.z;

    result.max.x = a.max.x > b.max.x ? a.max.x : b.max.x;
    result.max.y = a.max.y > b.max.y ? a.max.y : b.max.y;
    result.max.z = a.max.z > b.max.z ? a.max.z : b.max.z;

    return result;
}

bool aabbIntersection (aabb a, aabb b) {
    if (a.max.x < b.min.x) { return false; }
    if (a.min.x > b.max.x) { return false; }
    if (a.max.y < b.min.y) { return false; }
    if (a.min.y > b.max.y) { return false; }
    if (a.max.z < b.min.z) { return false; }
    if (a.min.z > b.max.z) { return false; }
    return true;
}

float aabbSurfaceArea (aabb a) {
    float width = a.max.x - a.min.x;
    float depth = a.max.z - a.min.z;
    float height = a.max.y - a.min.y;

    return 2.0f * (width * depth + width * height + depth * height);
}

int aabbAxisWithMaximumExtent (aabb a) {
    int max = 0;
    if (a.max.y - a.min.y > a.max.x - a.min.x) {
        max = 1;
        if (a.max.z - a.min.z > a.max.y - a.min.y) {
            max = 2;
        }
    }
    else {
        if (a.max.z - a.min.z > a.max.x - a.min.x) {
            max = 2;
        }
    }

    return max;
}

struct sphere {
    vector3 pos;
    float radius;
};

inline sphere Sphere (vector3 pos, float radius) {
    sphere result;

    result.pos = pos;
    result.radius = radius;

    return result;
};

struct plane {
    vector3 normal;
    float distance;
};

inline plane Plane (vector3 normal, float distance) {
    plane result;

    result.normal = normal;
    result.distance = distance;

    return result;
}

struct triangle {
    vector3 p0;
    vector3 p1;
    vector3 p2;
};

struct mesh_triangle {
    vector3 p0;
    vector3 p1;
    vector3 p2;
    vector3 n0;
    vector3 n1;
    vector3 n2;
};

inline triangle Triangle(vector3 p0, vector3 p1, vector3 p2) {
    triangle result;

    result.p0 = p0;
    result.p1 = p1;
    result.p2 = p2;

    return result;
}

// TODO(ebuchholz): figure out what this guy needs... numPositions?
struct mesh {
    vector3 *positions;
    vector3 *normals;
    unsigned int *indices;
    unsigned int numTriangles;
};

enum shape_type {
    SPHERE,
    PLANE,
    TRIANGLE,
    MESH
};

struct material {
    vector3 color;
    float roughness;
};

// TODO(ebuchholz): probably should set this up so that not evey object has a transform... maybe
// transformed meshes/spheres/planes/etc. are different shape_type's
struct object {
    shape_type shapeType;
    int shapeIndex;
    int materialIndex;
    matrix4x4 transform;
    matrix4x4 inverseTransform;
    // bounds
};

enum bound_edge_type { BOUND_EDGE_START, BOUND_EDGE_END };

struct bound_edge{
    float t;
    int objectIndex;
    bound_edge_type type;
};

inline bound_edge BoundEdge (float t, int objectIndex, bool starting) {
    bound_edge result;

    result.t = t;
    result.objectIndex = objectIndex;
    result.type = starting ? BOUND_EDGE_START : BOUND_EDGE_END;

    return result;
}

// Used by qsort to sort the bound_edge's by t
int compareBoundEdges (const void *a, const void *b) {
    bound_edge *edgeA = (bound_edge *)a;
    bound_edge *edgeB = (bound_edge *)b;
    if(edgeA->t == edgeB->t) {
        if((int)edgeA->type < (int)edgeB->type) {
            return -1;
        }
        else {
            return 1;
        }
    }
    else if (edgeA->t < edgeB->t) {
        return -1;
    }
    else {
        return 1;
    }
}

#define KDTREE_MIN_OBJECTS_PER_NODE (4)
#define KDTREE_MAX_DEPTH (64)
// TODO(ebuchholz): maybe just remove empty nodes and have leaves with 0 objects in them
#define KDTREE_NODE_EMPTY 0xffffffff

// TODO(ebuchholz): measure and determine best costs, these values are from pbrt
#define KDTREE_INTERSECTION_COST (80)
#define KDTREE_TRAVERSAL_COST (1)
#define KDTREE_EMPTY_BONUS (0.2f)

//for testing
#define KDTREE_USE_SURFACE_AREA_HEURISTIC

// maybe don't need this
enum kdtree_split_direction {
    KDTREE_SPLIT_X,
    KDTREE_SPLIT_Y,
    KDTREE_SPLIT_Z,
    KDTREE_SPLIT_LEAF
};

struct kdtree_node {
    kdtree_split_direction splitDirection;
    float splitPos;

    unsigned int firstChildIndex;
    unsigned int secondChildIndex;

    unsigned int objectArrayIndex; // index into kdtree list of objects
    unsigned int numObjects;

    // TODO(ebuchholz): shrink the size of this guy like in pbrt
    // small version of this struct is something like this
    //unsigned int splitDirection; // Flag for x/y/z split, or leaf
    //union {
    //    unsigned int secondChildIndex; // index into second child, first one is always right 
    //                                   // after the current one
    //    unsigned int numObjects;
    //}
    //union {
    //    float splitPos;
    //    unsigned int objectArrayIndex; // index into kdtree list of objects
    //}
};

// Used to process kdtree traversal from front to back
struct kdtree_work_record {
    unsigned int nodeIndex;
    float tMin;
    float tMax;
};

struct kdtree {
    aabb bounds;
    mesh_triangle *refinedTriangles;

    kdtree_node *nodes;
    unsigned int numNodes;
    unsigned int nodeCapacity;

    unsigned int *objectIndices;
    unsigned int numObjects;
    unsigned int objectCapacity;

    unsigned int materialIndex;
    int maxDepth;
};

#endif
