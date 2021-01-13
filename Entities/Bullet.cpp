//
// Created by alien on 09/01/2021.
//

#include "Bullet.h"

#include <algorithm>
#include <cstdio>

Bullet::Bullet(int vel, int x, int y, int w, int h, double creation)
    : vel(vel), x(x), y(y), w(w), h(h), creation(creation), init_y(y) {
    rect.x = x - (w / 2);
    rect.y = y - (h / 2);
    rect.w = w;
    rect.h = h;
}

void Bullet::Draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0xe2, 0xff, 0x29, 0xff);
    SDL_RenderFillRect(renderer, &rect);
}

void Bullet::Move(double dt) {
    dt -= creation;
    y = init_y + (int)(vel * (dt * 0.001));
    rect.y = y;
}

std::tuple<int, int> Bullet::GetXLimits() const {
    return {x, x + w};
}

std::tuple<int, int, int, int> Bullet::GetLimits() const {
    return {x, x + w, y, y + h};
}

void Bullet::Erase(std::vector<Bullet> &bullets) {
    auto it = std::find(bullets.begin(), bullets.end(), *this);
    if (it == bullets.end()) {
        printf("Couldn't find bullet to erase");
        return;
    }
    bullets.erase(it);
}

bool Bullet::operator==(const Bullet &other) const {
    return (x == other.x) && (y == other.y);
}