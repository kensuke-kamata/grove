#include "grove/application.h"
#include "grove/profiler.h"

#include <memory>
#include <raylib.h>

namespace grove
{

application::application(const char* title, int width, int height, int fps)
    : title(title)
    , width(width)
    , height(height)
    , fps(fps)
    , metrics(std::make_unique<profiler>())
{
}

application::~application() = default;

void application::run()
{
    InitWindow(width, height, title);
    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE))
        {
            fps = 30;
            SetTargetFPS(fps);
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            fps = 60;
            SetTargetFPS(fps);
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            fps = 120;
            SetTargetFPS(fps);
        }

        const float dt = GetFrameTime();
        metrics->update(dt);
        update(dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw();

        DrawText(
            TextFormat("target: %.2f fps / actual: %.2f fps", static_cast<float>(fps), metrics->fps()),
            16, 16, 20, DARKGRAY
        );

        EndDrawing();
    }

    CloseWindow();
}

}
