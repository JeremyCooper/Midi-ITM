//
// Created by Jeremy Cooper on 6/9/17.
//

#include "graphics/Scene.h"
#include "cinder/App/AppBase.h"
#include <cstdarg>
#include <cinder/Rand.h>
#include <cinder/Rect.h>

using namespace ci;


const int RADIUS = 75;
const int SEG_LENGTH = 120;
const int TRANSITION_LENGTH = 25;
const int FADEOUT_LENGTH = 40;
const int COLORS = 4;
const int MAX_OFFSET = 0;
const int TAIL_LENGTH = 12;
const float SLOW_FACTOR = 3.5f;
const ColorModel MODEL = CM_RGB;

//TODO: update perpVec after vector is altered due to collision on screen edge
//TODO: draw segments using opengl batches

ColorRange::ColorRange(int steps, std::vector<ColorA> palette) {
    for (auto it=palette.begin(); it!=palette.end(); ++it) {
        colors.push_back(*it);
    }
    iter = colors.begin() + rand() % colors.size();
}
ci::ColorA ColorRange::nextColor() {
    if (iter == colors.end()-1)
        iter = colors.begin();
    else
        ++iter;

    return *iter;
}
ci::ColorA ColorRange::currentColor() {
    return *iter;
}
ColorRange* ColorRange::copy() {
    return new ColorRange(0, colors);
}


vec2 findPerp(vec2 vec, int desired_length) {
    auto v_length = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    auto scale_factor = desired_length / v_length;
    return vec2(-scale_factor * vec.y, scale_factor * vec.x);
}
Segment::Segment(vec2 pos, vec2 vec, ColorA lColor, ColorA color) {
    mPos = pos;
    initialPos = pos;
    mVec = vec;
    lastColor = lColor;
    mColor = color;
    mTimer = 0;
    perpVec = findPerp(mVec, RADIUS);
    fadeout = false;

    points.push_back(vec2(initialPos + perpVec));
    points.push_back(vec2(mPos + perpVec));
    points.push_back(vec2(mPos - perpVec));
    points.push_back(vec2(initialPos - perpVec));
    //perpVec2 = findPerp(-mVec, RADIUS);
    //perpVec1.x = -mVec.y * 4.0f * RADIUS;
    //perpVec1.y = mVec.x * 4.0f * RADIUS;
}
void Segment::update(vec2 newPos) {
    if (mTimer < SEG_LENGTH) {
        mTimer++;
        if (!fadeout)
            mPos = newPos;
        points[1] = mPos + perpVec;
        points[2] = mPos - perpVec;
    }
}
void Segment::draw() {
    if (fadeout) {
        gl::color(lerp(mColor,
                       ColorA(CM_RGB, mColor.r, mColor.g, mColor.b, 0.0f),
                       clamp(static_cast<float>(mTimer) / FADEOUT_LENGTH, 0.0f, 1.0f)));
    } else {
        if (mTimer < TRANSITION_LENGTH)
            gl::color(lerp(lastColor,
                           mColor,
                           clamp(static_cast<float>(mTimer) / TRANSITION_LENGTH, 0.0f, 1.0f)));
        else
            gl::color(mColor);
    }

    gl::drawSolidCircle(initialPos, RADIUS);
    gl::drawSolid(points);
    gl::drawSolidCircle(mPos, RADIUS);
}
void Segment::begin_fadeout() {
    fadeout = true;
    mTimer = 0;
}


struct rand_angle_90 {
    short x, y;
    rand_angle_90() {
        switch(rand() % 4) {
            case 0:
                x = 1;
                y = 1;
                break;
            case 1:
                x = 1;
                y = -1;
                break;
            case 2:
                x = -1;
                y = -1;
                break;
            case 3:
                x = -1;
                y = 1;
                break;
            default:
                x = 999;
                y = 999;
        }
    }
};
Strand::Strand(vec2 pos, ColorRange* colors)
        :mColors(colors) {
    mPos = pos;
    rand_angle_90* vec = new rand_angle_90();
    mVec = vec2(vec->x / SLOW_FACTOR, vec->y / SLOW_FACTOR);
    vSegment.push_back(new Segment(mPos, mVec, colors->currentColor(), colors->currentColor()));
    mTimer = clamp(rand() % SEG_LENGTH, 0, MAX_OFFSET);
    removeSegment = false;
}
void Strand::update() {
    mTimer++;
    // Advance strand, collision detection
    if (mPos.x + RADIUS >= WIDTH || mPos.x - RADIUS <= 0) {
        mVec.x = -mVec.x;
    }
    if (mPos.y + RADIUS >= HEIGHT || mPos.y - RADIUS <= 0) {
        mVec.y = -mVec.y;
    }
    mPos += mVec;
    // update segments
    for (auto it=vSegment.begin(); it!=vSegment.end(); ++it) {
        (*it)->update(mPos);
    }
    if (mTimer > SEG_LENGTH) {
        mTimer = 0;
        vSegment.push_back(new Segment(mPos, mVec, mColors->currentColor(), mColors->nextColor()));
        if (vSegment.size() > TAIL_LENGTH) {
            removeSegment = true;
            vSegment[0]->begin_fadeout();
        }
    }
    if (removeSegment) {
        if (mTimer > FADEOUT_LENGTH) {
            vSegment.erase(vSegment.begin());
            removeSegment = false;
        }
    }
}
void Strand::draw() {
    for (auto it=vSegment.begin(); it!=vSegment.end(); ++it) {
        (*it)->draw();
    }
}


Strands::Strands(int num, ColorRange colors) {
    for (int i=0; i!=num; ++i) {
        Rand::randSeed(static_cast<u_int32_t>(rand()));
        vStrand.push_back(
                Strand(vec2(Rand::randInt(0+RADIUS+1, WIDTH-RADIUS-1),
                            Rand::randInt(0+RADIUS+1, HEIGHT-RADIUS-1)),
                       colors.copy()));
    }
}
void Strands::update() {
    for (auto it=vStrand.begin(); it!=vStrand.end(); ++it) {
        it->update();
    }
}
void Strands::draw() {
    for (auto it=vStrand.begin(); it!=vStrand.end(); ++it) {
        it->draw();
    }
}


std::vector<ColorA> gen_palette() {
    std::vector<ColorA> palette;
    for (int i=0; i!=COLORS; ++i) {
        palette.push_back(ColorA(MODEL, Rand::randFloat(), Rand::randFloat(), Rand::randFloat(), 1.0f));
    }
    return palette;
}
ThrowbackScene::ThrowbackScene() {
    //palette.push_back(Color(MODEL, 0.4f, 0.8f, 1.0f));
    //palette.push_back(Color(MODEL, 0.8f, 1.0f, 1.0f));

    mStrands = new Strands(6, ColorRange(0, gen_palette()));
}
void ThrowbackScene::update() {
    mStrands->update();
}
void ThrowbackScene::draw() {
    mFbo->bindFramebuffer();
    gl::setMatricesWindow( app::getWindowSize() );
    gl::clear(Color(0, 0, 0));
    mStrands->draw();
    mFbo->unbindFramebuffer();
}
void ThrowbackScene::event(int e) {
    switch (e) {
        case 0:
            break;
        //test event
        case 123:
            mStrands = new Strands(6, ColorRange(0, gen_palette()));
            break;
        default:
            break;
    }
}