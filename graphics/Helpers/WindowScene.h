//
// Created by Jeremy Cooper on 6/9/17.
//

#ifndef MIDI_ITM_WINDOWSCENE_H
#define MIDI_ITM_WINDOWSCENE_H


class Window {
private:
    int x, top_y, bot_y;
    int width, height;
    int rotation;
    float opacity;
public:
    Window(int x, int y, int width, int height, int rotation, float opacity)
            : x(x), top_y(0), bot_y(HEIGHT), width(width), height(height), rotation(rotation), opacity(opacity) {}
    int move_right(int amount) { return x += amount; }
    int move_left(int amount) { return x -= amount; }
    void pop_right() { x = WIDTH; }
    void pop_left() { x = 0 - width; }
    int getWidth() { return width; }

    void draw();
};
struct Margin {
    int top, left, right, bottom;
    Margin(int t, int l, int r, int b)
            :top(t), left(l), right(r), bottom(b) {}
};
class Windows {
private:
    std::vector<Window> windows;
    Margin* margins;
    int num;
    int spacer;
public:
    Windows(int num);
    void shift_right(int amount);
    void shift_left(int amount);
    void draw();
};

#endif //MIDI_ITM_WINDOWSCENE_H