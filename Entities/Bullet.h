//
// Created by alien on 09/01/2021.
//

#pragma once

#include <tuple>
#include <vector>
#include <SDL2/SDL.h>

class Bullet {
public:
    Bullet(int vel, int x, int y, int w, int h, double creation);
    void Draw(SDL_Renderer* renderer) const;
    void Move(double dt);
    std::tuple<int, int> GetXLimits() const;
    std::tuple<int, int, int, int> GetLimits() const;
    void Erase(std::vector<Bullet>& bullets);
    bool operator==(const Bullet& other) const;
private:
    int vel, x, y, w, h, init_y;
    double creation;
    SDL_Rect rect{};
};