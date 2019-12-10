#include "myLib.h"
#include "levelTwo.h"
// Variables
int hOff;
int vOff;
OBJ_ATTR shadowOAM[128];
ANISPRITE paddleOne;
ANISPRITE paddleTwo;
ANISPRITE ball;
ANISPRITE player2;
int bouncesRemaining;
int bounceTimer;
int level2SpdMod;
int powerUp;

// player2 animation states for aniState
enum {PIKAFRONT, PIKABACK, PIKARIGHT, PIKALEFT, PIKAIDLE};

// initialize the second level
void initLevelTwo() {
    powerUp = 0;
	
    level2SpdMod = 1;
    bouncesRemaining = 7;

    //initialize player2 for level two    
    player2.width = 16;
    player2.height = 16;
    player2.rdel = 1;
    player2.cdel = 1;

    player2.screenRow = SCREENHEIGHT/2-player2.width/2;
    player2.screenCol = SCREENWIDTH/2-player2.height/2;
    player2.aniCounter = 0;
    player2.curFrame = powerUp;
    player2.numFrames = 3;
    player2.aniState = PIKAFRONT;
    player2.hide = 0;

    // Initialize Paddle One
    paddleOne.width = 16;
    paddleOne.height = 16;
    paddleOne.cdel = -1;

    paddleOne.screenCol = SCREENWIDTH/2 - paddleOne.width/2;
    paddleOne.screenRow = SCREENHEIGHT - 20;
    paddleOne.aniState = 1;
    paddleOne.curFrame = 4;
    paddleOne.hide = 0;

    // Initialize Paddle Two
    paddleTwo.width = 16;
    paddleTwo.height = 16;
    paddleTwo.cdel = 1;

    paddleTwo.screenCol = SCREENWIDTH/2 - paddleOne.width/2;
    paddleTwo.screenRow = 20;
    paddleTwo.aniState = 1;
    paddleTwo.curFrame = 4;
    paddleTwo.hide = 0;

    // Initialize Ball
    ball.width = 16;
    ball.height = 16;
    ball.rdel = 1;
    ball.cdel = 1;
    int ballCol = rand() % 110;
    int ballRow = rand() % 130 + 10;
    if (abs(ballCol - player2.screenCol) < 40) {
        ballCol = 10;
    }
    if (abs(ballRow - player2.screenRow) < 40) {
        ballRow = 10;
    }
    ball.screenCol = ballCol;
    ball.screenRow = ballRow;
    ball.curFrame = 4;
    ball.aniState = 0;
    ball.hide = 0;
    
}

void updateLevelTwo() {
    // Update player2
    if (BUTTON_HELD(BUTTON_RIGHT) && player2.screenCol + player2.width < SCREENWIDTH - 40) {
        player2.screenCol += player2.cdel * level2SpdMod;
    }
    if (BUTTON_HELD(BUTTON_LEFT) && player2.screenCol > 40) {
        player2.screenCol-= player2.cdel * level2SpdMod;
    }
    if (BUTTON_HELD(BUTTON_UP) && player2.screenRow > 20 + paddleOne.height) {
        player2.screenRow-= player2.rdel * level2SpdMod;
    }
    if (BUTTON_HELD(BUTTON_DOWN) && player2.screenRow + player2.height < SCREENHEIGHT - 20) {
        player2.screenRow+= player2.rdel * level2SpdMod;
    }
    if (BUTTON_PRESSED(BUTTON_B) && level2SpdMod < 2) {
        level2SpdMod *= 2;
        powerUp = 5;
        player2.curFrame = powerUp;
    }
    //Update Paddle Two
    if (!paddleTwo.hide) {
        paddleTwo.screenCol += paddleTwo.cdel;
        if (paddleTwo.screenCol < 0 || paddleTwo.screenCol + paddleTwo.width > SCREENWIDTH) {
            paddleTwo.cdel *= -1;
        }
    }
    //Update Paddle One
    if (!paddleOne.hide) {
        paddleOne.screenCol += paddleOne.cdel;
        if (paddleOne.screenCol < 0 || paddleOne.screenCol + paddleOne.width > SCREENWIDTH) {
            paddleOne.cdel *= -1;
        }
    }
    //Update Ball
    ball.screenCol += ball.cdel * 2;
    ball.screenRow += ball.rdel * 2;
    
    if((collision(ball.screenCol, ball.screenRow, 16, 16, paddleOne.screenCol - 16, paddleOne.screenRow, 64, 16) ||
     collision(ball.screenCol, ball.screenRow, 16, 16, paddleTwo.screenCol - 16, paddleTwo.screenRow, 64, 16))) {
        //  ball.cdel *= -1;
         ball.rdel *= -1;
         if (bounceTimer % 20 == 0) {
         bouncesRemaining--;
         }
    }

    if (ball.screenCol <= 0 || ball.screenCol + ball.width - 1 >= SCREENWIDTH - 1) {
        ball.cdel *= -1;
    }

    if (ball.screenRow <= 0 || ball.screenRow + ball.height - 1 >= SCREENHEIGHT - 1) {
        ball.rdel *= -1;
    }

    if (collision(ball.screenCol, ball.screenRow, 16, 16, player2.screenCol, player2.screenRow, 16, 16)) {
        player2.hide = 1;
    }
    // Animate Stuff
    animateLevelTwo();
    bounceTimer++;
}

