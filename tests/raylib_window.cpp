#include <raylib.h>

int main()
{
    constexpr int screen_width = 800;
    constexpr int screen_height = 450;

    InitWindow(screen_width, screen_height, "grove: raylib window");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, raylib!", 300, 205, 32, DARKGREEN);
        EndDrawing();
    }

    CloseWindow();
}
