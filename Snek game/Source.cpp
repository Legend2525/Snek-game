#include <SDL.h>
#include <iostream>
#include <vector>

const int BLOCK_SIZE = 10;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct SnakeBlock
{
    int x, y;
};

enum class SnakeDir
{
    North,
    East,
    South,
    West
};

int RandomRange(int min, int max)
{
    int randNum = rand() % (max - min + 1) + min;

    return randNum;
}

int RoundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

int main(int argc, char* argv[])
{
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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    int speed = 10;
    SnakeDir direction = SnakeDir::North;

    std::vector<SnakeBlock> snake;
    SnakeBlock block;
    block.x = WINDOW_WIDTH / 2;
    block.y = WINDOW_HEIGHT / 2;
    snake.push_back(block);

    srand(time(0));

    SnakeBlock food;
    food.x = RoundUp(RandomRange(0, WINDOW_WIDTH - BLOCK_SIZE), BLOCK_SIZE);
    food.y = RoundUp(RandomRange(0, WINDOW_HEIGHT - BLOCK_SIZE), BLOCK_SIZE);

    int score = 0;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
                {
                    direction = SnakeDir::North;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
                {
                    direction = SnakeDir::East;
                }
                else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                {
                    direction = SnakeDir::South;
                }
                else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
                {
                    direction = SnakeDir::West;
                }
            }
        }

        SnakeBlock head = snake.front();

        switch (direction)
        {
        case SnakeDir::North:
            head.y -= BLOCK_SIZE;
            break;
        case SnakeDir::South:
            head.y += BLOCK_SIZE;
            break;
        case SnakeDir::West:
            head.x -= BLOCK_SIZE;
            break;
        case SnakeDir::East:
            head.x += BLOCK_SIZE;
            break;
        }

        if (head.x < 0 || head.x >= WINDOW_WIDTH || head.y < 0 || head.y >= WINDOW_HEIGHT)
        {
            std::cout << "You lose!\n";
            std::cout << "Score: " << score << "\n";
            quit = true;
        }

        for (auto it = snake.begin() + 1; it != snake.end(); it++)
        {
            if (head.x == it->x && head.y == it->y)
            {
                std::cout << "You lose!\n";
                std::cout << "Score: " << score << "\n";
                quit = true;
                break;
            }
        }

        snake.insert(snake.begin(), head);

        if (head.x == food.x && head.y == food.y)
        {
            food.x = RoundUp(RandomRange(0, WINDOW_WIDTH - BLOCK_SIZE), BLOCK_SIZE);
            food.y = RoundUp(RandomRange(0, WINDOW_HEIGHT - BLOCK_SIZE), BLOCK_SIZE);
            score++;

            std::cout << "Score: " << score << "\n";

            if (score >= 100)
            {
                std::cout << "You win!\n";
            }
        }
        else
        {
            snake.pop_back();
        }

        SDL_SetRenderDrawColor(renderer, 18, 18, 18, 255);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto it = snake.begin(); it != snake.end(); it++)
        {
            SDL_Rect blockRect = { it->x, it->y, BLOCK_SIZE, BLOCK_SIZE };
            SDL_RenderFillRect(renderer, &blockRect);
        }



        SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255);

        SDL_Rect foodRect{};
        foodRect.x = food.x;
        foodRect.y = food.y;
        foodRect.w = BLOCK_SIZE;
        foodRect.h = BLOCK_SIZE;

        SDL_RenderFillRect(renderer, &foodRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(1 / (speed / 1000.0f));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}