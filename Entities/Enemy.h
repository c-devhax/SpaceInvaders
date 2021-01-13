//
// Created by alien on 08/01/2021.
//

#pragma once

#include "Ship.h"
#include "../constants.h"

#include <tuple>
#include <vector>

class Enemy : public Ship {
public:
    Enemy(int vel, int x, int y, int w, int h, double creation);
    void Draw(SDL_Renderer* renderer) override;
    std::tuple<int, int, int, int> GetLimits() const override;
    std::tuple<int, int> GetXLimits() const;
    void Move(double dt);
    void Die(std::vector<Enemy>& all);
    bool CollidesWithWall() const { return head.y >= WINDOW_HEIGHT - head.h; }
    bool operator==(const Enemy& other) const;
private:
    double creation;
};