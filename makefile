engine3d: main.c
	gcc -o engine3d main.c updateFrame.c matrixMath.c -lallegro -lallegro_main -lallegro_primitives -I.