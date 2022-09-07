#ifndef LEVELS_H
#define LEVELS_H

#define BLOCK_SIZE 16
#define LEVEL_HEIGHT 15 // blocks

typedef enum
{
    BLOCK_AIR = '.',
    BLOCK_QB  = '?',
    BLOCK_BRICK = '#',
    BLOCK_GROUND = '4',
    BLOCK_COIN = 'o',
} block_type;

typedef struct level_t
{
    const char* data; // Pure level data
    int width;
} level_t;

extern const level_t level1;
extern const level_t level2;

void DrawLevel(const level_t* data);
int  CheckLevelCollision(struct Rectangle rect, const level_t* level);

#endif
