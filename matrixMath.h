#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct{
    float x,y,z;
}vec3d;

typedef struct{
    float m[4][4];
}mat4x4;

typedef struct{
    float m[4];
}mat1x4;

typedef struct{
    int p[3];
}triangle;

//functions always edit first vector/matrix

void scalarVec(float a, vec3d *b);
void addVec(vec3d *a, vec3d *b);
void crossVec(vec3d *a, vec3d *b);
void normalizeVec(vec3d *a);


void addMat44(mat4x4 *a, mat4x4 *b);
void addMat41(mat1x4 *a, mat1x4 *b);
void scalarMat44(float a, mat4x4 *b);
void scalarMat41(float a, mat1x4 *b);
void crossMat41(mat1x4 *a, mat4x4 *b);
void crossMat44(mat4x4 *a, mat4x4 *b);

//returning functions
float dotVec(vec3d *a, vec3d *b);
float lengthVec(vec3d *a);
mat1x4 *newCrossMat41(mat1x4 *a, mat4x4 *b);
vec3d *NewCrossVec(vec3d *a, vec3d *b);
vec3d addNewVec(vec3d *a, vec3d *b);
vec3d newVec(vec3d *a);

//rotational matrices
mat4x4 getRotMatX(float angle);
mat4x4 getRotMatY(float angle);
mat4x4 getRotMatZ(float angle);