#include <msp430.h>
#include <libTimer.h>
#include "states.h"
#include "switches.h"
#include "buzzer.h"
#include <lcdutils.h>
#include <lcddraw.h>
#include "abCircle.h"

AbRect paddle = { abRectGetBounds, abRectCheck, {1, 12}};
AbRectOutline fieldOutline = {
  abRectOutlineGetBounds, abRectOutlineCheck,
  {0, 0}
};

u_int bgColor = COLOR_BLUE;

Layer Ball = {
  (AbShape *)&circle3,
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

Layer Field2 = {
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2},
  {0,0}, {0,0},
  COLOR_BLUE,
  &Field
};

char buffer[5] = "0 - 0";
char *scoreBoard = buffer;
Vec2 velocity = {2, 1};
Vec2 upRight = {1, 0};
int p1Score = 0;
int p2Score = 0;

void restart(Vec2 *vel, Layer *ball, Layer *p1, Layer *p2) {
  __asm__(
    "mov #2, 0(R12) \n"
    "mov #1, 2(R12) \n"
    
    "mov #128, 2(R13) \n"
    "rra 2(R13) \n"
    "mov #160, 4(R13) \n"
    "rra 4(R13) \n"
    "mov #128, 6(R13) \n"
    "rra 6(R13) \n"
    "mov #160 , 8(R13) \n"
    "rra 8(R13) \n"
    
    
    "mov #10, 2(R14) \n"
    "mov #160, 4(R14) \n"
    "rra 2(R14) \n"
    "mov #10, 6(R14) \n"
    "mov #160, 8(R14) \n"
    
    "mov #128, 2(R15) \n"
    "sub #10, 2(R15) \n"
    "mov #160, 4(R15) \n"
    "rra 4(R15) \n"
    "mov #128, 6(R15) \n"
    "sub #10, 6(R15) \n"
    "mov #160, 8(R15) \n"
    "rra 8(R15) \n"
    
    "ret \n"
    );
}

typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

MovLayer ml2 = { &Ball, {1, 1}, 0};
MovLayer ml3 = { &Player1, {1,1}, &ml2 }; /**< not all layers move */
MovLayer ml1 = { &Player2, {1,1}, &ml3 }; 
MovLayer ml0 = { &Field, {0,0}, &ml1 }; 

void movePlayerDown(Layer *player) {
  __asm__(
   "add #6, 12(R12) \n"
   "ret \n"
  );
}

void movePlayerUp(Layer *player) {
  __asm__(
  "sub #6, 12(R12) \n"
  "ret \n"
  );
}

void moveBall(Layer *ball, Vec2 *velocity, Vec2 *upRight, Layer *player1, Layer *player2, char *scoreboard) {
  Vec2 newPos;
  int x = upRight->axes[0];
  int y = upRight->axes[1];
  /* This if and Else if handle the Top and Bottom Bounds */
  if (ball->pos.axes[1] == 8) {
    x *= -1;
    buzzer_set_period(2000);
    Vec2 newUpRight = {x, y};
    velocity->axes[0] *= -1; 
    *upRight = newUpRight;
  }
  else if (ball->pos.axes[1] == screenHeight-8) {
    x *= -1;
    buzzer_set_period(2000);
    Vec2 newUpRight = {x, y};
    velocity->axes[0] *= -1;
    *upRight = newUpRight;
  }
  
  /* Handles Player1 Paddle interactions with Ball */
  if (ball->pos.axes[0] == player1->pos.axes[0]+6) {
    if (ball->pos.axes[1] >= player1->pos.axes[1]+3 && ball->pos.axes[1] < player1->pos.axes[1]+13) {
      velocity->axes[0] *= -1;
      buzzer_set_period(3400);
      if (velocity->axes[1] < 0)
	velocity->axes[1] = -(velocity->axes[1]);
    }
    else if (ball->pos.axes[1] >= player1->pos.axes[1]-12 && ball->pos.axes[1] < player1->pos.axes[1]+3) {
      velocity->axes[0] *= -1;
      buzzer_set_period(3400);
      if ((velocity->axes[1]) > 0)
	velocity->axes[1] = -(velocity->axes[1]);
    }
  }
  /* Handles Player2 paddle interactions w/ ball. */
  else if (ball->pos.axes[0] == player2->pos.axes[0]-6) {
    if (ball->pos.axes[1] >= player2->pos.axes[1]+3 && ball->pos.axes[1] < player2->pos.axes[1]+13) {
      velocity->axes[0] *= -1;
      buzzer_set_period(3400);
      if (velocity->axes[1] < 0)
	velocity->axes[1] = -(velocity->axes[1]);
    }
    else if (ball->pos.axes[1] >= player2->pos.axes[1]-12 && ball->pos.axes[1] < player2->pos.axes[1]+3) {
      velocity->axes[0] *= -1;
      buzzer_set_period(3400);
      if (velocity->axes[1] >0)
	velocity->axes[1] = velocity->axes[1];
    }
  }
  /* Handles Score if P2 Scores*/
  if (ball->pos.axes[0] <= 4) {
    scoreboard[4] = '0' + (p2Score+1);
    p2Score += 1;
    buzzer_set_period(2500);
    //states_init();
    restart(velocity, &Ball, &Player1, &Player2);
    layerDraw(&Field);
    drawString5x7(screenWidth/2-15,20, scoreBoard, COLOR_GREEN, COLOR_BLUE);
    //__delay_cycles(2 * 16000000);
  }
  /* Handles Score if P1 Scores */
  if (ball->pos.axes[0] >= screenWidth - 4) {
    and_sr(~8);
    scoreboard[0] = '0' + (p1Score+1);
    p1Score += 1;
    buzzer_set_period(2500);
    //states_init();
    restart(velocity, &Ball, &Player1, &Player2);
    //layerDraw(&Field);
    drawString5x7(screenWidth/2-15,20, scoreBoard, COLOR_GREEN, COLOR_BLUE);
    //__delay_cycles(2 * 16000000);
  }

  if (x >= 0)
    vec2Add(&newPos, &ball->pos, velocity);
  else if (x < 0)
    vec2Sub(&newPos, &ball->pos, velocity);
  ball->posNext = newPos;
  or_sr(8);
}

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;			/**< disable interrupts (GIE on) */

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */

  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



void states_init()
{
  layerInit(&Field);
  layerDraw(&Field);
  Vec2 newRight = {1, 0};
  upRight = newRight;
  P1DIR |= LEDS;
  layerDraw(&Field);
  drawString5x7(screenWidth/2-15,20, scoreBoard, COLOR_GREEN, COLOR_BLUE);
  //__delay_cycles(2 * 16000000);
  enableWDTInterrupts();
  switch_init();
  or_sr(0x8);
  for(;;) {
    //switch_interrupt_handler();
    movLayerDraw(&ml0, &Field2);
    drawString5x7(screenWidth/2-15,20, scoreBoard, COLOR_GREEN, COLOR_BLUE);
  }
}

void state_update()
{
  static short count2 = 0;
  count2 ++;
  if (count2 == 15) {
  if (state0) {
    movePlayerUp(&Player1);
  }
  else if (state1) {
    movePlayerDown(&Player1);
  }
  else if (state2) {
    movePlayerDown(&Player2);
  }
  else if (state3) {
    movePlayerUp(&Player2);
  }
  count2 = 0;
    moveBall(&Ball, &velocity, &upRight, &Player1, &Player2, scoreBoard);
    }
  buzzer_set_period(0);
}
    
