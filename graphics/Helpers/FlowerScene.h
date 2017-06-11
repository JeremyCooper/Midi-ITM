//
// Created by Jeremy Cooper on 6/9/17.
//

#ifndef MIDI_ITM_FLOWERSCENE_H
#define MIDI_ITM_FLOWERSCENE_H


enum petal_dir_t {IN, OUT};
enum petal_side_t {TOP, RIGHT, BOTTOM, LEFT };
class Petal {
private:
    petal_dir_t dir;
    petal_side_t side;
    ci::vec2 pt0, pt1, pt2;
    ci::Color color;
    float fold_amount, time;
    int size, placement;
    void update_coords();
public:
    Petal(petal_dir_t dir, ci::Color color, int placement);
    void draw();
    void update();
};
class Flower {
private:
    std::vector<Petal> petals;
public:
    Flower(int starting_petals);
    void draw();
    void update();
};

#endif //MIDI_ITM_FLOWERSCENE_H
