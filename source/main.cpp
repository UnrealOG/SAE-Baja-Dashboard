#include <opencv2/opencv.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

void draw_circle(SDL_Renderer* renderer, int32_t centerX, int32_t centerY,
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

vector<pair<SDL_Texture*, SDL_Rect>>
generate_numbers(SDL_Renderer* renderer, int max, int increment, double angle1,
				 double angle2, int x, int y, int radius, TTF_Font* font) {
	double amount = (double)max / increment;
	double angle_diff = angle2 - angle1;
	double delta_angle = angle_diff / amount;

	vector<pair<SDL_Texture*, SDL_Rect>> numbers;

	for (int i = 0; i <= amount; i++) {
		SDL_Surface* surface_text = TTF_RenderText_Solid(
			font, to_string(i * increment).c_str(), SDL_Color({0, 0, 0, 0xff}));
		SDL_Texture* text =
			SDL_CreateTextureFromSurface(renderer, surface_text);

		SDL_Rect rect;
		rect.x = x - 10 - radius * cos(angle1 + (delta_angle * i));
		rect.y = y - 10 - radius * sin(angle1 + (delta_angle * i));
		rect.w = 20;
		rect.h = 20;

		numbers.push_back(make_pair(text, rect));
		SDL_FreeSurface(surface_text);
	}

	return numbers;
}

int main() {

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

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

	TTF_Init();
	TTF_Font* sans = TTF_OpenFont("OpenSans.ttf", 30);

	auto speedometer_numbers =
		generate_numbers(renderer, 60, 5, -M_PI / 4, 5 * M_PI / 4,
						 (width / 2 - twidth / 2) / 2, height / 3, 180, sans);
	auto tachyometer_numbers = generate_numbers(
		renderer, 7, 1, -M_PI / 4, 5 * M_PI / 4,
		(3 * width / 2 + twidth / 2) / 2, height / 3, 180, sans);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	double angle = -M_PI / 4;
	double radius = 200;

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
		}

		capture >> frame;
		SDL_UpdateTexture(tex, NULL, (void*)frame.data, frame.step1());

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, tex, NULL, &camera_rect);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

		// Speedometer
		draw_circle(renderer, (width / 2 - twidth / 2) / 2, height / 3, radius);
		SDL_RenderDrawLine(renderer, (width / 2 - twidth / 2) / 2, height / 3,
						   ((double)width / 2 - (double)twidth / 2) / 2 -
							   (radius - 10) * cos(angle),
						   (double)height / 3 - (radius - 10) * sin(angle));
		for (auto& num : speedometer_numbers) {
			SDL_RenderCopy(renderer, num.first, NULL, &num.second);
		}

		// Tachyometer
		draw_circle(renderer, (3 * width / 2 + twidth / 2) / 2, height / 3,
					radius);
		SDL_RenderDrawLine(renderer, (3 * width / 2 + twidth / 2) / 2,
						   height / 3,
						   ((double)3 * width / 2 + (double)twidth / 2) / 2 -
							   (radius - 10) * cos(angle),
						   (double)height / 3 - (radius - 10) * sin(angle));
		for (auto& num : tachyometer_numbers) {
			SDL_RenderCopy(renderer, num.first, NULL, &num.second);
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

		angle += 0.2;
		if (angle > 5 * M_PI / 4)
			angle = -M_PI / 4;

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(sans);
	TTF_Quit();
	SDL_Quit();

	return 0;
}
