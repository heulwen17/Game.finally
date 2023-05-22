#ifndef PADDLE_H
#define PADDLE_H

#include "Object.h"

class Paddle: public Object
{
    public:
        Paddle(SDL_Renderer* renderer);
        ~Paddle();

        void Update(float delta);
        void Render(float delta);

    private:
        SDL_Texture* texture;
};

#endif // PADDLE_H
