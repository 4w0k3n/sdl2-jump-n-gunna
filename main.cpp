#include <iostream>

#include "SDL2/SDL.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.4
#define COLOR_BACKGROUND 0x6a7a8c
#define COLOR_FLOOR 0x000000
#define HEIGHT_FLOOR 120
#define PLAYER_POS_X 100
#define PLAYER_POS_Y SCREEN_HEIGHT - HEIGHT_FLOOR
#define PLAYER_SIZE 40
#define PLAYER_COLOR 0xfffffff

class Player {
   public:
	int pos_x;
	int pos_y;

   public:
	Player() {}
	Player(int pos_x, int pos_y) {
		this->pos_x = pos_x;
		this->pos_y = pos_y;
	}

	void drawPlayer(SDL_Surface *surface) {
		if (this->pos_y < SCREEN_HEIGHT - HEIGHT_FLOOR) {
			this->pos_y++;
		}
		SDL_Rect player = {pos_x, pos_y - PLAYER_SIZE, PLAYER_SIZE,
						   PLAYER_SIZE};
		SDL_FillRect(surface, &player, PLAYER_COLOR);
	}

	void jump(SDL_Surface *surface) { this->pos_y -= 100; }
};

class World {
   private:
	Player player;

   public:
	World() {}
	World(Player *player) { this->player = *player; }
	void drawWorld(SDL_Surface *surface) {
		SDL_Rect background = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_FillRect(surface, &background, COLOR_BACKGROUND);

		SDL_Rect floor = {0, SCREEN_HEIGHT - HEIGHT_FLOOR, SCREEN_WIDTH, 2};
		SDL_FillRect(surface, &floor, COLOR_FLOOR);
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
	Player player(PLAYER_POS_X, PLAYER_POS_Y);
	World world(&player);

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
							std::cout << "Jump" << std::endl;
							player.jump(surface);
							break;
						}
					}
				}
			}
		}
		world.drawWorld(surface);
		player.drawPlayer(surface);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}