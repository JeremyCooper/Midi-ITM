//
// Created by Jeremy Cooper on 6/1/17.
//

#ifndef MIDI_ITM_GRAPHICS_H
#define MIDI_ITM_GRAPHICS_H

//#include "graphics/GFX_Api.h"
#include "graphics/Layer.h"
#include <vector>


class Graphics {
private:
    std::vector<Layer> mLayers;
    //syphonClient mClientSyphon;
    syphonServer mServer;
public:
    Graphics();
    //~Graphics();
    void setup();
    void update();
    void draw();
    void publish();
};


#endif //MIDI_ITM_GRAPHICS_H
