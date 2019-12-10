#include "myLib.h"
#include "levelThree.h"
#include "sound.h"
#include "oof.h"


// Variables
int hOff;
int vOff;
OBJ_ATTR shadowOAM[128];
ANISPRITE boulder;
ANISPRITE player3;
ANISPRITE gideon;
ANISPRITE bullet1;
ANISPRITE bullet2;
ANISPRITE bullet3;
int enemyHealth;
int damageTimer;
int speedMod = 2;
int level3SpdMod;
int powerUp;

// player3 animation states for aniState
enum {PIKAFRONT, PIKABACK, PIKARIGHT, PIKALEFT, PIKAIDLE};

void initLevelThree() {
    enemyHealth = 7;
    damageTimer = 0;
    level3SpdMod = 1;
    powerUp = 0;

    //initialize player3 for level two    
    player3.width = 16;
    player3.height = 16;
    player3.rdel = 1;
    player3.cdel = 1;

    player3.screenRow = SCREENHEIGHT/2-player3.width/2;
    player3.screenCol = 20;
    player3.aniCounter = 0;
    player3.curFrame = 0;
    player3.numFrames = 3;
    player3.aniState = PIKAFRONT;
    player3.hide = 0;

    //Initialize Gideon
    gideon.width = 64;
    gideon.height = 64;
    gideon.rdel = 1;

    gideon.screenRow = SCREENHEIGHT/2-gideon.width/2;
    gideon.screenCol = 100;
    gideon.aniCounter = 0;
    gideon.curFrame = 0;
    gideon.numFrames = 2;
    gideon.aniState = 7;
    gideon.hide = 0;

    //init player projectile
    boulder.width = 16;
    boulder.height = 16;
    boulder.cdel = 1;

    boulder.screenRow = -boulder.height;
    boulder.screenCol = 0;
    boulder.aniState = 0;
    boulder.curFrame = 3;
    boulder.hide = 1;

    //init Enemy Bullet 1
    bullet1.width = 16;
    bullet1.height = 16;
    bullet1.rdel = 1;
    bullet1.cdel = 1;

    bullet1.screenRow = -bullet1.height;
    bullet1.screenCol = 0;
    bullet1.aniState = 11;
    bullet1.curFrame = 0;
    bullet1.hide = 1;

    //init Enemy Bullet 2
    bullet2.width = 16;
    bullet2.height = 16;
    bullet2.rdel = 1;
    bullet2.cdel = 1;

    bullet2.screenRow = -bullet1.height;
    bullet2.screenCol = 0;
    bullet2.aniState = 11;
    bullet2.curFrame = 1;
    bullet2.hide = 1;

    //init Enemy Bullet 3
    bullet3.width = 16;
    bullet3.height = 16;
    bullet3.rdel = 1;
    bullet3.cdel = 1;

    bullet3.screenRow = -bullet1.height;
    bullet3.screenCol = 0;
    bullet3.aniState = 11;
    bullet3.curFrame = 2;
    bullet3.hide = 1;
}

