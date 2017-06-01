class Layer {
private:
	// ...??? how to grab a pointer to the sub-array?
	float (*control_state)[10][10];
    bool motion_layer;
	int x, y;
	~Layer () {}
public:
	Layer(float (*input_state)[10][10])
		:control_state(input_state) {
        motion_layer = false;
	}
    bool isMotionLayer() { return motion_layer; }
	void update () {
		// ...???? how to use that pointer
		y = *control_state[0][0];
		x = control_state[0][0][0] * SCREEN_WIDTH;
	}
	void draw () {
	}
	void render() {
	}
};