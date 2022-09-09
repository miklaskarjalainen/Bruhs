#include <raylib.h>

#include "globals.h"
#include "levels.h"

#include "objects/player.h"
#include "objects/goomba.h"
#include "objects/object.h"

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
    
    gCurrentLevel = &level1;

    ObjectSpawn(CreateGoomba((Vector2){100.f, 32.f}));

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            gCamera.zoom = (float)GetScreenHeight() / (float)SCREEN_HEIGHT;
        }

        if (IsKeyDown(KEY_ONE))
        {
            gCurrentLevel = &level1;
        }
        if (IsKeyDown(KEY_TWO))
        {
            gCurrentLevel = &level2;
        }
        

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);

            BeginMode2D(gCamera);
                DrawLevel(gCurrentLevel);

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
