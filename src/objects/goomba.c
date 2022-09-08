#include "object.h"
#include "goomba.h"

object_t CreateGoomba(const Vector2 position)
{
    return (object_t) {
        .type = OBJ_GOOMBA,
            .position = position,
            .motion = (Vector2){ 32.0f, .0f }, // non 0 x because otherwise would get flipped at start
            .is_grounded = false,
            .dir = DIR_LEFT
    };
}