//
// Created by Jeremy Cooper on 6/9/17.
//

#include "graphics/Scene.h"
#include "cinder/App/AppBase.h"
//#include "cinder/GeomIo.h"
using namespace ci;

Windows::Windows(int num)
        :num(num) {
    margins = new Margin(0, 0, 0, 0);
    //int max_size = WIDTH/num;
    for (int i=0; i!=num; ++i) {
        windows.push_back(
                Window(i * 50, 0, 30, HEIGHT, 0, 1)
        );
    }
}
void Windows::shift_right(int amount) {
    for (auto it=windows.begin(); it!=windows.end(); ++it) {
        if (it->move_right(amount) >= WIDTH) {
            it->pop_left();
        }
    }
}
void Windows::shift_left(int amount) {
    for (auto it=windows.begin(); it!=windows.end(); ++it) {
        if (it->move_left(amount) <= 0 + it->getWidth()) {
            it->pop_right();
        }
    }
}
void Windows::draw() {
    for (auto it=windows.begin(); it!=windows.end(); ++it) {
        it->draw();
    }
}
void Window::draw() {
    gl::color(1, 1, 1);
    gl::drawSolidRect(Rectf(vec2(x, top_y), vec2(x+width, bot_y)));
}
WindowScene::WindowScene() {
    windows = new Windows(10);
    //enum controls {};
    //controller = new Controller(2, 10);
}
void WindowScene::update() {
    windows->shift_right(1);
}
void WindowScene::draw() {
    mFbo->bindFramebuffer();
    gl::clear(Color(0, 0, 0));
    gl::setMatricesWindow( app::getWindowSize() );
    windows->draw();
    mFbo->unbindFramebuffer();
}