#include <stdlib.h>
#include <stdbool.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

#include "main.h"


int main(int argc, char **argv){

    FILE *fp;
    if (argc >= 2)
         fp = fopen(argv[1], "r");
    else fp = fopen("examples/cube.obj", "r");

	model = loadObject(fp);

	int quit = 0;
	quit = allegro_handler();
	return quit;
}

object3d loadObject(FILE *fp){
	char *line = NULL;
    size_t len = 0;
	int nread;

	int numOfVectors = 0;
	int numOfFaces = 0;

	//getting number of vectors and faces
	while((nread = getline(&line, &len, fp)) != -1){
		if(line[0] == 'v') numOfVectors++;
		else if(line[0] == 'f') numOfFaces++;
	}

	//allocating memory for faces and vecotrs
	vec3d *vectors;
	vectors = malloc(sizeof(vec3d)*numOfVectors);
	triangle *triangles;
	triangles = malloc(sizeof(triangle)*numOfFaces);

	//getting reading pointer on the beggining of the file
	fseek(fp,0,SEEK_SET);

	//putting vectors and vaces into arrays
	int vecIndex = 0;
	int facIndex = 0;
    while ((nread = getline(&line, &len, fp)) != -1) {
		if(line[0] == 'v'){
			sscanf(line,"%*s %f %f %f",&vectors[vecIndex].x,&vectors[vecIndex].y,&vectors[vecIndex].z);
			vecIndex++;
		}
		else if(line[0] == 'f'){
			sscanf(line,"%*s %i %i %i",&triangles[facIndex].p[0],&triangles[facIndex].p[1],&triangles[facIndex].p[2]);
			facIndex++;
		}
    }

	object3d tempModel;
	tempModel.numOfVectors = numOfVectors;
	tempModel.numOfFaces = numOfFaces;
	tempModel.vectors = vectors;
	tempModel.triangles = triangles;

	fprintf(stderr,"MODEL LOADED\nNUM OF VECTORS: %i\tNUM OF FACES: %i\n",numOfVectors,numOfFaces);
	return tempModel;
}

int allegro_handler(){
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool running = true;
    bool key[12] = {true, true, true, true, true, true, true, true, true, true, true, true};

	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	// Create the display
	display = al_create_display(WIDTH, HEIGHT);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.\n");
		return 1;
	}

	// Install keyboard
    if(!al_install_keyboard()){
        fprintf(stderr, "Failed to initialize keyboard.\n");
        return 1;
    }

	// Initialize 3d engine
	if(init()){
		fprintf(stderr, "Failed to initialize 3d engine.\n");
        return 1;
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

	// Start the timer
	al_start_timer(timer);

	//first update
	updateFrame(key);

	while(running){
		
		ALLEGRO_EVENT ev;
		ALLEGRO_TIMEOUT timeout;

		al_init_timeout(&timeout, 10);

		bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);

      	// Handle the events
		if (get_event) {
			switch (ev.type) {
				case ALLEGRO_EVENT_TIMER:
					updateFrame(key); //update screen on every new frame
					break;

				//key events
                case ALLEGRO_EVENT_KEY_UP:
                    switch (ev.keyboard.keycode) {
                        case ALLEGRO_KEY_UP:
                            key[KEY_UP] = true;
                            break;
                        case ALLEGRO_KEY_DOWN:
                            key[KEY_DOWN] = true;
                            break;
                        case ALLEGRO_KEY_LEFT:
                            key[KEY_LEFT] = true;
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            key[KEY_RIGHT] = true;
                            break;
						case ALLEGRO_KEY_Q:
							key[KEY_Q] = true;
							break;
						case ALLEGRO_KEY_E:
							key[KEY_E] = true;
							break;
						case ALLEGRO_KEY_W:
							key[KEY_W] = true;
							break;
						case ALLEGRO_KEY_S:
							key[KEY_S] = true;
							break;
						case ALLEGRO_KEY_A:
							key[KEY_A] = true;
							break;
						case ALLEGRO_KEY_D:
							key[KEY_D] = true;
							break;
						case ALLEGRO_KEY_LSHIFT:
							key[KEY_LSHIFT] = true;
							break;
						case ALLEGRO_KEY_LCTRL:
							key[KEY_LCTRL] = true;
							break;
                    }
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:
                    switch (ev.keyboard.keycode) {
                        case ALLEGRO_KEY_UP:
                            key[KEY_UP] = false;
                            break;
                        case ALLEGRO_KEY_DOWN:
                            key[KEY_DOWN] = false;
                            break;
                        case ALLEGRO_KEY_LEFT:
                            key[KEY_LEFT] = false;
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            key[KEY_RIGHT] = false;
                            break;
						case ALLEGRO_KEY_Q:
							key[KEY_Q] = false;
							break;
						case ALLEGRO_KEY_E:
							key[KEY_E] = false;
							break;
						case ALLEGRO_KEY_W:
							key[KEY_W] = false;
							break;
						case ALLEGRO_KEY_S:
							key[KEY_S] = false;
							break;
						case ALLEGRO_KEY_A:
							key[KEY_A] = false;
							break;
						case ALLEGRO_KEY_D:
							key[KEY_D] = false;
							break;
						case ALLEGRO_KEY_LSHIFT:
							key[KEY_LSHIFT] = false;
							break;
						case ALLEGRO_KEY_LCTRL:
							key[KEY_LCTRL] = false;
							break;
                    }
                    break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				default:
					break;
			}
		}
	}

	// Clean up after program has finished
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}