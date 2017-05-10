const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#include "sdl_utils.h"
#include "layer_class.h"

const int NUM_LAYERS = 10;
const int MAX_CONTROLS = 10;
const int MAX_PARAMS = 10;
//layer, control, param, value
float control_states[NUM_LAYERS][MAX_CONTROLS][MAX_PARAMS]; //read only
bool active_layers[NUM_LAYERS] = {false};
bool update_layer[NUM_LAYERS] = {true};
int i;
void sdl_mainloop(SDL_Renderer *ren){
	//initialize layers
	Layer* layers[NUM_LAYERS];
	for (i=0; i!=NUM_LAYERS; ++i){
		layers[i] = new Layer(
				SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT),
				ren,
				&control_states[i]);
	}
	active_layers[0] = true;
	while (!done){
		for (i=0; i!=NUM_LAYERS; ++i){
			if (active_layers[i]){
				layers[i]->update();
				layers[i]->draw();
			}
		}
		SDL_SetRenderTarget(ren, NULL);
		for (i=0; i!=NUM_LAYERS; ++i){
			if (active_layers[i]){
				layers[i]->render();
			}
		}
		SDL_RenderPresent(ren);
		for (i=0; i!=NUM_LAYERS; ++i)
			update_layer[i] = false;
	}
}
