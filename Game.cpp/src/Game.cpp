#include "Game.h"

Game::Game()
{
    window = 0;
    renderer = 0;
}

Game::~Game() {}

bool Game::Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Peak a boo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN );
    if (!window)
    {
        std::cout << "Error creating window:" << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        return false;
    }
    //Timing
    lasttick = SDL_GetTicks();
    fpstick = lasttick;
    fps = 0;

    return true;
}

void Game::Clean()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Game::Run()
{
    SDL_Surface* Menu = IMG_Load("menu.png");
    texture = SDL_CreateTextureFromSurface(renderer, Menu);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(Menu);
    SDL_Delay(7000);

    board = new Board(renderer);
    paddle = new Paddle(renderer);
    ball = new Ball(renderer);

    NewGame();
    // vong lap chinh cua game
    while (1)
    {
        // xu li events
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                break;
            }
        }
        // Tinh delta va fps
        unsigned int curtick = SDL_GetTicks();
        float delta = (curtick - lasttick) / 1000.0f;
        if (curtick - fpstick >= FPS_DELAY)
        {
            fpstick = curtick;
        }
        lasttick = curtick;

        Update(delta);
        Render(delta);
    }

    delete board;
    delete paddle;
    delete ball;

    Clean();

    SDL_Quit();
}

void Game::NewGame()
{
    board -> CreateBoard();
    ResetPaddle();
}

void Game::ResetPaddle()
{
    paddlestick = true;
    StickBall();
}

void Game::StickBall()
{
    //khoi tao toa do ban dau cua ball
    ball -> x = paddle -> x + paddle -> width/2 - ball -> width/2;
    ball -> y = paddle -> y - ball -> height;
}

void Game::Update(float delta)
{
    int mx, my;
    int mouse_state = SDL_GetMouseState(&mx, &my);
    SetPaddleX(mx - paddle -> width/2.0f);

    if (mouse_state&SDL_BUTTON(1))
    {
        if (paddlestick)
        {
            paddlestick = false;
            ball -> SetDirection(1, -1);
        }
    }
    if (paddlestick)
    {
        StickBall();
    }
    CheckBoardCollisions();
    CheckPaddleCollisions();
    CheckBrickCollisions();

    if (GetBrickCount() == 0)
    {
        SDL_Surface* Congratulation = IMG_Load("win.png");
        texture = SDL_CreateTextureFromSurface(renderer, Congratulation);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(Congratulation);
        SDL_Delay(5000);

        Clean();

        SDL_Quit();
    }
    board -> Update(delta);
    paddle -> Update(delta);

    if (!paddlestick)
    {
        ball -> Update(delta);
    }
}

void Game::SetPaddleX(float x)
{
    // giu cho thanh ngang luon nam trong cua so
    float newx;
    if (x < board->x)
    {
        newx = board->x;
    }
    else if (x + paddle->width > board->x + board->width)
    {
        newx = board->x + board->width - paddle->width;
    }
    else
    {
        newx = x;
    }
    paddle->x = newx;
}

void Game::CheckBoardCollisions()
{
    if (ball->y < board->y)
    {
        ball->y = board->y;
        ball->diry *= -1;
    }
    else if (ball->y + ball->height > board->y + board->height)
    {
        // Lam roi bong
        ResetPaddle();
    }
    // Va cham trai, phai
    if (ball->x <= board->x)
    {
        ball->x = board->x;
        ball->dirx *= -1;
    }
    else if (ball->x + ball->width >= board->x + board->width)
    {
        ball->x = board->x + board->width - ball->width;
        ball->dirx *= -1;
    }
}

