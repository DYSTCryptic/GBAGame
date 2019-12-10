#include "myLib.h"
//Spritesheet
#include "spritesheet.h"
//Help Screens
#include "levelOneHelpScreen.h"
#include "levelTwoHelpScreen.h"
#include "levelThreeHelpScreen.h"
//Levels
#include "levelOne.h"
#include "levelTwo.h"
#include "levelThree.h"
//Splash Screens/ Backgrounds
#include "pilgrimbg.h"
#include "titlescreen.h"
#include "pausescreen.h"
#include "winscreen.h"
#include "losescreen.h"
#include "pilgrimlevel1.h"
#include "ralphbg.h"
#include "citybg.h"
//Sound stuff
#include "sound.h"
//Music
#include "startScreenMusic.h"
#include "bgMusic.h"
#include "victory.h"
#include "transition.h"
//Text
#include "text.h"

// Prototypes
void initialize();
void goToLevelOne();
void levelOne();
void goToLevelTwo();
void levelTwo();

//seed
int seed;

// States
enum {LEVELONEHELP, LEVELONE, LEVELTWOHELP, LEVELTWO, LEVELTHREEHELP, LEVELTHREE, START, WIN, LOSE, PAUSE};
int state;
int currLevel;

// Button Variables
unsigned short buttons;
unsigned short oldButtons;

int main() {

    initialize();

    setupSounds();
	setupInterrupts();

    
    while(1) {

        // Update button variables
        oldButtons = buttons;
        buttons = BUTTONS;
                
        // State Machine
        switch(state) {
            case LEVELONEHELP:
                levelOneHelp();
                break;
            case LEVELONE:
                levelOne();
                break;
            case LEVELTWOHELP:
                levelTwoHelp();
                break;
            case LEVELTWO:
                levelTwo();
                break;
            case LEVELTHREEHELP:
                levelThreeHelp();
                break;
            case LEVELTHREE:
                levelThree();
                break;
            case PAUSE:
                pause();
                break;
            case START:
                start();
                break;
            case LOSE:
                lose();
                break;
            case WIN:
                win();
                break;
        }
               
    }
}

// Sets up GBA
void initialize() {


    // Set up the first state
    goToStart();

    
    
}

void goToLevelOneHelp() {
    
    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, levelOneHelpScreenPal, PALETTE, levelOneHelpScreenPalLen / 2);

    drawFullscreenImage4(levelOneHelpScreenBitmap);

    waitForVBlank();
    flipPage();

    state = LEVELONEHELP;
}

void levelOneHelp() {
    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(transition, TRANSITIONLEN, TRANSITIONFREQ, 0);
        flipPage();
        goToLevelOne();
    }
}

// Sets up the game state
void goToLevelOne() {

    // Make sure changes do not appear onscreen
    waitForVBlank();

    DMANow(3, citybgPal, PALETTE, 32);
    DMANow(3, citybgTiles, &CHARBLOCK[1], citybgTilesLen/2);
    DMANow(3, citybgMap, &SCREENBLOCK[30], citybgMapLen/2);
    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;
    REG_BG1CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_SIZE_SMALL;


    // DMANow(3, ralphbgPal, PALETTE, 256);
    DMANow(3, ralphbgTiles, &CHARBLOCK[0], ralphbgTilesLen/2);
    DMANow(3, ralphbgMap, &SCREENBLOCK[28], ralphbgMapLen/2);
    
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    // Set up the sprites
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, spritesheetPal, SPRITEPALETTE, 512 /2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128);



    // Set up the display
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE;

    

    state = LEVELONE;
    currLevel = 1;
}
void goToLevelTwoHelp() {
    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, levelTwoHelpScreenPal, PALETTE, levelTwoHelpScreenPalLen / 2);

    drawFullscreenImage4(levelTwoHelpScreenBitmap);

    waitForVBlank();
    flipPage();

    state = LEVELTWOHELP;
}

void levelTwoHelp() {
    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(transition, TRANSITIONLEN, TRANSITIONFREQ, 0);
        flipPage();
        initLevelTwo();
        goToLevelTwo();
    }
}


void goToLevelTwo() {
    waitForVBlank();

    DMANow(3, citybgPal, PALETTE, 256);
    DMANow(3, citybgTiles, &CHARBLOCK[0], citybgTilesLen/2);
    DMANow(3, citybgMap, &SCREENBLOCK[28], citybgMapLen/2);
    REG_BG3VOFF = vOff;
    REG_BG3HOFF = hOff;
    REG_BG3CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    // Set up the sprites
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, spritesheetPal, SPRITEPALETTE, 512 /2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128);

    // Set up the display
    REG_DISPCTL = MODE0 | BG3_ENABLE | SPRITE_ENABLE;

    

    state = LEVELTWO;
    currLevel = 2;    
}
void goToLevelThreeHelp() {
    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, levelThreeHelpScreenPal, PALETTE, levelThreeHelpScreenPalLen / 2);

    drawFullscreenImage4(levelThreeHelpScreenBitmap);

    waitForVBlank();
    flipPage();

    state = LEVELTHREEHELP;
}
void levelThreeHelp() {
    if (BUTTON_PRESSED(BUTTON_A)) {
        playSoundB(transition, TRANSITIONLEN,TRANSITIONFREQ, 0);
        flipPage();
        initLevelThree();
        goToLevelThree();
    }
}

