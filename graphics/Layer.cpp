//
// Created by Jeremy Cooper on 6/1/17.
//

#include "Layer.h"
#include "Graphics.h"

using namespace ci;


Layer::Layer(int id)
        :id(id),
         mServer(syphonServer()),
         mFbo(gl::Fbo::create(WIDTH, HEIGHT, gl::Fbo::Format().colorTexture().samples(8))),
         pScene(nullptr) {

    std::cout << "Layer created. ID: " << id << std::endl;
    //mFbo = gl::Fbo::create(WIDTH, HEIGHT, gl::Fbo::Format().colorTexture().samples(8));
}
Layer::~Layer() {
    //delete mServer;
}
/*
 * Update
 */
void Layer::setup() {
    mServer.setName("Layer_" + std::to_string(id));
    if (pScene != nullptr)
        pScene->setup();
}
void Layer::update() {
    if (pScene != nullptr)
        pScene->update();
}
void Layer::draw() {
    if (pScene != nullptr)
        mSceneLoader.returnScene()->draw();
}
void Layer::publish() {
    if (pScene != nullptr)
        mServer.publishTexture(mSceneLoader.returnScene()->publish()->getColorTexture(), false);
}
Scene* Layer::get() {
    return mSceneLoader.returnScene();
}
void Layer::changeScene(int sceneID) {
    /*
     * TODO:
     * Going to need to call Graphics::updateScenePointer(int i)
     * when the scene is changed or events will go to nullptr
     */
    switch (sceneID) {
        case 0:
            pScene = mSceneLoader.loadScene<FlowerScene>();
            break;
        case 1:
            pScene = mSceneLoader.loadScene<NerdScene>();
            break;
        case 2:
            pScene = mSceneLoader.loadScene<WindowScene>();
            break;
        case 3:
            pScene = mSceneLoader.loadScene<GLScene>();
            break;
        case 4:
            pScene = mSceneLoader.loadScene<ThrowbackScene>();
            break;
        default:
            break;
    }
    if (pScene == nullptr)
        return;
    pScene->setup();
}