float Game::GetReflection(float hitx)
{
    if (hitx < 0)
    {
        hitx = 0;
    }
    else if (hitx > paddle->width)
    {
        hitx = paddle->width;
    }
    hitx -= paddle->width / 2.0f;
    // xac dinh diem va cham
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::CheckPaddleCollisions()
{
    // Lay toa do tam cua ball
    float ballcenterx = ball->x + ball->width / 2.0f;
    // Check va cham cua ball va paddle
    if (ball->Collides(paddle))
    {
        ball->y = paddle->y - ball->height;
        ball->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
    }
}

void Game::CheckBrickCollisions()
//check va cham roi pha gach
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];
            // Check trang thai cua brick (bi break hay chua)
            if (brick.state)
            {
                // Toa do brick
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // Toa do tam cua ball
                float ballcenterx = ball->x + 0.5f*ball->width;
                float ballcentery = ball->y + 0.5f*ball->height;

                // Toa do tam cua brick
                float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                if (ball->x <= brickx + BOARD_BRWIDTH && ball->x+ball->width >= brickx && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky)
                {
                    // Xac nhan va cham, pha gach
                    board->bricks[i][j].state = false;

                    float ymin = 0;
                    if (bricky > ball->y)
                    {
                        ymin = bricky;
                    }
                    else
                    {
                        ymin = ball->y;
                    }
                    float ymax = 0;
                    if (bricky+BOARD_BRHEIGHT < ball->y+ball->height)
                    {
                        ymax = bricky + BOARD_BRHEIGHT;
                    }
                    else
                    {
                        ymax = ball->y + ball->height;
                    }
                    float ysize = ymax - ymin;
                    float xmin = 0;
                    if (brickx > ball->x)
                    {
                        xmin = brickx;
                    }
                    else
                    {
                        xmin = ball->x;
                    }
                    float xmax = 0;
                    if (brickx + BOARD_BRWIDTH < ball->x + ball->width)
                    {
                        xmax = brickx + BOARD_BRWIDTH;
                    }
                    else
                    {
                        xmax = ball->x + ball->width;
                    }

                    float xsize = xmax - xmin;
                    // Set va cham dan hoi
                    if (xsize > ysize)
                    {
                        if (ballcentery > brickcentery)
                        {
                            // Duoi
                            ball->y += ysize + 0.01f; // Roi khoi va cham
                            BallBrickResponse(3);
                        }
                        else
                        {
                            // Tren
                            ball->y -= ysize + 0.01f; // Roi khoi va cham
                            BallBrickResponse(1);
                        }
                    }
                    else
                    {
                        if (ballcenterx < brickcenterx)
                        {
                            // Trai
                            ball->x -= xsize + 0.01f; // Roi khoi va cham
                            BallBrickResponse(0);
                        }
                        else
                        {
                            // Phai
                            ball->x += xsize + 0.01f; // Roi khoi va cham
                            BallBrickResponse(2);
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::BallBrickResponse(int dirindex)
//xac dinh huong moi cua ball
{
    // 0: Trai, 1: Tren, 2: Phai, 3: Duoi

    // he so dinh huong
    int mulx = 1;
    int muly = 1;

    if (ball->dirx > 0)
    {
        // Ball chuyen dong theo chieu + cua truc hoanh
        if (ball->diry > 0)
        {
            // Ball chuyen dong theo chieu + cua truc tung
            // +1 +1
            if (dirindex == 0 || dirindex == 3)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
        else if (ball->diry < 0)
        {
            // chuyen dong theo chieu - cua truc tung
            // +1 -1
            if (dirindex == 0 || dirindex == 1)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
    }
    else if (ball->dirx < 0)
    {
        // Ball chuyen dong theo chieu - cua truc hoanh
        if (ball->diry > 0)
        {
            // Ball chuyen dong theo chieu + cua truc tung
            // -1 +1
            if (dirindex == 2 || dirindex == 3)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
        else if (ball->diry < 0)
        {
            // Ball chuyen dong theo chieu - cua truc tung
            // -1 -1
            if (dirindex == 1 || dirindex == 2)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
    }
    // dat lai huong moi cua ball
    ball->SetDirection(mulx*ball->dirx, muly*ball->diry);
}

int Game::GetBrickCount()
{
    int brickcount = 0;
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];
            if (brick.state)
            {
                brickcount++;
            }
        }
    }
    return brickcount;
}

void Game::Render(float delta)
{
    SDL_RenderClear(renderer);
    board->Render(delta);
    paddle->Render(delta);
    ball->Render(delta);
    SDL_RenderPresent(renderer);
}
