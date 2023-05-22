#include "Object.h"

Object::Object(SDL_Renderer* renderer)
{
    this -> renderer = renderer;

    x = 0;
    y = 0;
    width = 1;
    height = 1;
}

Object::~Object() {}

void Object::Update(float delta) {}

void Object::Render(float delta) {}

//check va cham giua 2 doi tuong
bool Object::Collides(Object* object)
{
    if (x + width > object -> x && x < object -> x + object -> width && y + height > object -> y && y < object -> y + object -> height)
    {
        return true;
    }
    return false;
}
