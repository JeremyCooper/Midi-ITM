class Layer {
private:
	SDL_Texture* tex;
	SDL_Renderer* ren;
	float (*control_state)[10][10];
	SDL_Rect r;
	~Layer () {
		cleanup(tex);
	}
public:
	Layer(SDL_Texture* tex_, SDL_Renderer* ren_,
			float (*input_state)[10][10])
		:tex(tex_), ren(ren_), control_state(input_state) {
		r.x = 0;
		r.y = 0;
		r.w = 200;
		r.h = 200;
	}
	void update () {
		r.w = control_state[0][0][0] * SCREEN_WIDTH;
		SDL_SetRenderTarget(ren, tex);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);
	}
	void draw () {
		SDL_SetRenderDrawColor(ren, *control_state[0][0], 255, 0, 255);
		SDL_RenderFillRect(ren, &r);
	}
	void render() {
		SDL_RenderCopy(ren, tex, NULL, NULL);
	}
};
