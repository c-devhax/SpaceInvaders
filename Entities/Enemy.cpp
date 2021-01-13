//
// Created by alien on 08/01/2021.
//

#include "Enemy.h"

#include "../constants.h"

#include <algorithm>
#include <stdio.h>
#include <chrono>

Enemy::Enemy(int vel, int x, int y, int w, int h, double creation)
    : Ship(vel, x + w/4, y, w, h), creation(creation) {
    // base
    base.x = x + w/4;
    base.y = y;
    base.w = w / 2;
    base.h = (h/3) * 2;

    // left wing
    left_wing.x = x;
    left_wing.y = y;
    left_wing.w = w / 4;
    left_wing.h = h / 3;

    // right wing
    right_wing.x = base.x + base.w;
    right_wing.y = y;
    right_wing.w = left_wing.w;
    right_wing.h = left_wing.h;

    // head
    head.x = base.x + (base.w / 4);
    head.y = y + base.h;
    head.w = left_wing.w;
    head.h = left_wing.h;
}

void Enemy::Draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    Ship::Draw(renderer);
}

void Enemy::Move(double dt) {
    dt -= creation;
    int dist = (int)(vel * (dt * 0.001));
    head.y = (y + dist) + base.h;
    base.y = y + dist;
    left_wing.y = y + dist;
    right_wing.y = y + dist;
    // restricting to screen
    if (head.y > (WINDOW_HEIGHT - head.h)) {
        int diff = head.y - (WINDOW_HEIGHT - head.h);
        head.y -= diff;
        base.y -= diff;
        left_wing.y -= diff;
        right_wing.y -= diff;
    }
}

void Enemy::Die(std::vector<Enemy>& all) {
    auto it = std::find(all.begin(), all.end(), *this);
    if (it == all.end()) {
        printf("Couldn't find enemy to remove\n");
        return;
    }
    all.erase(it);
}

std::tuple<int, int, int, int> Enemy::GetLimits() const {
    return {left_wing.x, left_wing.x + w, base.y, base.y + h};
}

std::tuple<int, int> Enemy::GetXLimits() const {
    return {left_wing.x, left_wing.x + w};
}

bool Enemy::operator==(const Enemy &other) const {
    return (x == other.x) && (y == other.y);
}
