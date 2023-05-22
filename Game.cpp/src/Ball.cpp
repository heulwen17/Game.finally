#include "Ball.h"
#include <iostream>

using namespace std;

Ball::Ball(SDL_Renderer* renderer): Object(renderer)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* surface = IMG_Load("ball.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    x = 0;
    y = 0;
    width = 24;
    height = 24;

    SetDirection(1, 1);
}

Ball::~Ball()
{
    // pha huy ball
    SDL_DestroyTexture(texture);
}

void Ball::Update(float delta)
{
    //toa do
    x += dirx * delta;
    y += diry * delta;
}

void Ball::Render(float delta)
{
    //in ball ra man hinh
    SDL_Rect rect;
    rect.x = (int)(x + 0.5f);
    rect.y = (int)(y + 0.5f);
    rect.w = width;
    rect.h = height;
    SDL_RenderCopy(renderer, texture, 0, &rect);
}

void Ball::SetDirection(float dirx, float diry)
{
    float length = sqrt(dirx * dirx + diry * diry);
    this->dirx = BALL_SPEED * (dirx / length);
    this->diry = BALL_SPEED * (diry / length);
}
