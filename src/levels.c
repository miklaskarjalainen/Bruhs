#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "globals.h"
#include "levels.h"

#include "objects/player.h"
#include "objects/goomba.h"
#include "objects/object.h"

const level_t level1 = {
    .data = 
        "........................................................."
        "........................................................."
        "........................................................."
        "........................................................."
        "........................................................."
        "........................................................."
        ".............?..........................................."
        "........................................................."
        "........................................................."
        "........................................................."
        ".......?G..#?#?#........................................."
        "........................................................."
        ".................../|...................................."
        ".4...............4.{}...................................."
        "444444444444444444444444444444444444444444444444444444444",
    .width = 57
};

const level_t level2 = {
    .data = 
        "#.................."
        "#.......oo........."
        "#.........oo......."
        "#....o......o......"
        "#.....o......o....."
        "#.....o.......ooo.."
        "#.....o........oo.."
        "#......o........o.."
        "#........o......o.."
        "#.........ooo.oo.o."
        "#.................."
        "#.................."
        "#......oooooo......"
        "#.................."
        "###################",
    .width = 19
};

void DrawBlock(int x, int y, Color c)
{
    DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, c);
}

void DrawLevel(const level_t* data)
{
    assert(data);

    const int Width = data->width;
    for (int x = 0; x < Width; x++)
    {
        const int xPos = x * 16;
        const int relativeX = (int)(gCamera.offset.x / -gCamera.zoom);
        // Not in view, don't render
        if (xPos + 16 < relativeX)           // left of the screen
            continue;
        if (xPos > SCREEN_WIDTH + relativeX) // right of the screen
            continue;

        for (int y = 0; y < LEVEL_HEIGHT; y++)
        {
            block_type t = data->data[Width * y + x];
            switch (t)
            {
            case BLOCK_AIR:
                break;
            case BLOCK_QB:
                DrawBlock(x, y, ORANGE);
                break;
            case BLOCK_BRICK:
                DrawBlock(x, y, RED);
                break;
            case BLOCK_GROUND:
                DrawBlock(x, y, BROWN);
                break;
            case BLOCK_COIN:
                DrawBlock(x, y, YELLOW);
                break;
            case BLOCK_GOOMBA:
                ObjectSpawn(CreateGoomba((Vector2) { x * 16, y * 16}));
                gCurrentLevel.data[Width * y + x] = BLOCK_AIR;
                
                break;
            default:
                break;
            }
        }
    }
}

int CheckLevelCollision(Rectangle rect, const level_t* level)
{
    assert(level);

    const Rectangle size = {
        (rect.x / 16),
        (rect.y / 16),
        (rect.width / 16),
        (rect.height / 16),
    };

    const int Width = level->width;
    for (int x = size.x; x < (size.x + size.width); x++)
    {
        for (int y = size.y; y < (size.y + size.height); y++)
        {
            block_type t = level->data[Width * y + x];
            switch (t)
            {
            case BLOCK_QB:
            case BLOCK_GROUND:
            case BLOCK_BRICK:
                if (CheckCollisionRecs((Rectangle) { x * 16, y * 16, 16, 16 }, rect))
                    return 1;
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

inline block_type GetBlockAt(int x, int y)
{
    assert(gCurrentLevel.data);
    return gCurrentLevel.data[gCurrentLevel.width * y + x];
}

inline void FreeCurrentLevel()
{
    if (gCurrentLevel.data != NULL)
    {
        free(gCurrentLevel.data);
        gCurrentLevel.data = NULL;
    }
    gCurrentLevel.width = 0;
}

inline void ChangeLevelTo(const level_t* data)
{
    FreeCurrentLevel();
    
    const size_t size = strlen(data->data);
    gCurrentLevel.data = malloc(size);
    assert(gCurrentLevel.data);
    memcpy(gCurrentLevel.data, data->data, size);

    gCurrentLevel.width = data->width;
}
