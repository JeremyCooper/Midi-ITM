//
// Created by Jeremy Cooper on 6/1/17.
//

#ifndef MIDI_ITM_SCENELOADER_H
#define MIDI_ITM_SCENELOADER_H

#include "Scene.h"


class SceneLoader {
private:
    Scene *mScene;
public:
    void loadScene(std::string sceneName);
    void unLoadScene();
};


#endif //MIDI_ITM_SCENELOADER_H
