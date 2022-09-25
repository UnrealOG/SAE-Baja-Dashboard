#include <opencv2/opencv.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <iostream>

using namespace cv;

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
        std::cerr << "SDL renderer could not be creat: " << SDL_GetError()
                  << "\n";
    }

    VideoCapture capture("/dev/video0");
    Mat frame;
    capture >> frame;
    SDL_Texture* tex =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR24,
                          SDL_TEXTUREACCESS_STREAMING, frame.cols, frame.rows);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;

            capture >> frame;
            SDL_UpdateTexture(tex, NULL, (void*)frame.data, frame.step1());

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, tex, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
