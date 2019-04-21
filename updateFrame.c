#include <stdlib.h>
#include <stdbool.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include <math.h>

#include "updateFrame.h"
#include "main.h"

//making array of vectors to be translated
vec3d *tempVecs;
//array of polygons to be displayed
bool *displayedPol;
//array of lighting rays
float *lightArrs;


//translating vector
vec3d transVec;

//angles for rotating
float objAngl[3] = {0,0,0};

//projection matrix
mat4x4 projMat = {
	{
		{AR*TANF2,  0,      0,          0},
    	{0,         TANF2,  0,          0},
    	{0,         0,      Q,          1},
    	{0,         0,      -ZNEAR*Q,   0}
	}
};

int init(){
    //allocating memory for arrays 
    tempVecs = malloc(sizeof(vec3d) * model.numOfVectors);
    displayedPol = malloc(sizeof(bool) * model.numOfFaces);
    lightArrs = malloc(sizeof(float) * model.numOfFaces);

    transVec.x = 0; transVec.y = 0; transVec.z = 2;

    fprintf(stderr,"INIT COMPLETE\n");

    return 0;
}

void updateFrame(bool keys[]){

    //key checks
    checkKeys(keys);

	//reset the background
	al_clear_to_color(al_map_rgb(0, 0, 0));

    //genereting tempVecs
    for(int i = 0; i < model.numOfVectors; i++){
        tempVecs[i] = newVec(&model.vectors[i]);
    }

    //rotating using rotMatrices
    for(int i = 0; i < model.numOfVectors; i++){
        mat1x4 tempMat;
        tempMat.m[0] = tempVecs[i].x;
        tempMat.m[1] = tempVecs[i].y;
        tempMat.m[2] = tempVecs[i].z;
        tempMat.m[3] = 0;

        mat4x4 rotMatX = getRotMatX(objAngl[0]);
        crossMat41(&tempMat,&rotMatX);

        mat4x4 rotMatY = getRotMatY(objAngl[1]);
        crossMat41(&tempMat,&rotMatY);

        mat4x4 rotMatZ = getRotMatZ(objAngl[2]);
        crossMat41(&tempMat,&rotMatZ);

        tempVecs[i].x = tempMat.m[0];
        tempVecs[i].y = tempMat.m[1];
        tempVecs[i].z = tempMat.m[2];
    }

    //translate object by a vector
    for(int i = 0; i < model.numOfVectors; i++){
        addVec(&tempVecs[i],&transVec);
    }

    //sorting polygons this helps with displaying later
    qsort(model.triangles,model.numOfFaces,sizeof(triangle),cmpfunc);

    //checking if polygon is behind another
    //unfortunetly this algorithm will still display some hidden faces
    for(int i = 0; i < model.numOfFaces; i++){

        //rewriting data to temporary vairables
        float x1,x2,x3,y1,y2,y3,z1,z2,z3;

        x1 = tempVecs[model.triangles[i].p[0]-1].x;
        x2 = tempVecs[model.triangles[i].p[1]-1].x;
        x3 = tempVecs[model.triangles[i].p[2]-1].x;

        y1 = tempVecs[model.triangles[i].p[0]-1].y;
        y2 = tempVecs[model.triangles[i].p[1]-1].y;
        y3 = tempVecs[model.triangles[i].p[2]-1].y;

        z1 = tempVecs[model.triangles[i].p[0]-1].z;
        z2 = tempVecs[model.triangles[i].p[1]-1].z;
        z3 = tempVecs[model.triangles[i].p[2]-1].z;

        //making three vectors two specifing triangle and one from camera to the triangle
        vec3d v1 = {x1-x2,y1-y2,z1-z2};
        vec3d v2 = {x3-x2,y3-y2,z3-z2};
        vec3d v3 = {x2,y2,z2};

        //getting normal vector
        crossVec(&v1,&v2);
        normalizeVec(&v1);

        //normalizing vector pointing from camera to triangle
        normalizeVec(&v3);

        //calculating dot product
        float dotV = dotVec(&v1,&v3);

        //if dot product is negative given triangle is not needed to be displayed
        if( dotV < 0){
            displayedPol[i] = false;
            lightArrs[i] = 0;
        }
        else{
            displayedPol[i] = true;
            lightArrs[i] = dotV * 255; //making lighting of the triangle based on the dot product
        }

    }

    
    //multiplying by projection matrix
    for(int i = 0; i < model.numOfVectors; i++){
        mat1x4 tempMat;
        tempMat.m[0] = tempVecs[i].x;
        tempMat.m[1] = tempVecs[i].y;
        tempMat.m[2] = tempVecs[i].z;
        tempMat.m[3] = 1;

        if(tempMat.m[2] <= ZNEAR) continue;

        crossMat41(&tempMat,&projMat);

        scalarMat41(1/tempMat.m[3],&tempMat);

        tempVecs[i].x = tempMat.m[0];
        tempVecs[i].y = tempMat.m[1];
        tempVecs[i].z = tempMat.m[2];
    }
    

    //drawing on screen polygons
    for(int i = 0; i < model.numOfFaces; i++){
        
        if(!displayedPol[i]) continue;

        float x1,x2,x3,y1,y2,y3;        

        //if triangle is too close it is ignored
        if(tempVecs[model.triangles[i].p[0]-1].z <= ZNEAR) continue;
        if(tempVecs[model.triangles[i].p[1]-1].z <= ZNEAR) continue;
        if(tempVecs[model.triangles[i].p[2]-1].z <= ZNEAR) continue;

        x1 =  HEIGHT/2 * tempVecs[model.triangles[i].p[0]-1].x + WIDTH/2;
        x2 =  HEIGHT/2 * tempVecs[model.triangles[i].p[1]-1].x + WIDTH/2;
        x3 =  HEIGHT/2 * tempVecs[model.triangles[i].p[2]-1].x + WIDTH/2;

        y1 =  HEIGHT/2 * tempVecs[model.triangles[i].p[0]-1].y + HEIGHT/2;
        y2 =  HEIGHT/2 * tempVecs[model.triangles[i].p[1]-1].y + HEIGHT/2;
        y3 =  HEIGHT/2 * tempVecs[model.triangles[i].p[2]-1].y + HEIGHT/2;

        al_draw_filled_triangle(x1,y1,x2,y2,x3,y3, al_map_rgb(lightArrs[i], lightArrs[i], lightArrs[i]));
    }    

    //update screen
	al_flip_display();

}

