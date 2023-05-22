#ifndef BALL_H
#define BALL_H

#include "Object.h"
#include <math.h>

// dinh nghia toc do cua ball (px/s)
const float BALL_SPEED = 550;

class Ball: public Object
{
    public:
        Ball(SDL_Renderer* renderer);
        ~Ball();

        void Update(float delta);
        void Render(float delta);

        void SetDirection(float dirx, float diry);
        float dirx, diry;

    private:
        SDL_Texture* texture;
};

#endif // BALL_H
