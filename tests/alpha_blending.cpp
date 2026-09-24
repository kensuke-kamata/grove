#include <raylib.h>

Color blend(Color src, Color dst, int alpha)
{
    const unsigned char r = dst.r + (src.r - dst.r) * alpha / 255;
    const unsigned char g = dst.g + (src.g - dst.g) * alpha / 255;
    const unsigned char b = dst.b + (src.b - dst.b) * alpha / 255;
    return Color(r, g, b, 255);
}

int main()
{
    constexpr int screen_width = 800;
    constexpr int screen_height = 450;

    constexpr Rectangle slider_track(220.f, 340.f, 360.f, 8.f);
    constexpr Rectangle slider_hit_area(
        slider_track.x,
        slider_track.y - 15.f,
        slider_track.width,
        slider_track.height + 30.f);

    int alpha = 128;

    InitWindow(screen_width, screen_height, "grove: alpha blending");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const Vector2 mouse = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, slider_hit_area))
        {
            const float pos = (mouse.x - slider_track.x) / slider_track.width;
            alpha = static_cast<int>(pos * 255.f + 0.5f);
        }

        const float knob_x = slider_track.x + (alpha / 255.f) * slider_track.width;
        const int percent = alpha * 100 / 255;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // center rectangle
        const Color color = blend(BLUE, RAYWHITE, alpha);
        DrawRectangle(310, 110, 180, 180, color);

        // slider
        DrawRectangleRec(slider_track, LIGHTGRAY);
        DrawRectangleRec(
            Rectangle(
                slider_track.x,
                slider_track.y,
                knob_x - slider_track.x,
                slider_track.height
            ),
            BLUE);
        DrawCircle(
            static_cast<int>(knob_x),
            static_cast<int>(slider_track.y + slider_track.height / 2.f),
            12.f,
            BLUE);

        DrawText(TextFormat("alpha: %d%%", percent), 350, 375, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
