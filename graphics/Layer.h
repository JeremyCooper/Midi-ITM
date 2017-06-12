//
// Created by Jeremy Cooper on 6/1/17.
//

#ifndef MIDI_ITM_LAYER_H
#define MIDI_ITM_LAYER_H

#include "cinderSyphon.h"
#include "SceneLoader.h"


class Layer {
private:
    int id;
    syphonServer mServer;
    SceneLoader mSceneLoader;
    ci::gl::FboRef mFbo;
    Scene* pScene;
public:
    Layer(int id);
    ~Layer();
    void setup();
    void update();
    void draw();
    void publish();
    Scene* get();
    void changeScene(int);
};


#endif //MIDI_ITM_LAYER_H
