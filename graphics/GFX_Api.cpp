//
// Created by Jeremy Cooper on 6/1/17.
//

#include "GFX_Api.h"

static int GFX_Api::communicate(int layer, int control, int param, int value) {
	layer = fix(layer);
	control = fix(control);
	param = fix(param);
	value = fix(value);
	control_states[layer][control][param] = value / 127.0;
	std::cout << value / 127.0 << std::endl;
	return 0;
}