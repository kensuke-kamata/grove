#include "game.h"

#include <raylib.h>

namespace grove
{

game::game()
    : application("grove: so long", 640, 480, 60)
{
}

void game::update(float dt)
{
    if (IsKeyDown(KEY_D))
    {
        x += speed * dt;
    }
    else if (IsKeyDown(KEY_A))
    {
        x -= speed * dt;
    }
    else if (IsKeyDown(KEY_S))
    {
        y += speed * dt;
    }
    else if (IsKeyDown(KEY_W))
    {
        y -= speed * dt;
    }
}

void game::draw()
{
    DrawRectangleRec(
        {
            x - size / 2,
            y - size / 2,
            size,
            size
        },
        BLUE
    );
}

}
