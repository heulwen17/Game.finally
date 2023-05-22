#ifndef OBJECT_H
#define OBJECT_H

#include <SDL.h>
#include <SDL_image.h>

class Object
{
    public:
        Object(SDL_Renderer* renderer);
        ~Object();

        float x, y, width, height;

        virtual void Update(float delta);
        virtual void Render(float delta);
        //va cham
        bool Collides(Object* other);
    protected:
        SDL_Renderer* renderer;
};

#endif // OBJECT_H
