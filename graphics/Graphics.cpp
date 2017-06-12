//
// Created by Jeremy Cooper on 6/1/17.
//

#include "Graphics.h"
using namespace ci;


Graphics::Graphics() {
    srand (static_cast<unsigned int>(time(NULL)));
    for (int i=0; i!=2; ++i) {
        //Add layer to vector and set scene
        mLayers.push_back(Layer(i));
        mLayers[i].changeScene(i);
        vScenePtr.push_back(mLayers[i].get());
    }
    mLayers[0].changeScene(4);
}
void Graphics::setup() {
    for (auto it = mLayers.begin(); it!=mLayers.end(); ++it) {
        it->setup();
    }
}
void Graphics::updateScenePointer(int i) {
    vScenePtr[i] = mLayers[i].get();
}
void Graphics::update() {
    for (auto it = mLayers.begin(); it!=mLayers.end(); ++it) {
        it->update();
    }
}
void Graphics::draw() {
    for (auto it = mLayers.begin(); it!=mLayers.end(); ++it) {
        it->draw();
    }
}
void Graphics::publish() {
    for (auto it = mLayers.begin(); it!=mLayers.end(); ++it) {
        it->publish();
    }
}
void Graphics::sendEvent(int i, int e) {
    mLayers[i].get()->event(e);
    //vScenePtr[i]->event(e);
}