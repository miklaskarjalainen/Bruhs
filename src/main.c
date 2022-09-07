#include <raylib.h>

#include "globals.h"
#include "player.h"
#include "object.h"
#include "levels.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "clone bruhs");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    gCurrentLevel = &level1;
    while (!WindowShouldClose())
    {
        WindowEvents();

        if (IsKeyDown(KEY_ONE))
        {
            gCurrentLevel = &level1;
        }
        if (IsKeyDown(KEY_TWO))
        {
            gCurrentLevel = &level2;
        }
        PlayerUpdate(&gPlayer);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(10, 10);

            BeginMode2D(gCamera);
                DrawLevel(gCurrentLevel);
                PlayerDraw(&gPlayer);
            EndMode2D();

            DrawText(TextFormat("%f", gPlayer.motion.x), 10, 30, 18, BLACK);
            DrawText(TextFormat("%.2f %.2f", gPlayer.obj.position.x, gCamera.offset.x / gCamera.zoom), 10, 50, 18, GREEN);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
