#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>

#include "Board.h"
#include "Paddle.h"
#include "Ball.h"

#define FPS_DELAY 500

class Game
{
    public:
        Game();
        ~Game();

        bool Init();
        void Run();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        SDL_Texture* texture;
        // Timing
        unsigned int lasttick, fpstick, fps;
        Board* board;
        Paddle* paddle;
        Ball* ball;
        bool paddlestick;

        void Clean();
        void Update(float delta);
        void Render(float delta);

        void NewGame();
        void ResetPaddle();
        void StickBall();

        void SetPaddleX(float x);
        void CheckBoardCollisions();
        float GetReflection(float hitx);
        void CheckPaddleCollisions();
        void CheckBrickCollisions();
        void BallBrickResponse(int dirindex);
        int GetBrickCount();
};

#endif // GAME_H
