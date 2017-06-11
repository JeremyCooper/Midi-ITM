//
// Created by Jeremy Cooper on 6/8/17.
//

#include "graphics/Scene.h"
#include "graphics/Helpers/FlowerScene.h"
#include "cinder/Rand.h"
//#include "Helpers/TriangleHelpers.h"
using namespace ci;


Petal::Petal(petal_dir_t dir, Color color, int placement)
        :dir(dir), color(color), fold_amount(0.0f), size(100), placement(placement), time(0.0f) {
    side = static_cast<petal_side_t>(Rand::randInt(0, 4));
    update_coords();
}
void Petal::update_coords() {
    switch (side) {
        case TOP:
            pt0 = vec2(WIDTH / 2, HEIGHT / 2 - size + fold_amount);
            pt1 = vec2(WIDTH / 2 - size, HEIGHT / 2 - size);
            pt2 = vec2(WIDTH / 2 + size, HEIGHT / 2 - size);
            break;
        case RIGHT:
            pt0 = vec2(WIDTH / 2 + size - fold_amount, HEIGHT / 2);
            pt1 = vec2(WIDTH / 2 + size, HEIGHT / 2 - size);
            pt2 = vec2(WIDTH / 2 + size, HEIGHT / 2 + size);
            break;
        case BOTTOM:
            pt0 = vec2(WIDTH / 2, HEIGHT / 2 + size - fold_amount);
            pt1 = vec2(WIDTH / 2 - size, HEIGHT / 2 + size);
            pt2 = vec2(WIDTH / 2 + size, HEIGHT / 2 + size);
            break;
        case LEFT:
            pt0 = vec2(WIDTH / 2 - size + fold_amount, HEIGHT / 2);
            pt1 = vec2(WIDTH / 2 - size, HEIGHT / 2 - size);
            pt2 = vec2(WIDTH / 2 - size, HEIGHT / 2 + size);
            break;
    }
}
void Petal::update() {
    time += 0.01f;
    switch (dir) {
        case IN:
            fold_amount += sinf(time) - 0.1f;
            break;
        case OUT:
            fold_amount += sinf(time) - 0.1f;
            break;
    }
    update_coords();
}
void Petal::draw() {
    gl::color(color);
    gl::drawSolidTriangle(pt0, pt1, pt2);
}
Flower::Flower(int starting_petals) {
    for (int i=0; i!=starting_petals; ++i) {
        petals.push_back(Petal(IN, Color(0.5f, 0, 1), i));
    }
}
void Flower::update() {
    for (auto it = petals.begin(); it!=petals.end(); ++it) {
        it->update();
    }
}
void Flower::draw() {
    for (auto it = petals.begin(); it!=petals.end(); ++it) {
        it->draw();
    }
}

FlowerScene::FlowerScene() {
    flower = new Flower(4);
}
void FlowerScene::update() {
    flower->update();
}
void FlowerScene::draw() {
    mFbo->bindFramebuffer();
    gl::clear(Color(0, 0, 0));
    flower->draw();
    mFbo->unbindFramebuffer();
}