#include <iostream>
#include <vector>

#include "SDL2/SDL.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 1
#define COLOR_SKY 0x4e90c2
#define COLOR_FLOOR 0xc2994e
#define COLOR_FLOOR_ELEMENTS 0x000000
#define HEIGHT_FLOOR 120
#define PLAYER_POS_X 100
#define PLAYER_POS_Y SCREEN_HEIGHT - HEIGHT_FLOOR
#define PLAYER_SIZE 40
#define PLAYER_COLOR 0xfffffff
#define ELEMENT_OFFSET 20
#define FLOOR_ELEMENT_SIZE 5
#define FLOOR_ELEMENT_COUNT 25
#define SKY_ELEMENT_COUNT 10
#define SKY_ELEMENT_SIZE 10
#define COLOR_SKY_ELEMENT 0xffffff
#define PLAYER_SPEED 10

class Player {
   public:
	int pos_x;
	int pos_y;
	int vel_y;

   public:
	Player() {}
	Player(int pos_x, int pos_y) {
		this->pos_x = pos_x;
		this->pos_y = pos_y;
		this->vel_y = 0;
	}

	void update() {
		this->vel_y += GRAVITY;

		this->pos_y += this->vel_y;

		if (this->pos_y >= PLAYER_POS_Y) {
			this->pos_y = PLAYER_POS_Y;
			this->vel_y = 0;
		}
	}

	void jump() {
		if (this->vel_y == 0 && this->pos_y == PLAYER_POS_Y) this->vel_y = -25;
	}

	void render(SDL_Surface *surface) {
		SDL_Rect player = {pos_x, pos_y - PLAYER_SIZE, PLAYER_SIZE,
						   PLAYER_SIZE};
		SDL_FillRect(surface, &player, PLAYER_COLOR);
	}
};

class SkyElement {
   private:
	int pos_x;
	int pos_y;
	int width;
	int height;
	int vel_x;

   public:
	SkyElement() {}
	SkyElement(int pos_x, int pos_y, int width, int height) {
		this->pos_x = pos_x;
		this->pos_y = pos_y;
		this->width = width;
		this->height = height;
		this->vel_x = PLAYER_SPEED;
	}

	void render(SDL_Surface *surface) {
		SDL_Rect element = {pos_x, pos_y, width, height};
		SDL_FillRect(surface, &element, COLOR_SKY_ELEMENT);
	}

	void update() {
		this->pos_x -= this->vel_x;
		if (this->pos_x < 0) {
			this->pos_x = SCREEN_WIDTH;
		}
	}
};

class Sky {
   private:
	std::vector<SkyElement> elements;

   public:
	Sky() {
		for (int i = 0; i < SKY_ELEMENT_COUNT; i++) {
			int rand_x = rand() % (SCREEN_WIDTH - ELEMENT_OFFSET);
			int rand_y = rand() % (SCREEN_HEIGHT - (HEIGHT_FLOOR + 200) -
								   ELEMENT_OFFSET);

			this->elements.push_back(
				SkyElement(rand_x, rand_y, SKY_ELEMENT_SIZE, SKY_ELEMENT_SIZE));
		}
	}

	void render(SDL_Surface *surface) {
		for (int i = 0; i < this->elements.size(); i++) {
			this->elements[i].render(surface);
		}
	}

	void update() {
		for (int i = 0; i < this->elements.size(); i++) {
			this->elements[i].update();
		}
	}
};

class FloorElement {
   private:
	int pos_x;
	int pos_y;
	int width;
	int height;
	int vel_x;

   public:
	FloorElement() {}
	FloorElement(int pos_x, int pos_y, int width, int height) {
		this->pos_x = pos_x;
		this->pos_y = pos_y;
		this->width = width;
		this->height = height;
		this->vel_x = PLAYER_SPEED;
	}

	void render(SDL_Surface *surface) {
		SDL_Rect element = {pos_x, pos_y, width, height};
		SDL_FillRect(surface, &element, COLOR_FLOOR_ELEMENTS);
	}

	void update() {
		this->pos_x -= this->vel_x;
		if (this->pos_x < 0) {
			this->pos_x = SCREEN_WIDTH;
			this->pos_y =
				SCREEN_HEIGHT - (rand() % (SCREEN_HEIGHT - PLAYER_POS_Y +
										   ELEMENT_OFFSET)) >
						SCREEN_HEIGHT - ELEMENT_OFFSET
					? SCREEN_HEIGHT - ELEMENT_OFFSET
					: SCREEN_HEIGHT - (rand() % (SCREEN_HEIGHT - PLAYER_POS_Y +
												 ELEMENT_OFFSET));
		}
	}
};

class Floor {
   private:
	std::vector<FloorElement> elements;

   public:
	Floor() {
		for (int i = 0; i < FLOOR_ELEMENT_COUNT; i++) {
			int rand_x = rand() % (SCREEN_WIDTH - ELEMENT_OFFSET);
			int rand_y =
				SCREEN_HEIGHT - (rand() % (SCREEN_HEIGHT - PLAYER_POS_Y +
										   ELEMENT_OFFSET)) >
						SCREEN_HEIGHT - ELEMENT_OFFSET
					? SCREEN_HEIGHT - ELEMENT_OFFSET
					: SCREEN_HEIGHT - (rand() % (SCREEN_HEIGHT - PLAYER_POS_Y +
												 ELEMENT_OFFSET));
			this->elements.push_back(FloorElement(
				rand_x, rand_y, FLOOR_ELEMENT_SIZE, FLOOR_ELEMENT_SIZE));
		}
	}

	void render(SDL_Surface *surface) {
		SDL_Rect r_floor = {0, SCREEN_HEIGHT - HEIGHT_FLOOR, SCREEN_WIDTH,
							HEIGHT_FLOOR};
		SDL_FillRect(surface, &r_floor, COLOR_FLOOR);
		for (int i = 0; i < this->elements.size(); i++) {
			this->elements[i].render(surface);
		}
	}

	void update() {
		for (int i = 0; i < this->elements.size(); i++) {
			this->elements[i].update();
		}
	}
};

class World {
   private:
	Player *player;
	Floor *floor;
	Sky *sky;

   public:
	World() {
		this->player = new Player(PLAYER_POS_X, PLAYER_POS_Y);
		this->floor = new Floor();
		this->sky = new Sky();
	}

	Player &getPlayer() { return *this->player; }

	void update() {
		this->player->update();
		this->floor->update();
		this->sky->update();
	}
	void render(SDL_Surface *surface) {
		SDL_Rect r_background = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_FillRect(surface, &r_background, COLOR_SKY);

		this->floor->render(surface);
		this->player->render(surface);
		this->sky->render(surface);
	}
};

int main() {
	std::cout << "Hello world" << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow("SDL2 - GOL", SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
										  SCREEN_HEIGHT, 0);

	if (!window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	bool WINDOW_OPEN = true;
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	World world = World();

	while (WINDOW_OPEN) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					WINDOW_OPEN = false;
					break;
				}
				case SDL_KEYDOWN: {
					switch (event.key.keysym.sym) {
						case SDLK_SPACE: {
							world.getPlayer().jump();
							break;
						}
					}
				}
			}
		}
		world.update();
		world.render(surface);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}