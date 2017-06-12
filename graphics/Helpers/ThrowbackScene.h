//
// Created by Jeremy Cooper on 6/9/17.
//

#ifndef MIDI_ITM_THROWBACKSCENE_H
#define MIDI_ITM_THROWBACKSCENE_H


class ColorRange {
private:
    std::vector<ci::ColorA> colors;
    std::vector<ci::ColorA>::iterator iter;
public:
    ColorRange(int steps, std::vector<ci::ColorA> palette);
    ci::ColorA nextColor();
    ci::ColorA currentColor();
    ColorRange* copy();
};
class Segment {
private:
    ci::ColorA mColor, lastColor;
    ci::vec2 mPos, initialPos, mVec, perpVec;
    std::vector<ci::vec2> points;
    int mTimer;
    bool fadeout;
public:
    Segment(ci::vec2 pos, ci::vec2 vec, ci::ColorA lastColor, ci::ColorA color);
    void update(ci::vec2 newPos);
    void draw();
    void begin_fadeout();
};
class Strand {
private:
    std::vector<Segment*> vSegment;
    ci::vec2 mPos, mVec;
    ColorRange* mColors;
    int mTimer;
    bool removeSegment;
public:
    Strand(ci::vec2 pos, ColorRange* color);
    void update();
    void draw();
};
class Strands {
private:
    std::vector<Strand> vStrand;
public:
    Strands(int num, ColorRange colors);
    void update();
    void draw();
};

#endif //MIDI_ITM_THROWBACKSCENE_H
