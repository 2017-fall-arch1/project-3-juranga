#include "../lcdLib/lcdutils.h"
#include "shape.h"

void movePlayerDown(Layer *player) {
  Vec2 newPos;
  Vec2 velocity = {0,12};
  vec2Add(&newPos, &player->pos, &velocity);
  player->pos = newPos;
}
