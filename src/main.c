#include <raylib.h>

#include "assets.h"
#include "globals.h"
#include "levels.h"

#include "objects/player.h"

#ifdef _MSC_VER
#   ifdef NDEBUG
#      pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#   endif
#endif

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "clone bruhs");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    LoadAssets();

    ChangeLevelTo(&level1);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            gCamera.zoom = (float)GetScreenHeight() / (float)SCREEN_HEIGHT;
        }

        if (IsKeyDown(KEY_ONE))
        {
            ChangeLevelTo(&level1);
        }
        if (IsKeyDown(KEY_TWO))
        {
            ChangeLevelTo(&level2);
        }
        

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);
            

            BeginMode2D(gCamera);
                DrawLevel(&gCurrentLevel);

                ObjectsUpdateAndDraw();
                PlayerUpdate(&gPlayer);
                PlayerDraw(&gPlayer);
            EndMode2D();

            DrawText(TextFormat("%.2f %.2f", gPlayer.obj.position.x, gCamera.offset.x / gCamera.zoom), 10, 50, 18, GREEN);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
