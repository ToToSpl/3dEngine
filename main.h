#include <stdlib.h>
#include <stdbool.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include "matrixMath.h"
#include "updateFrame.h"

#define FPS 30
#define WIDTH 1920
#define HEIGHT 1080

//parameters for profection matrix

#define TANF2 5 //tangent of field of view defided by 2 
#define AR 1 //aspect ratio
#define ZNEAR 0.1
#define ZFAR 1000
#define Q 1.0001


typedef struct{
    int numOfFaces;
    int numOfVectors;
    vec3d *vectors;
    triangle *triangles;
}object3d;

object3d model;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_W, KEY_S, KEY_A, KEY_D, KEY_Q, KEY_E, KEY_LSHIFT, KEY_LCTRL
};

int main();
object3d loadObject(FILE *fp);
int allegro_handler();