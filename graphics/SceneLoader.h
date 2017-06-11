//
// Created by Jeremy Cooper on 6/1/17.
//

#ifndef MIDI_ITM_SCENELOADER_H
#define MIDI_ITM_SCENELOADER_H

#include "Scene.h"


class SceneLoader {
private:
    Scene *mScene;
    bool sceneLoaded;
public:
    SceneLoader() :sceneLoaded(false) {}
    template <typename T> Scene* loadScene() {
        try {
            if (sceneLoaded)
                unLoadScene();
            sceneLoaded = true;
            return mScene = new T;
        } catch (...) {
            std::cout << "Scene failed to load" << std::endl;
        }
        return nullptr;
    }
    void unLoadScene() { delete mScene; }
    Scene* returnScene() {
        if (sceneLoaded) {
            return mScene;
        } else {
            return nullptr;
        }
    }
};


#endif //MIDI_ITM_SCENELOADER_H