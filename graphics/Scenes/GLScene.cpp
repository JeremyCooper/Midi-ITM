//
// Created by Jeremy Cooper on 6/9/17.
//

#include "graphics/Scene.h"
#include "cinder/app/AppBase.h"
using namespace ci;


GLScene::GLScene() {
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef shader = static_cast<std::shared_ptr<ci::gl::GlslProg> &&>(gl::getStockShader(lambert));
    mBox = gl::Batch::create(geom::Cube(), shader);

    mCam.lookAt(vec3(3, 4.5, 4.5), vec3(0, 1, 0));
}
void GLScene::update() {

}
void GLScene::draw() {
    mFbo->bindFramebuffer();

    gl::clear();
    gl::enableDepthRead();
    gl::enableDepthWrite();

    gl::setMatrices(mCam);

    int numBoxes = 64;
    float maxAngle = static_cast<float>(M_PI * 7);
    float spiralRadius = 1;
    float height = 2;
    float boxSize = 0.05f;
    float anim = app::getElapsedFrames() / 30.0f;

    for (int s=0; s!=numBoxes; ++s) {
        float rel = s / static_cast<float>(numBoxes);
        float angle = rel * maxAngle;
        float y = static_cast<float>(fabs(cos(rel * M_PI + anim)) * height);
        float r = rel * spiralRadius;
        vec3 offset(r * cos(angle), y / 2, r * sin(angle));

        gl::ScopedModelMatrix scpModelMtx;
        //gl::pushModelMatrix();
        gl::translate(offset);
        gl::scale(vec3(boxSize, y, boxSize));
        gl::color(Color(CM_HSV, rel, 1, 1));
        mBox->draw();
        //gl::popModelMatrix();
    }
    mFbo->unbindFramebuffer();
}