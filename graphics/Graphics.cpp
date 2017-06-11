//
// Created by Jeremy Cooper on 6/1/17.
//

#include "Graphics.h"
using namespace ci;


Graphics::Graphics() {
    for (int i=0; i!=1; ++i) {
        mLayers.push_back(Layer(i));
        mLayers[i].changeScene(i);
    }
    mLayers[0].changeScene(3);
}
void Graphics::setup() {
    for (auto it = mLayers.begin(); it!=mLayers.end(); ++it) {
        it->setup();
    }
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