void updateLevelThree() {
    // Update player3
    if (BUTTON_HELD(BUTTON_RIGHT) && player3.screenCol + player3.width < SCREENWIDTH/2 - 10) {
        player3.screenCol+= player3.cdel * level3SpdMod;
    }
    if (BUTTON_HELD(BUTTON_LEFT) && player3.screenCol > 0) {
        player3.screenCol-= player3.cdel * level3SpdMod;
    }
    if (BUTTON_HELD(BUTTON_UP) && player3.screenRow > 0) {
        player3.screenRow-= player3.rdel * level3SpdMod;
    }
    if (BUTTON_HELD(BUTTON_DOWN) && player3.screenRow + player3.height < SCREENHEIGHT - 10) {
        player3.screenRow+= player3.rdel * level3SpdMod;
    }
    if (BUTTON_PRESSED(BUTTON_A)) {
        throwBoulder();
    }
    if (BUTTON_PRESSED(BUTTON_B) && level3SpdMod < 2) {
        level3SpdMod *= 2;
        player3.curFrame = powerUp;
        powerUp = 5;
        boulder.aniState = 2;
    }
    if (!gideon.hide){
        gideon.screenRow += gideon.rdel;
        if (gideon.screenRow < 0 || gideon.screenRow + gideon.height > SCREENHEIGHT) {
            gideon.rdel *= -1;
        }
        if (!boulder.hide) {
            if (collision(boulder.screenCol, boulder.screenRow, 16, 16, gideon.screenCol, gideon.screenRow, 64, 64) && damageTimer % 50 == 0) {
            enemyHealth--;
            boulder.hide = 1;
            gideon.curFrame = 4;
            if (damageTimer % 50 == 0) {
                gideon.curFrame = 0;
            }
            playSoundB(oof, OOFLEN, OOFFREQ, 0);
            }
        }
    }

    if(bullet1.hide && bullet2.hide && bullet3.hide) {
        fireBullets();
    }

    if (!bullet1.hide) {
        if (bullet1.screenRow + bullet1.height-1 >= 0 && bullet1.screenRow < SCREENHEIGHT
            && bullet1.screenCol + bullet1.cdel > 0
            && bullet1.screenCol + bullet1.cdel < SCREENWIDTH-1) {
                bullet1.screenCol -= bullet1.cdel * speedMod;
                bullet1.screenRow -= bullet1.rdel * speedMod;
                if(collision(bullet1.screenCol, bullet1.screenRow, 16, 16, player3.screenCol, player3.screenRow, 16, 16)) {
            player3.hide = 1;
        }
        } else {
            bullet1.hide = 1;
        }
        
    }

    if (!bullet2.hide) {
        if (bullet2.screenRow + bullet2.height-1 >= 0 && bullet2.screenRow < SCREENHEIGHT
            && bullet2.screenCol + bullet2.cdel > 0
            && bullet2.screenCol + bullet2.cdel < SCREENWIDTH-1) {
                bullet2.screenCol -= bullet2.cdel * speedMod;
                // bullet2.screenRow -= bullet2.rdel;
            if(collision(bullet2.screenCol, bullet2.screenRow, 16, 16, player3.screenCol, player3.screenRow, 16, 16)) {
            player3.hide = 1;
        }
        } else {
            bullet2.hide = 1;
        }
        
    }

    if (!bullet3.hide) {
        if (bullet3.screenRow + bullet3.height-1 >= 0 && bullet3.screenRow < SCREENHEIGHT
            && bullet3.screenCol + bullet3.cdel > 0
            && bullet3.screenCol + bullet3.cdel < SCREENWIDTH-1) {
                bullet3.screenCol -= bullet3.cdel * speedMod;
                bullet3.screenRow += bullet3.rdel * speedMod;
            if(collision(bullet3.screenCol, bullet3.screenRow, 16, 16, player3.screenCol, player3.screenRow, 16, 16)) {
            player3.hide = 1;
        }
        } else {
            bullet3.hide = 1;
        }
        
    }

    // If active, update Boulder; otherwise ignore
	if (!boulder.hide) {
		if (boulder.screenRow + boulder.height-1 >= 0 && boulder.screenRow < SCREENHEIGHT
            && boulder.screenCol + boulder.cdel > 0
            && boulder.screenCol + boulder.cdel < SCREENWIDTH-1) {

			boulder.screenCol += boulder.cdel * level3SpdMod;
            
		} else {
			boulder.hide = 1;
		}
	}

    if (!bullet1.hide) {
        if (collision(bullet1.screenCol, bullet1.screenRow, 16, 16, player3.screenCol, player3.screenRow, 16, 16)) {
            player3.hide = 1;
        }
    } else {
        bullet1.hide = 1;
    }
    if (!bullet2.hide) {
        if (collision(bullet2.screenCol, bullet2.screenRow, 16, 16, player3.screenCol, player3.screenRow, 16, 16)) {
            player3.hide = 1;
        }
    } else {
        bullet2.hide = 1;
    }
    if (!bullet3.hide) {
        if (collision(bullet3.screenCol, bullet3.screenRow, 16, 16, player3.screenCol, player3.screenRow, 16, 16)) {
            player3.hide = 1;
        }
    } else {
        bullet3.hide = 1;
    }
    animatePlayer3();
    damageTimer++;

}

