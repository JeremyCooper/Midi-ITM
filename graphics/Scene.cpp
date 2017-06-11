//
// Created by Jeremy Cooper on 6/1/17.
//

#include "Scene.h"
using namespace ci;


void NerdScene::draw() {
    mFbo->bindFramebuffer();
    gl::color(1, 0, 0);
    gl::drawSolidCircle(vec2(100, 100), 400);
    mFbo->unbindFramebuffer();
}