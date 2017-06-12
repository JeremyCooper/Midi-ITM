//
// Created by Jeremy Cooper on 6/1/17.
//

#ifndef MIDI_ITM_SCENE_H
#define MIDI_ITM_SCENE_H

#include <iostream>
#include "cinder/gl/Fbo.h"
#include "cinder/gl/gl.h"
#include "Config.h"


class Scene {
protected:
    ci::gl::FboRef mFbo;
    //Controller* controller;
public:
    Scene() {
        mFbo = ci::gl::Fbo::create(WIDTH, HEIGHT, ci::gl::Fbo::Format().colorTexture().samples(8));
    }
    virtual ~Scene() { std::cout << "Base DEstructor" << std::endl; }
    virtual void setup() {}
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void event(int e) { std::cout << "Event " << e << "consumed" << std::endl; }
    ci::gl::FboRef publish() { return mFbo; }
};

#include "graphics/Helpers/FlowerScene.h"
class FlowerScene : public Scene {
private:
    Flower* flower;
public:
    FlowerScene();
    ~FlowerScene() {}
    void update();
    void draw();
};
#include "graphics/Helpers/WindowScene.h"
class WindowScene : public Scene {
private:
    Windows* windows;
public:
    WindowScene();
    ~WindowScene() {}
    void update();
    void draw();
};
#include "graphics/Helpers/GLScene.h"
class GLScene : public Scene {
private:
    ci::CameraPersp mCam;
    ci::gl::BatchRef mBox;
public:
    GLScene();
    ~GLScene() {}
    void update();
    void draw();
};
#include "graphics/Helpers/ThrowbackScene.h"
class ThrowbackScene : public Scene {
private:
    Strands* mStrands;
    std::vector<ci::ColorA> palette;
public:
    ThrowbackScene();
    ~ThrowbackScene() {}
    void update();
    void draw();
    void event(int e);
};
class NerdScene : public Scene {
public:
    NerdScene() {}
    ~NerdScene() {}
    void update() { }
    void draw();
};


#endif //MIDI_ITM_SCENE_H
