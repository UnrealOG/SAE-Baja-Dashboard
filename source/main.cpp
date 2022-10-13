#include <opencv2/opencv.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <iostream>

using namespace cv;

void DrawCircle(SDL_Renderer* renderer, int32_t centerX, int32_t centerY,
				int32_t radius) {
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y) {
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
		SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
		SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);

		if (error <= 0) {
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0) {
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

int main() {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Renderer* dash_renderer = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL failed to initizalize: " << SDL_GetError() << "\n";
		return 1;
	}

	window =
		SDL_CreateWindow("Dashboard", SDL_WINDOWPOS_UNDEFINED,
						 SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "SDL window could not be created: " << SDL_GetError()
				  << "\n";
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "SDL renderer could not be created: " << SDL_GetError()
				  << "\n";
		return 1;
	}

	dash_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!dash_renderer) {
		std::cerr << "SDL renderer could not be created: " << SDL_GetError()
				  << "\n";
		return 1;
	}

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	VideoCapture capture("/dev/video0");
	capture.set(CAP_PROP_FRAME_WIDTH, 400);
	capture.set(CAP_PROP_FRAME_HEIGHT, 300);
	Mat frame;
	capture >> frame;
	SDL_Texture* tex =
		SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR24,
						  SDL_TEXTUREACCESS_STREAMING, frame.cols, frame.rows);

	int twidth, theight;
	SDL_QueryTexture(tex, nullptr, nullptr, &twidth, &theight);
	std::cout << width << ", " << height << "\n";
	std::cout << twidth << ", " << theight << "\n";

	SDL_Rect camera_rect = {width / 2 - twidth / 2, height / 2 - theight / 2,
							twidth, theight};

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(dash_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(dash_renderer);
	SDL_RenderPresent(dash_renderer);

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		SDL_RenderClear(dash_renderer);
		DrawCircle(dash_renderer, width / 4, height / 2, 50);
		SDL_RenderPresent(dash_renderer);

		capture >> frame;
		SDL_UpdateTexture(tex, NULL, (void*)frame.data, frame.step1());

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, tex, NULL, &camera_rect);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