void goToLevelThree() {
    waitForVBlank();

    DMANow(3, pilgrimbgPal, PALETTE, 256);
    DMANow(3, pilgrimbgTiles, &CHARBLOCK[0], pilgrimbgTilesLen/2);
    DMANow(3, pilgrimbgMap, &SCREENBLOCK[28], pilgrimbgMapLen/2);
    REG_BG3VOFF = vOff;
    REG_BG3HOFF = hOff;
    REG_BG3CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_SIZE_SMALL;

    // Set up the sprites
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, spritesheetPal, SPRITEPALETTE, 512 /2);
    hideSprites();
    DMANow(3, shadowOAM, OAM, 128);

    // Set up the display
    REG_DISPCTL = MODE0 | BG3_ENABLE | SPRITE_ENABLE;



    

    state = LEVELTHREE;
    currLevel = 3;    
}

void levelTwo() {

    updateLevelTwo();
    drawLevelTwo();

    if (bouncesRemaining == 0) {
        // stopSound();
        // initLevelThree();
        flipPage();
        goToLevelThreeHelp();
        // goToWin();
    }
    
    if (BUTTON_PRESSED(BUTTON_START)) {
        pauseSound();
        flipPage();
        goToPause();
    }
    if (player2.hide == 1) {
        flipPage();
        goToLose();
    }
}

void levelThree() {

    updateLevelThree();
    drawLevelThree();

    
    if (player3.hide) {
        flipPage();
        goToLose();
    }
    if (enemyHealth == 0) {
        stopSound();
        flipPage();
        goToWin();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        flipPage();
        goToPause();
    }
}

void pause() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        unpauseSound();
        switch (currLevel) {
            case 1:
            goToLevelOne();
            break;
            case 2:
            goToLevelTwo();
            break;
            case 3:
            goToLevelThree();
            break;

        }
    }
}

void goToPause() {
    
    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, pausescreenPal, PALETTE, pausescreenPalLen / 2);

    drawFullscreenImage4(pausescreenBitmap);

    waitForVBlank();
    flipPage();

    state = PAUSE;

}

// Runs every frame of the game state
void levelOne() {

    updateGame();
    drawGame();

    if(BUTTON_PRESSED(BUTTON_START)) {
        pauseSound();
        flipPage();
        goToPause();
    }
    if(remaining == 0) {
        // initLevelTwo();
        flipPage();
        goToLevelTwoHelp();
    }
    if(player.hide == 1) {
        stopSound();
        flipPage();
        goToLose();
    }
}
void lose() {

    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        
        switch (currLevel) {
            case 1:
            initLevelOne();
            goToLevelOne();
            break;
            case 2:
            initLevelTwo();
            goToLevelTwo();
            break;
            case 3:
            initLevelThree();
            goToLevelThree();
            break;
        }
    }
}

void goToLose() {
    

    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, losescreenPal, PALETTE, pausescreenPalLen / 2);

    drawFullscreenImage4(losescreenBitmap);

    waitForVBlank();
    flipPage();

    state = LOSE;
    
}

void start() {
    waitForVBlank();
    seed++;

    if (BUTTON_PRESSED(BUTTON_START)) {
        srand(seed);
        stopSound();

        playSoundA(bgMusic, BGMUSICLEN, BGMUSICFREQ, 1);
        initLevelOne();
        flipPage();
        goToLevelOneHelp();

    }
}

void goToStart() {

    playSoundA(startScreenMusic, STARTSCREENMUSICLEN, STARTSCREENMUSICFREQ, 1);

    
    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, titlescreenPal, PALETTE, titlescreenPalLen / 2);

    drawFullscreenImage4(titlescreenBitmap);

    waitForVBlank();
    flipPage();
    
    seed = 0;
    
    state = START;
}
void win() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        stopSound();
        initLevelOne();
        flipPage();
        goToStart();
    }
}

void goToWin() {

    playSoundA(victory, VICTORYLEN, VICTORYFREQ, 1);

    REG_DISPCTL = MODE4 | BG2_ENABLE | DISP_BACKBUFFER;
    
    DMANow(3, winscreenPal, PALETTE, pausescreenPalLen / 2);

    drawFullscreenImage4(winscreenBitmap);

    waitForVBlank();
    flipPage();

    state = WIN;
    
}
