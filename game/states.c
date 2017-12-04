#include <msp430.h>

#include "states.h"
#include "switches.h"
#include "buzzer.h"
#include <lcdutils.h>
#include <lcddraw.h>
#include "abCircle.h"

AbRect paddle = { abRectGetBounds, abRectCheck, {1, 12}};
AbRectOutline fieldOutline = {
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2, screenHeight/2}
};

u_int bgColor = COLOR_BLUE;

Layer Ball = {
  (AbShape *)&circle2,
  {screenWidth/2, screenHeight/2},
  {0,0}, {0,0},
  COLOR_WHITE,
  0
};

Layer Player1 = {
  (AbShape *)&paddle,
  {10, screenHeight/2},
  {0,0}, {0,0},
  COLOR_GREEN,
  &Ball
};

Layer Player2 = {
  (AbShape *)&paddle,
  {screenWidth - 10, screenHeight/2},
  {0,0}, {0,0},
  COLOR_GREEN,
  &Player1
};

Layer Field = {
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},
  {0,0}, {0,0},
  COLOR_BLUE,
  &Player2
};

void movePlayerUp(Layer *player) {
  Vec2 newPos;
  Vec2 velocity = {0,12};
  vec2Add(&newPos, &player->pos, &velocity);
  player->pos = newPos;
}

void movePlayerDown(Layer *player) {
  Vec2 newPos;
  Vec2 velocity = {0, 12};
  vec2Sub(&newPos, &player->pos, &velocity);
  player->pos = newPos;
}

Vec2 velocity = {8, 2};
Vec2 upRight = {1, 0};

void moveBall(Layer *ball, Vec2 *velocity, Vec2 *upRight, Layer *player1, Layer *player2, char **scoreboard) {
  Vec2 newPos;
  int x = upRight->axes[0];
  int y = upRight->axes[1];
  if (ball->pos.axes[1] == 8) {
    x *= -1;
    buzzer_set_period(2000);
    Vec2 newupRight = {x, y};
    velocity->axes[0] *= -1; 
    *upRight = newupRight;
  }
  else if (ball->pos.axes[1] == screenHeight-8) {
    x *= -1;
    buzzer_set_period(2000);
    Vec2 newUpRight = {x, y};
    velocity->axes[0] *= -1;
    *upRight = newUpRight;
  }
  if (ball->pos.axes[0] == player1->pos.axes[0]+6) {
    //buzzer_set_period(5000);
    int x2 = velocity->axes[0];
    int y2 = velocity->axes[1];
    if (ball->pos.axes[1] >= player1->pos.axes[1]+3 && ball->pos.axes[1] < player1->pos.axes[1]+13) {
      x2 *= -1;
      buzzer_set_period(3400);
      if (y2 < 0)
	y2 = -(velocity->axes[1]);
      Vec2 newVelocity = {x2, y2};
      *velocity = newVelocity;
    }
    else if (ball->pos.axes[1] >= player1->pos.axes[1]-12 && ball->pos.axes[1] < player1->pos.axes[1]+3) {
      x2 *= -1;
      buzzer_set_period(3400);
      if (y2 > 0)
	y2 = -(velocity->axes[1]);
      Vec2 newVelocity = {x2, y2};
      *velocity = newVelocity;
    }
    else {
      Vec2 newVelocity = {x2, y2};
      *velocity = newVelocity;
      short temp = (*scoreboard[4] << 8 ) + 1;
      *scoreboard[4] = temp >> 8;
      drawString5x7(20,20, *scoreboard, COLOR_GREEN, COLOR_RED);
    }
  }
  else if (ball->pos.axes[0] == player2->pos.axes[0]-6) {
    int x2 = velocity->axes[0];
    int y2 = velocity->axes[1];
    if (ball->pos.axes[1] >= player2->pos.axes[1]+3 && ball->pos.axes[1] < player2->pos.axes[1]+13) {
      x2 *= -1;
      buzzer_set_period(3400);
      if (y2 < 0)
	y2 = -(velocity->axes[1]);
      Vec2 newVelocity = {x2, y2};
      *velocity = newVelocity;
    }
    else if (ball->pos.axes[1] >= player2->pos.axes[1]-12 && ball->pos.axes[1] < player2->pos.axes[1]+3) {
      x2 *= -1;
      buzzer_set_period(3400);
      if (y2 >0)
	y2 = velocity->axes[1];
      Vec2 newVelocity = {x2, y2};
      *velocity = newVelocity;
    }
    else {
      Vec2 newVelocity = {x2, y2};
      *velocity = newVelocity;
    }
  }
  
  if (x >= 0)
    vec2Add(&newPos, &ball->pos, velocity);
  else if (x < 0)
    vec2Sub(&newPos, &ball->pos, velocity);
  ball->pos = newPos;
  layerDraw(&Field);
}

char *scoreBoard = "0 - 0";

void states_init()
{
  P1DIR |= LEDS;
  drawString5x7(20,20, scoreBoard, COLOR_GREEN, COLOR_RED);
  layerDraw(&Field);
  state_update();
}

void switches(char ledFlags) {
  P1OUT &= (0xff - LEDS) | ledFlags;
  P1OUT |= ledFlags;
}

void state_update()
{
  char ledFlags = 0;
  if (state0) {
    movePlayerDown(&Player1);
  }
  else if (state1) {
    movePlayerUp(&Player1);
  }
  else if (state2) {
    movePlayerUp(&Player2);
  }
  else if (state3) {
    movePlayerDown(&Player2);
  }
  else {
    buzzer_set_period(0);
  }
  moveBall(&Ball, &velocity, &upRight, &Player1, &Player2, &scoreBoard);
}
    
