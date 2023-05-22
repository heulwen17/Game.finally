#ifndef BOARD_H
#define BOARD_H

#include "Object.h"
#include <stdlib.h>
#include <time.h>

// Gioi han kich co cua mang gach
#define BOARD_WIDTH 12
#define BOARD_HEIGHT 6
#define BOARD_BRWIDTH 64
#define BOARD_BRHEIGHT 24

class Brick
{
    public:
        int type;
        bool state;
};

class Board: public Object
{
    public:
        Board(SDL_Renderer* renderer);
        ~Board();

        void Update(float delta);
        void Render(float delta);
        void CreateBoard();

        float brickoffsetx, brickoffsety;

        // Khai bao mang hai chieu cua gach
        Brick bricks[BOARD_WIDTH][BOARD_HEIGHT];

    private:
        SDL_Texture* bricktexture;
        SDL_Texture* sidetexture;
};

#endif // BOARD_H