void checkKeys(bool keys[]){
    if(keys[KEY_LEFT]) transVec.x -= 0.1;
    if(keys[KEY_RIGHT]) transVec.x += 0.1;
    if(keys[KEY_UP]) transVec.y -= 0.1;
    if(keys[KEY_DOWN]) transVec.y += 0.1;
    if(keys[KEY_LCTRL]) transVec.z -= 0.1;
    if(keys[KEY_LSHIFT]) transVec.z += 0.1;
    
    if(keys[KEY_Q]) objAngl[2] -= 0.1;
    if(keys[KEY_E]) objAngl[2] += 0.1;
    if(keys[KEY_A]) objAngl[1] -= 0.1;
    if(keys[KEY_D]) objAngl[1] += 0.1;
    if(keys[KEY_W]) objAngl[0] -= 0.1;
    if(keys[KEY_S]) objAngl[0] += 0.1;
}

//this function is comparing average z position of two triangles
//needed for qsort function
int cmpfunc (const void * a, const void * b){
    float zA = 0;
    float zB = 0;
    triangle *triangleA = (triangle *)a;
    triangle *triangleB = (triangle *)b;

    zA += tempVecs[triangleA->p[0]].z;
    zA += tempVecs[triangleA->p[1]].z;
    zA += tempVecs[triangleA->p[2]].z;
    zA /= 3;

    zB += tempVecs[triangleB->p[0]].z;
    zB += tempVecs[triangleB->p[1]].z;
    zB += tempVecs[triangleB->p[2]].z;
    zB /= 3;

    if(zB > zA) return 1;
    else return -1;
}