void drawLevelTwo() {

    if (player2.hide) {
        shadowOAM[0].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[0].attr0 = (ROWMASK & player2.screenRow) | ATTR0_SQUARE;
        shadowOAM[0].attr1 = (COLMASK & player2.screenCol) | ATTR1_SMALL;
        shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(player2.aniState * 2, player2.curFrame * 2);
    }

    if (paddleOne.hide) {
        shadowOAM[1].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[1].attr0 = (ROWMASK & paddleOne.screenRow) | ATTR0_SQUARE | ATTR0_WIDE;
        shadowOAM[1].attr1 = (COLMASK & paddleOne.screenCol) | ATTR1_SMALL;
        shadowOAM[1].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(paddleOne.aniState * 2, paddleOne.curFrame * 2);
    }

    if (paddleTwo.hide) {
        shadowOAM[2].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[2].attr0 = (ROWMASK & paddleTwo.screenRow) | ATTR0_SQUARE | ATTR0_WIDE;
        shadowOAM[2].attr1 = (COLMASK & paddleTwo.screenCol) | ATTR1_SMALL;
        shadowOAM[2].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(paddleTwo.aniState * 2, paddleTwo.curFrame * 2);
    }

    if (ball.hide) {
        shadowOAM[3].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[3].attr0 = (ROWMASK & ball.screenRow) | ATTR0_SQUARE;
        shadowOAM[3].attr1 = (COLMASK & ball.screenCol) | ATTR1_SMALL;
        shadowOAM[3].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(ball.aniState * 2, ball.curFrame * 2);
    }

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    
}

void animateLevelTwo() {
    // Set previous state to current state
    player2.prevAniState = player2.aniState;
    player2.aniState = PIKAIDLE;

    // Change the animation frame every 20 frames of gameplay
    if(player2.aniCounter % 20 == 0) {
        if (player2.curFrame < player2.numFrames -1){
        player2.curFrame = (player2.curFrame + 1);
        } else {
            player2.curFrame = powerUp;
        }
    }

    // Control movement and change animation state
    if(BUTTON_HELD(BUTTON_UP))
        player2.aniState = PIKABACK;
    if(BUTTON_HELD(BUTTON_DOWN))
        player2.aniState = PIKAFRONT;
    if(BUTTON_HELD(BUTTON_LEFT))
        player2.aniState = PIKALEFT;
    if(BUTTON_HELD(BUTTON_RIGHT))
        player2.aniState = PIKARIGHT;

    // If the player2 aniState is idle, frame is player2 standing
    if (player2.aniState == PIKAIDLE) {
        player2.curFrame = powerUp;
        player2.aniCounter = 0;
        player2.aniState = player2.prevAniState;
    } else {
        player2.aniCounter++;
    }
}