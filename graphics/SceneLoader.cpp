//
// Created by Jeremy Cooper on 6/1/17.
//

#include "SceneLoader.h"

void SceneLoader::loadScene(std::string sceneName) {
    mScene = Scene();
}
void SceneLoader::unLoadScene() {
    delete mScene;
}