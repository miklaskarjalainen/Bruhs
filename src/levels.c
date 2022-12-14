#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#include "assets.h"
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
        "...........................44............................"
        ".......?G..#?#?#...........44............................"
        "...........................44............................"
        "........44.........4444444444444444......................"
        ".4.................4444444444444444......................"
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

void DrawBlockText(int x, int y, Rectangle t)
{
    DrawTexturePro(TileSetSprite, t, (Rectangle) {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, (Vector2){ 0 }, 0.f, WHITE);
}

void DrawLevel(const level_t* data)
{
    assert(data);
    static char ps = 0;
    ps++;
    if (ps > 10)
    {
        ps = 0;
        if (++gAnimFrame > 3)
            gAnimFrame = 0;
    }

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
                DrawBlockText(x, y, TILE_QB(gAnimFrame));
                break;
            case BLOCK_BRICK:
                DrawBlockText(x, y, TILE_BRICK);
                break;
            case BLOCK_GROUND:
                DrawBlockText(x, y, TILE_GROUND);
                break;
            case BLOCK_COIN:
                DrawBlockText(x, y, TILE_COIN(gAnimFrame));
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

block_type GetBlockAt(int x, int y)
{
    assert(gCurrentLevel.data);
    return gCurrentLevel.data[gCurrentLevel.width * y + x];
}

void SetLevelBlock(int tx, int ty, block_type b)
{
    gCurrentLevel.data[ty * gCurrentLevel.width + tx] = b;
}

void ChangeLevelTo(const level_t* data)
{
    if (gCurrentLevel.data != NULL)
        free(gCurrentLevel.data);
    
    const size_t size = strlen(data->data);
    gCurrentLevel.data = malloc(size);
    assert(gCurrentLevel.data);
    memcpy(gCurrentLevel.data, data->data, size);

    gCurrentLevel.width = data->width;
}
