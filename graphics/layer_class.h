class Layer {
private:
	// ...??? how to grab a pointer to the sub-array?
	float (*control_state)[10][10];
	int x, y;
	~Layer () {}
public:
	Layer(float (*input_state)[10][10])
		:control_state(input_state) {
	}
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
