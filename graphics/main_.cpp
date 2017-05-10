const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#include "layer_class.h"

const int NUM_LAYERS = 10;
const int MAX_CONTROLS = 10;
const int MAX_PARAMS = 10;
//layer, control, param, value
float control_states[NUM_LAYERS][MAX_CONTROLS][MAX_PARAMS]; //read only
bool active_layers[NUM_LAYERS] = {false};
bool update_layer[NUM_LAYERS] = {true};
int i;
void mainloop(){
	//initialize layers
	Layer* layers[NUM_LAYERS];
	for (i=0; i!=NUM_LAYERS; ++i){
		layers[i] = new Layer(
				&control_states[i]);
	}
	active_layers[0] = true;
	while (!done){
		/*
		 * Update and internally draw
		 */
		for (i=0; i!=NUM_LAYERS; ++i){
			if (active_layers[i]){
				layers[i]->update();
				layers[i]->draw();
			}
		}
		/*
		 * Draw layers to screen
		 */
		for (i=0; i!=NUM_LAYERS; ++i){
			if (active_layers[i]){
				layers[i]->render();
			}
		}
		/*
		 * For preventing re-renders of data that
		 * has not changed
		 */
		for (i=0; i!=NUM_LAYERS; ++i)
			update_layer[i] = false;
	}
}
