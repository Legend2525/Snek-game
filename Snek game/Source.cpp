#include <SDL.h>
#include <iostream>
#include <vector>

const int BLOCK_SIZE = 10;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct SnakeBlock {
    int x, y;
};

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    int speed = 100;
    int direction = SDLK_RIGHT;

    std::vector<SnakeBlock> snake;
    SnakeBlock block;
    block.x = WINDOW_WIDTH / 2;
    block.y = WINDOW_HEIGHT / 2;
    snake.push_back(block);

    SnakeBlock food;
    food.x = rand() % (WINDOW_WIDTH - BLOCK_SIZE);
    food.y = rand() % (WINDOW_HEIGHT - BLOCK_SIZE);

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP && direction != SDLK_DOWN) {
                    direction = SDLK_UP;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && direction != SDLK_UP) {
                    direction = SDLK_DOWN;
                }
                else if (event.key.keysym.sym == SDLK_LEFT && direction != SDLK_RIGHT) {
                    direction = SDLK_LEFT;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT && direction != SDLK_LEFT) {
                    direction = SDLK_RIGHT;
                }
            }
        }

        SnakeBlock head = snake.front();
        switch (direction) {
        case SDLK_UP:
            head.y -= BLOCK_SIZE;
            break;
        case SDLK_DOWN:
            head.y += BLOCK_SIZE;
            break;
        case SDLK_LEFT:
            head.x -= BLOCK_SIZE;
            break;
        case SDLK_RIGHT:
            head.x += BLOCK_SIZE;
            break;
        }

        if (head.x < 0 || head.x >= WINDOW_WIDTH || head.y < 0 || head.y >= WINDOW_HEIGHT) {
            quit = true;
        }

        for (auto it = snake.begin() + 1; it != snake.end(); it++) {
            if (head.x == it->x && head.y == it->y) {
                quit = true;
                break;
            }
        }

        snake.insert(snake.begin(), head);
        if (head.x == food.x && head.y == food.y) {
            food.x = rand() % (WINDOW_WIDTH - BLOCK_SIZE);
            food.y = rand() % (WINDOW_HEIGHT - BLOCK_SIZE);
        }
        else {
            snake.pop_back();
        }

        SDL_SetRenderDrawColor(renderer, 0,



            
            0, 0, 255);
        SDL_RenderClear(renderer);

  
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto it = snake.begin(); it != snake.end(); it++) {
            SDL_Rect blockRect = { it->x, it->y, BLOCK_SIZE, BLOCK_SIZE };
            SDL_RenderFillRect(renderer, &blockRect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect foodRect = { food.x, food.y, BLOCK_SIZE, BLOCK_SIZE };
        SDL_RenderFillRect(renderer, &foodRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(speed);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}