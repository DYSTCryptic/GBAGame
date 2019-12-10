#include "myLib.h"
#include "levelOne.h"
#include "collisionmap.h"
#include "sound.h"
#include "oof.h"

// Variables
int hOff;
int vOff;
OBJ_ATTR shadowOAM[128];
ANISPRITE player;
ANISPRITE row1[5];
ANISPRITE row2[5];
ANISPRITE row3[5];
ANISPRITE boulder;
// ANISPRITE enemyboulder;
// ANISPRITE lives[3];
int playerLives;
int remaining;
int boulderTimer;
int level1SpdMod;
int powerUp;

// player animation states for aniState
enum {PIKAFRONT, PIKABACK, PIKARIGHT, PIKALEFT, PIKAIDLE};

// Initialize the game
void initLevelOne() {

	// Place screen on map
    vOff = 96;
    hOff = 9;

    remaining = 15;

    powerUp = 0;
    level1SpdMod = 1;

    //initialize player for level one    
    player.width = 16;
    player.height = 16;
    player.rdel = 1;
    player.cdel = 1;

    player.screenRow = 20;
    player.screenCol = SCREENWIDTH/2-player.height/2;
    player.aniCounter = 0;
    player.curFrame = 0;
    player.numFrames = 3;
    player.aniState = PIKAFRONT;
    player.hide = 0;

    //initialize invaders for level one
    int row1spacing = 10;
    int row2spacing = 10;
    int row3spacing = 10;
    for (int i = 0; i < 5; i++) {
        row1[i].width = 16;
        row1[i].height = 16;
        row1[i].cdel = 1;
        row1[i].rdel = 5;
        row1[i].screenRow = SCREENHEIGHT - 48;
        row1[i].screenCol = row1spacing;
        row1[i].curFrame = 0;
        row1[i].aniState = 4;
        row1[i].hide = 0;
        row1[i].numFrames = 3;
        row1[i].aniCounter = 0;
        row1spacing += 40;
    }

    for (int i = 0; i < 5; i++) {
        row2[i].width = 16;
        row2[i].height = 16;
        row2[i].cdel = 1;
        row2[i].rdel = 5;
        row2[i].screenRow = SCREENHEIGHT - 32;
        row2[i].screenCol = row2spacing;
        row2[i].curFrame = 0;
        row2[i].aniState = 5;
        row2[i].hide = 0;
        row2[i].numFrames = 3;
        row2[i].aniCounter = 0;
        row2spacing += 40;
    }

    for (int i = 0; i < 5; i++) {
        row3[i].width = 16;
        row3[i].height = 16;
        row3[i].cdel = 1;
        row3[i].rdel = 5;
        row3[i].screenRow = SCREENHEIGHT - 16;
        row3[i].screenCol = row3spacing;
        row3[i].curFrame = 0;
        row3[i].aniState = 6;
        row3[i].hide = 0;
        row3[i].numFrames = 3;
        row3[i].aniCounter = 0;
        row3spacing += 40;
    }

    //init player projectile
    boulder.width = 16;
        boulder.height = 16;
        boulder.rdel = 1;

        boulder.screenRow = -boulder.height;
		boulder.screenCol = 0;
        boulder.aniState = 0;
        boulder.curFrame = 3;
        boulder.hide = 1;
}

// Updates the game each frame
void updateGame() {

	updatePlayer();
    for (int i = 0; i < 5; i++) {
        updateInvaders(&row1[i]);
    }
    for (int i = 0; i < 5; i++) {
        updateInvaders(&row2[i]);
    }
    for (int i = 0; i < 5; i++) {
        updateInvaders(&row3[i]);
    }
    updateboulder(&boulder);
}

// Draws the game each frame
void drawGame() {

    drawPlayer();
    drawInvaders();
    drawboulder();

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);

    REG_BG0HOFF = hOff;
    REG_BG0VOFF = vOff;
}




//Determine whether or not to draw the enemies
void drawInvaders() {
    for (int i = 0; i < 5; i++) {
        if (row1[i].hide) {
        shadowOAM[0 + i + 1].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[0 + i + 1].attr0 = (ROWMASK & row1[i].screenRow) | ATTR0_SQUARE;
        shadowOAM[0 + i + 1].attr1 = (COLMASK & row1[i].screenCol) | ATTR1_SMALL;
        shadowOAM[0 + i + 1].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(row1[i].aniState * 2, row1[i].curFrame * 2);
    }
    }
    for (int i = 0; i < 5; i++) {
        if (row2[i].hide) {
        shadowOAM[0 + i  + 6].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[0 + i  + 6].attr0 = (ROWMASK & row2[i].screenRow) | ATTR0_SQUARE;
        shadowOAM[0 + i  + 6].attr1 = (COLMASK & row2[i].screenCol) | ATTR1_SMALL;
        shadowOAM[0 + i  + 6].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(row2[i].aniState * 2, row2[i].curFrame * 2);
    }
    }
    for (int i = 0; i < 5; i++) {
        if (row3[i].hide) {
        shadowOAM[0 + i  + 11].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[0 + i  + 11].attr0 = (ROWMASK & row3[i].screenRow) | ATTR0_SQUARE;
        shadowOAM[0 + i  + 11].attr1 = (COLMASK & row3[i].screenCol) | ATTR1_SMALL;
        shadowOAM[0 + i  + 11].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(row3[i].aniState * 2, row3[i].curFrame * 2);
    }
    }
}

