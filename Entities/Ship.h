//
// Created by alien on 08/01/2021.
//

#pragma once

#include <tuple>
#include <SDL2/SDL.h>

class Ship {
public:
    Ship(int vel, int x, int y, int w, int h)
        : vel(vel), x(x), y(y), w(w), h(h) {

    }

    virtual void Draw(SDL_Renderer* renderer) {
        SDL_RenderFillRect(renderer, &base);
        SDL_RenderFillRect(renderer, &right_wing);
        SDL_RenderFillRect(renderer, &left_wing);
        SDL_RenderFillRect(renderer, &head);
    }

    virtual std::tuple<int, int, int, int> GetLimits() const { return {0, 0, 0, 0}; }

protected:
    int vel = 0;
    int x, y, w, h;
    SDL_Rect base{};
    SDL_Rect right_wing{};
    SDL_Rect left_wing{};
    SDL_Rect head{};
};