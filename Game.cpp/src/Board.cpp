#include "Board.h"

Board::Board(SDL_Renderer* renderer): Object(renderer)
{
    SDL_Surface* surface = IMG_Load("bricks.png");
    bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("side.png");
    sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srand(time(0));

    x = 16;
    y = 0;
    width = 768;
    height = 600;

    brickoffsetx = 0;
    brickoffsety = 16;
}

Board::~Board()
{
    // Ham huy
    SDL_DestroyTexture(bricktexture);
    SDL_DestroyTexture(sidetexture);
}

void Board::Update(float delta) {}

void Board::Render(float delta)
{
    // In gach len man hinh
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = bricks[i][j];
            // Check xem gach vo hay chua
            if (!brick.state)
            {
                continue;
            }
            SDL_Rect srcrect;
            srcrect.x = (brick.type % 2) * BOARD_BRWIDTH;
            srcrect.y = (brick.type / 2) * BOARD_BRHEIGHT;
            srcrect.w = BOARD_BRWIDTH;
            srcrect.h = BOARD_BRHEIGHT;

            SDL_Rect dstrect;
            dstrect.x = brickoffsetx + x + i * BOARD_BRWIDTH;
            dstrect.y = brickoffsety + y + j * BOARD_BRHEIGHT;
            dstrect.w = BOARD_BRWIDTH;
            dstrect.h = BOARD_BRHEIGHT;
            //in gach ra man hinh
            SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
        }
    }
    // In sides ra man hinh
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    dstrect.x = 800 - 16;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);
}

void Board::CreateBoard()
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick;
            brick.type = (i + j) % 4;
            brick.state = true;
            bricks[i][j] = brick;
        }
    }
}
