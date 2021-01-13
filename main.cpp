#include "constants.h"
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Entities/Bullet.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <chrono>
#include <SDL2/SDL.h>

int RandRange(int min, int max) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

std::tuple<int, int> RandCoord(const int min_x, const int max_x, const int min_y, const int max_y) {
    return {RandRange(min_x, max_x), RandRange(min_y, max_y)};
}

bool ShipsCollide(const Enemy& enemy, const Player& player) {
    auto[e_left, e_right, e_top, e_bottom] = enemy.GetLimits();
    auto[p_left, p_right, p_top, p_bottom] = player.GetLimits();

    if (p_top < e_bottom && p_top > e_top) {
        if (p_right < e_right && p_right > e_left) return true;
        else if (p_left < e_right && p_left > e_left) return true;
    }
    else if (p_bottom < e_top && p_bottom > e_bottom) {
        if (p_right < e_right && p_right > e_left) return true;
        else if (p_left < e_right && p_left > e_left) return true;
    }
    return false;
}

bool CheckBulletCollision(Enemy& enemy, Bullet& bullet) {
    auto[e_left, e_right, e_top, e_bottom] = enemy.GetLimits();
    auto[b_left, b_right, b_top, b_bottom] = bullet.GetLimits();

    return e_bottom > b_top;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        exit(2);
    }

    SDL_Window* window = SDL_CreateWindow("Space Invaders", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (window == nullptr || renderer == nullptr) {
        std::cout << "Couldn't initialize window or renderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(2);
    }
    
    Player player(PLAYER_HEALTH, PLAYER_VEL,
                  (WINDOW_WIDTH / 2) - (PLAYER_WIDTH / 2), ((WINDOW_HEIGHT / 8) * 7) - (PLAYER_HEIGHT / 2),
                  PLAYER_WIDTH, PLAYER_HEIGHT);

    // Main loop
    bool running = true;

    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    double dt = 0;
    while (running) {
        SDL_Event event;
        auto start = std::chrono::high_resolution_clock::now();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                else if (event.key.keysym.sym == SDLK_w) player.Move(Direction::Up);
                else if (event.key.keysym.sym == SDLK_s) player.Move(Direction::Down);
                else if (event.key.keysym.sym == SDLK_a) player.Move(Direction::Left);
                else if (event.key.keysym.sym == SDLK_d) player.Move(Direction::Right);
                else if (event.key.keysym.sym == SDLK_SPACE) bullets.push_back(player.Shoot(dt));
            }
        }

        // clearing the screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer);

        // spawning enemies
        if ((int)dt % 2000 == 0) { // once every 20 seconds
            for (int i = 0; i < ENEMIES_PER_S * 5; i++) {
                long now = std::chrono::time_point_cast<std::chrono::microseconds>(
                        std::chrono::high_resolution_clock::now()
                        ).time_since_epoch().count();
                long startTime = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
                long duration = now - startTime;

                int min_x = 0, max_x = WINDOW_WIDTH - ENEMY_WIDTH,
                    min_y = 0, max_y = WINDOW_HEIGHT - ENEMY_HEIGHT;
                auto[x, y] = RandCoord(min_x, max_x, min_y, max_y);
                Enemy new_enemy(ENEMY_VEL, x, 0, ENEMY_WIDTH, ENEMY_HEIGHT, dt + duration);
                enemies.push_back(new_enemy);
            }
        }

        // moving enemies
        for (Enemy& enemy : enemies)
            enemy.Move(dt);
        for (Bullet& bullet : bullets)
            bullet.Move(dt);

        // checking for collisions
        {
            // enemy-player
            for (Enemy& enemy : enemies) {
                if (ShipsCollide(enemy, player)) {
                    enemy.Die(enemies);
                    player.TakeDamage(ENEMY_IMPACT_DMG);
                    std::cout << player.Health() << std::endl;
                }
                else if (enemy.CollidesWithWall()) {
                    enemy.Die(enemies);
                    player.lives--;
                }
            }
            // enemy-bullet
            for (Enemy& enemy : enemies) {
                for (Bullet& bullet : bullets) {
                    auto[ex, eX] = enemy.GetXLimits();
                    auto[bx, bX] = enemy.GetXLimits();
                    if ((ex < bx && bx < eX) || (bx < ex && ex < bX)) {
                        if (CheckBulletCollision(enemy, bullet)) {
                            enemy.Die(enemies);
                            bullet.Erase(bullets);
                        }
                    }
                }
            }
        }

        if (player.IsDead() || player.lives <= 0)
            running = false;

        // drawing entities
        player.Draw(renderer);
        for (Enemy& enemy : enemies) {
            enemy.Draw(renderer);
        }
        for (Bullet& bullet : bullets) {
            bullet.Draw(renderer);
        }

        // displaying
        SDL_RenderPresent(renderer);

        // updating time
        auto end = std::chrono::high_resolution_clock::now();
        long startTime = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
        long endTime = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
        long duration = endTime - startTime;
        dt += duration * 0.001;
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