//Update enemies each frame
void updateInvaders(ANISPRITE* e) {
    for (int i = 0; i < 5; i++) {
        if (!e->hide) {
            e->screenCol += e->cdel;
            if(e->screenCol == 0 || e-> screenCol == SCREENWIDTH - 10) {
                e->cdel *= -1;
                e->screenRow -= e->rdel;
            }

            if(collision(e->screenCol, e->screenRow, e->width, e->height, player.screenCol, player.screenRow, player.width, player.height)) {
                player.hide = 1;
            }

            if(!boulder.hide && collision(e->screenCol, e->screenRow, e->width, e->height, boulder.screenCol, boulder.screenRow, boulder.width, boulder.height)) {
                e->hide = 1;
                remaining--;
                boulder.hide = 1;
                playSoundB(oof, OOFLEN, OOFFREQ, 0);
            }
        }
    }

    
    animateEnemy(e);

}

void animateEnemy(ANISPRITE *e) {
    e->aniCounter++;
    // Change the animation frame every 20 frames of gameplay
    if(e->aniCounter % 20 == 0) {
        e->curFrame = (e->curFrame + 1) % e->numFrames;
    }
}

void drawboulder() {

    if (boulder.hide) {
        shadowOAM[16].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[16].attr0 = (ROWMASK & boulder.screenRow) | ATTR0_SQUARE;
        shadowOAM[16].attr1 = (COLMASK & boulder.screenCol) | ATTR1_SMALL;
        shadowOAM[16].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(boulder.aniState * 2, boulder.curFrame * 2);
    }
}

void updateboulder(ANISPRITE* b) {

	// If active, update; otherwise ignore
	if (!b->hide) {
		if (b->screenRow + b->height-1 >= 0 && b->screenRow < SCREENHEIGHT
            && b->screenCol + b->cdel > 0
            && b->screenCol + b->cdel < SCREENWIDTH-1) {

			b->screenRow += b->rdel * level1SpdMod;
            
		} else {
			b->hide = 1;
		}
	}
}

// Spawn a boulder
void fireboulder() {

		if (boulder.hide) {

			// Position the new boulder
			boulder.screenRow = player.screenRow;
			boulder.screenCol = player.screenCol + player.width/2
				- boulder.width/2;

			// Take the boulder out of the pool
			boulder.hide = 0;			
		} else {
            boulder.hide = 1;
            // Position the new boulder
			boulder.screenRow = player.screenRow;
			boulder.screenCol = player.screenCol + player.width/2
				- boulder.width/2;

			// Take the boulder out of the pool
            boulder.hide = 0;
        }
	
}


// Handle every-frame actions of the player
void updatePlayer() {

    if(BUTTON_HELD(BUTTON_LEFT)) {
        if (player.screenCol > 0) {

            // Update player's screen position if the above is true
            player.screenCol -= player.cdel * level1SpdMod;
            hOff--;
            
        }
    }
    if(BUTTON_HELD(BUTTON_RIGHT)) {
        if (player.screenCol + player.width < SCREENWIDTH) {

            // Update player's world position if the above is true
            player.screenCol += player.cdel * level1SpdMod;
            hOff++;
        }
    }

    if (BUTTON_PRESSED(BUTTON_A)) {
        fireboulder();
    }

    if (BUTTON_PRESSED(BUTTON_B) && level1SpdMod < 2) {
        level1SpdMod *= 2;
        powerUp = 5;
        player.curFrame = powerUp;
        boulder.aniState = 2;
    }


    

    animatePlayer();
}
// Handle player animation states
void animatePlayer() {

    // Set previous state to current state
    player.prevAniState = player.aniState;
    player.aniState = PIKAIDLE;

    // Change the animation frame every 20 frames of gameplay
    if(player.aniCounter % 20 == 0) {
        if (player.curFrame < player.numFrames - 1) {
        player.curFrame = (player.curFrame + 1);
        } else {
            player.curFrame = powerUp;
        }
    }

    // Control movement and change animation state
    if(BUTTON_HELD(BUTTON_UP))
        player.aniState = PIKABACK;
    if(BUTTON_HELD(BUTTON_DOWN))
        player.aniState = PIKAFRONT;
    if(BUTTON_HELD(BUTTON_LEFT))
        player.aniState = PIKALEFT;
    if(BUTTON_HELD(BUTTON_RIGHT))
        player.aniState = PIKARIGHT;

    // If the player aniState is idle, frame is player standing
    if (player.aniState == PIKAIDLE) {
        player.curFrame = powerUp;
        player.aniCounter = 0;
        player.aniState = player.prevAniState;
    } else {
        player.aniCounter++;
    }
}

// Draw the player
void drawPlayer() {

    if (player.hide) {
        shadowOAM[0].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[0].attr0 = (ROWMASK & player.screenRow) | ATTR0_SQUARE;
        shadowOAM[0].attr1 = (COLMASK & player.screenCol) | ATTR1_SMALL;
        shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(player.aniState * 2, player.curFrame * 2);
    }
}