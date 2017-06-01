//
// Created by Jeremy Cooper on 6/1/17.
//

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
void gfx_setup() {
    //initialize layers
    Layer *layers[NUM_LAYERS];
    for (i = 0; i != NUM_LAYERS; ++i) {
        layers[i] = new Layer(
                &control_states[i]);
    }
}
void gfx_update() {
    /*
     * Update layers
     */
    for (i = 0; i != NUM_LAYERS; ++i) {
        if (active_layers[i] && update_layer[i]) {
            layers[i]->update();
        }
    }
}
void gfx_draw() {
    /*
     * Have layers internally draw
     */
    for (i = 0; i != NUM_LAYERS; ++i) {
        if (active_layers[i] && update_layer[i]) {
            layers[i]->draw();
        }
        /*
         * For preventing re-renders of data that
         * has not changed
         */
        if (!layers[i]->isMotionLayer())
            update_layer[i] = false;
    }
}
void gfx_render() {
    /*
     * Draw layers to screen
     */
    for (i = 0; i != NUM_LAYERS; ++i) {
        if (active_layers[i]) {
            layers[i]->render();
        }
    }
}