//
// Created by alien on 08/01/2021.
//


#pragma once

#include "Ship.h"
#include "Bullet.h"

#include <tuple>

enum class Direction {
    Up,
    Right,
    Down,
    Left
};

class Player : public Ship {
public:
    Player(int health, int vel, int x, int y, int w, int h);
    void Draw(SDL_Renderer* renderer) override;
    std::tuple<int, int, int, int> GetLimits() const override;
    void Move(Direction dir);
    void TakeDamage(int damage);
    Bullet Shoot(double creation);
    bool IsDead() const { return health == 0; }
    int Health() const { return health; }
    int lives;
private:
    int health;
};
