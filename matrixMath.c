#include "matrixMath.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


//VECTORS

void scalarVec(float a, vec3d *b){
    b -> x *= a;
    b -> y *= a;
    b -> z *= a;
}

void addVec(vec3d *a, vec3d *b){
    a -> x += b -> x;
    a -> y += b -> y;
    a -> z += b -> z;
}

vec3d addNewVec(vec3d *a, vec3d *b){
    vec3d c;
    c.x = a -> x + b -> x;
    c.y = a -> y + b -> y;
    c.z = a -> z + b -> z;
    return c;
}

vec3d newVec(vec3d *a){
    vec3d c;
    c.x = a -> x;
    c.y = a -> y;
    c.z = a -> z;
    return c;
}

void crossVec(vec3d *a, vec3d *b){
    float *i = malloc(sizeof(float));
    float *j = malloc(sizeof(float));
    float *k = malloc(sizeof(float));

    *i = (a->y * b->z) - (a->z * b->y);
    *j = (a->z * b->x) - (a->x * b->z);
    *k = (a->x * b->y) - (a->y * b->x);

    a -> x = *i;
    a -> y = *j;
    a -> z = *k;

    free(i);
    free(j);
    free(k);
}

void normalizeVec(vec3d *a){
    scalarVec(1/lengthVec(a),a);
}

float dotVec(vec3d *a, vec3d *b){
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

float lengthVec(vec3d *a){
    return sqrtf(powf(a->x,2)+powf(a->y,2)+powf(a->z,2));
}

vec3d *NewCrossVec(vec3d *a, vec3d *b){
    vec3d *c = malloc(sizeof(vec3d));

    c->x = (a->y * b->z) - (a->z * b->y);
    c->y = (a->z * b->x) - (a->x * b->z);
    c->z = (a->x * b->y) - (a->y * b->x);

    return c;
}

//MATRICES

void addMat(mat4x4 *a, mat4x4 *b){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            a->m[i][j] += b->m[i][j];
        } 
    }
}

void addMat41(mat1x4 *a, mat1x4 *b){
    for(int j = 0; j < 4; j++){
        a->m[j] += b->m[j];
    } 
}


void scalarMat44(float a, mat4x4 *b){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            b->m[i][j] *= a;
        } 
    }
}

void scalarMat41(float a, mat1x4 *b){
    for(int i = 0; i < 4; i++){
        b->m[i] *= a;
    } 
}

void crossMat41(mat1x4 *a, mat4x4 *b){
    //generating temporary matrix
    mat1x4 *c = malloc(sizeof(mat1x4));

    //for every column of b
    for(int i = 0; i < 4; i++){
        //temporary variable holding addition
        float k = 0;
        //for every row in i'th column in b multiply by element in a and add to k
        for(int j = 0; j < 4; j++){
            k += a->m[j] * b->m[j][i];
        }
        //write temporary variable to temporary matrix
        c->m[i] = k;
    }

    //rewrite data from temporary matrix
    for(int i = 0; i < 4; i++){
        a->m[i] = c->m[i];
    }

    //clean memory
    free(c);
}

void crossMat44(mat4x4 *a, mat4x4 *b){
    mat4x4 *c = malloc(sizeof(mat4x4));

    for(int l = 0; l < 4; l++){
        for(int i = 0; i < 4; i++){
            float k = 0;
            for(int j = 0; j < 4; j++){
                k += a->m[l][j] * b->m[j][i];
            }
            c->m[l][i] = k;
        }
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            a->m[i][j] = c->m[i][j];
        }
    }

    free(c);
}

mat1x4 *newCrossMat41(mat1x4 *a, mat4x4 *b){
    mat1x4 *c = malloc(sizeof(mat1x4));

    for(int i = 0; i < 4; i++){
        float k = 0;
        for(int j = 0; j < 4; j++){
            k += a->m[j] * b->m[j][i];
        }
        c->m[i] = k;
    }

    return c;
}

mat4x4 getRotMatX(float angle){
    mat4x4 rotMat = {{
     {1,    0,              0,              0},
     {0,    cosf(angle),    -sinf(angle),   0},
     {0,    sinf(angle),    cosf(angle),    0},
     {0,    0,              0,              0}
    }};

    return rotMat;
}

mat4x4 getRotMatY(float angle){
    mat4x4 rotMat = {{
     {cosf(angle),  0,  sinf(angle),    0},
     {0,            1,  0,              0},
     {-sinf(angle), 0,  cosf(angle),    0},
     {0,            0,  0,              0}
    }};
    return rotMat;
}

mat4x4 getRotMatZ(float angle){
    mat4x4 rotMat = {{
     {cosf(angle),    -sinf(angle),     0,  0},
     {sinf(angle),    cosf(angle),      0,  0},
     {0,                        0,      1,  0},
     {0,                        0,      0,  0}
    }};
    return rotMat;
}
