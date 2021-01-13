//
// Created by alien on 08/01/2021.
//

#include "Player.h"
#include "../constants.h"

Player::Player(int health, int vel, int x, int y, int w, int h)
    : Ship(vel, x + w/4, y + h/3, w, h), health(health), lives(PLAYER_LIVES) {
    // setting the base rect
    base.x = this->x;
    base.y = this->y;
    base.w = w/2;
    base.h = (h/3) * 2;

    // left wing
    left_wing.x = x;
    left_wing.y = base.y + h/3;
    left_wing.w = w/4;
    left_wing.h = h/3;

    // right wing
    right_wing.x = base.x + base.w;
    right_wing.y = base.y + h/3;
    right_wing.w = left_wing.w;
    right_wing.h = left_wing.h;

    // head
    head.x = base.x + (base.w / 4);
    head.y = y;
    head.w = left_wing.w;
    head.h = left_wing.h;
}

void Player::Draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x44, 0x00, 0x00, 0xff);
    Ship::Draw(renderer);
}

void Player::Move(Direction dir) {
    // Moving
    if (dir == Direction::Up) {
        head.y -= vel;
        base.y -= vel;
        left_wing.y -= vel;
        right_wing.y -= vel;
    } else if (dir == Direction::Down) {
        head.y += vel;
        base.y += vel;
        left_wing.y += vel;
        right_wing.y += vel;
    } else if (dir == Direction::Right) {
        head.x += vel;
        base.x += vel;
        left_wing.x += vel;
        right_wing.x += vel;
    } else if (dir == Direction::Left) {
        head.x -= vel;
        base.x -= vel;
        left_wing.x -= vel;
        right_wing.x -= vel;
    }
    // Restricting to the edge of the screen
    if (left_wing.x < 0) { // the ship hit the left side of the screen
        int diff = -left_wing.x;
        head.x += diff;
        base.x += diff;
        left_wing.x += diff;
        right_wing.x += diff;
    } else if (right_wing.x > WINDOW_WIDTH - right_wing.w) { // the ship hit the right side of the screen
        int diff = right_wing.x - (WINDOW_WIDTH - right_wing.w);
        head.x -= diff;
        base.x -= diff;
        left_wing.x -= diff;
        right_wing.x -= diff;
    } else if (head.y < 0) { // the ship hit the top of the screen
        int diff = -head.y;
        head.y += diff;
        base.y += diff;
        left_wing.y += diff;
        right_wing.y += diff;
    } else if (base.y > WINDOW_HEIGHT - base.h) { // the ship hit the bottom of the screen
        int diff = base.y - (WINDOW_HEIGHT - base.h);
        head.y -= diff;
        base.y -= diff;
        left_wing.y -= diff;
        right_wing.y -= diff;
    }
}

void Player::TakeDamage(int damage) {
    health -= damage;
    if (health < 0)
        health = 0;
}

std::tuple<int, int, int, int> Player::GetLimits() const {
    return {base.x, base.x + base.w, base.y, base.y + base.h};
}

Bullet Player::Shoot(double creation) {
    int bx = head.x + (head.w / 2);
    int by = head.y;
    return Bullet(-BULLET_VEL, bx, by, BULLET_WIDTH, BULLET_HEIGHT, creation);
}