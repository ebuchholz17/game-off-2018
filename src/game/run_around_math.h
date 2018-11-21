#ifndef RUN_AROUND_MATH_H
#define RUN_AROUND_MATH_H

#include <math.h> // TODO(ebuchholz): see if we can remove this later

// TODO(ebuchholz): Better constants
#define PI 3.14159265359f

union matrix3x3 {
    float m[9];
    // TODO(ebuchholz): see if we can add some other ways to access the elements
};

union matrix4x4 {
    float m[16];

    // TODO(ebuchholz): 2 dimensional array? struct with individual members? [] operator?
};

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


// TODO(ebuchholz): double check all of this quaternion stuff, it's ported from nfl thrower 
// typescript code
struct quaternion {
    float w;
    float x;
    float y;
    float z;
};

struct aabb {
    vector3 min;
    vector3 max;
};

struct sphere {
    vector3 pos;
    float radius;
};

struct plane {
    vector3 normal;
    float distance;
};

struct triangle {
    vector3 p0;
    vector3 p1;
    vector3 p2;
};

struct line {
    vector3 a;
    vector3 b;
};

#endif