// Handle player animation states
void animatePlayer3() {

    // Set previous state to current state
    player3.prevAniState = player3.aniState;
    player3.aniState = PIKAIDLE;

    // Change the animation frame every 20 frames of gameplay
    if(player3.aniCounter % 20 == 0) {
        if (player3.curFrame < player3.numFrames - 1) {
        player3.curFrame = (player3.curFrame + 1) ;
        } else {
            player3.curFrame = powerUp;
        }
    }

    // Control movement and change animation state
    if(BUTTON_HELD(BUTTON_UP))
        player3.aniState = PIKABACK;
    if(BUTTON_HELD(BUTTON_DOWN))
        player3.aniState = PIKAFRONT;
    if(BUTTON_HELD(BUTTON_LEFT))
        player3.aniState = PIKALEFT;
    if(BUTTON_HELD(BUTTON_RIGHT))
        player3.aniState = PIKARIGHT;

    // If the player aniState is idle, frame is player standing
    if (player3.aniState == PIKAIDLE) {
        player3.curFrame = powerUp;
        player3.aniCounter = 0;
        player3.aniState = player3.prevAniState;
    } else {
        player3.aniCounter++;
    }
}
void drawLevelThree () {
    if (player3.hide) {
        shadowOAM[0].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[0].attr0 = (ROWMASK & player3.screenRow) | ATTR0_SQUARE;
        shadowOAM[0].attr1 = (COLMASK & player3.screenCol) | ATTR1_SMALL;
        shadowOAM[0].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(player3.aniState * 2, player3.curFrame * 2);
    }
    if (gideon.hide) {
        shadowOAM[1].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[1].attr0 = (ROWMASK & gideon.screenRow) | ATTR0_SQUARE;
        shadowOAM[1].attr1 = (COLMASK & gideon.screenCol) | ATTR1_LARGE;
        shadowOAM[1].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(gideon.aniState * 2, gideon.curFrame * 2);
    }
    if (boulder.hide) {
        shadowOAM[2].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[2].attr0 = (ROWMASK & boulder.screenRow) | ATTR0_SQUARE;
        shadowOAM[2].attr1 = (COLMASK & boulder.screenCol) | ATTR1_SMALL;
        shadowOAM[2].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(boulder.aniState * 2, boulder.curFrame * 2);
    }
    if (bullet1.hide) {
        shadowOAM[3].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[3].attr0 = (ROWMASK & bullet1.screenRow) | ATTR0_SQUARE;
        shadowOAM[3].attr1 = (COLMASK & bullet1.screenCol) | ATTR1_SMALL;
        shadowOAM[3].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(bullet1.aniState * 2, bullet1.curFrame * 2);
    }
    if (bullet2.hide) {
        shadowOAM[4].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[4].attr0 = (ROWMASK & bullet2.screenRow) | ATTR0_SQUARE;
        shadowOAM[4].attr1 = (COLMASK & bullet2.screenCol) | ATTR1_SMALL;
        shadowOAM[4].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(bullet2.aniState * 2, bullet2.curFrame * 2);
    }
    if (bullet3.hide) {
        shadowOAM[5].attr0 |= ATTR0_HIDE;
    } else {
        shadowOAM[5].attr0 = (ROWMASK & bullet3.screenRow) | ATTR0_SQUARE;
        shadowOAM[5].attr1 = (COLMASK & bullet3.screenCol) | ATTR1_SMALL;
        shadowOAM[5].attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(bullet3.aniState * 2, bullet3.curFrame * 2);
    }

    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*4);
}

void throwBoulder() {
    if (boulder.hide) {

			// Position the new boulder
			boulder.screenRow = player3.screenRow;
			boulder.screenCol = player3.screenCol + player3.width/2
				- boulder.width/2;

			// Take the boulder out of the pool
			boulder.hide = 0;			
		} else {
            boulder.hide = 1;
            // Position the new boulder
			boulder.screenRow = player3.screenRow;
			boulder.screenCol = player3.screenCol + player3.width/2
				- boulder.width/2;

			// Take the boulder out of the pool
            boulder.hide = 0;
        }
}

void fireBullets() {
    if (bullet1.hide) {
        bullet1.screenRow = gideon.screenRow;
        bullet1.screenCol = gideon.screenCol + gideon.width/2 - bullet1.width/2;

        bullet1.hide = 0;
    }
    if (bullet2.hide) {
        bullet2.screenRow = gideon.screenRow;
        bullet2.screenCol = gideon.screenCol + gideon.width/2 - bullet2.width/2;

        bullet2.hide = 0;
    }
    if (bullet3.hide) {
        bullet3.screenRow = gideon.screenRow;
        bullet3.screenCol = gideon.screenCol + gideon.width/2 - bullet3.width/2;

        bullet3.hide = 0;
    }
}