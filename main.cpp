#include "Pokitto.h"
#include "gfx.h"

Pokitto::Core game;


  /*************************************************************/
 /*** I use my own control reading, I find it more accurate ***/
/*************************************************************/
#define HELD 0
#define NEW 1
#define RELEASE 2
byte CompletePad, ExPad, TempPad, myPad;
bool _A[3], _B[3], _C[3], _Up[3], _Down[3], _Left[3], _Right[3];

DigitalIn _aPin(P1_9);
DigitalIn _bPin(P1_4);
DigitalIn _cPin(P1_10);
DigitalIn _upPin(P1_13);
DigitalIn _downPin(P1_3);
DigitalIn _leftPin(P1_25);
DigitalIn _rightPin(P1_7);

void UPDATEPAD(int pad, int var) {
  _C[pad] = (var >> 1)&1;
  _B[pad] = (var >> 2)&1;
  _A[pad] = (var >> 3)&1;
  _Down[pad] = (var >> 4)&1;
  _Left[pad] = (var >> 5)&1;
  _Right[pad] = (var >> 6)&1;
  _Up[pad] = (var >> 7)&1;
}
void UpdatePad(int joy_code){
  ExPad = CompletePad;
  CompletePad = joy_code;
  UPDATEPAD(HELD, CompletePad); // held
  UPDATEPAD(RELEASE, (ExPad & (~CompletePad))); // released
  UPDATEPAD(NEW, (CompletePad & (~ExPad))); // newpress
}
byte updateButtons(byte var){
   var = 0;
   if (_cPin) var |= (1<<1);
   if (_bPin) var |= (1<<2);
   if (_aPin) var |= (1<<3); // P1_9 = A
   if (_downPin) var |= (1<<4);
   if (_leftPin) var |= (1<<5);
   if (_rightPin) var |= (1<<6);
   if (_upPin) var |= (1<<7);

   return var;
}
/**************************************************************************/


typedef struct {
  int invincible;
  float x;
  float vx;
  float y;
  float vy;
  byte h;
  boolean flip;
  boolean big;
  boolean crouch;
} Char;
Char player;

int levelNum=0;
//int maxLevels=sizeof(levels)/102;
int tileSize=12;
uint32_t frameNumber=0;
byte gameMode;

uint32_t myDelay;
uint32_t tempTime;



void setup(){
    game.begin();
    game.display.width = 110; // half size
    game.display.height = 86;

    game.display.clear();
    game.display.setColorDepth(4);
    game.display.load565Palette(sprite_pal);
    game.display.bgcolor = 13;
    game.display.invisiblecolor = 13;
    game.sound.ampEnable(true);
    game.sound.playMusicStream();

    tempTime = game.getTime();
    myDelay = 120;
}


int main(){
    setup();

    int pFrame=0;

    while (game.isRunning()) {

            myPad = updateButtons(myPad);
            UpdatePad(myPad);

            frameNumber++;
            char oldMode = gameMode;
            switch(gameMode){
                case 0:
//                    drawTitleScreen();
                    break;
                case 20:
                    // play levels
                    myDelay = 30;
//                    playLevel();
                    break;
            }


            game.display.drawBitmap(0,0,sprites[pFrame]);
            game.display.update();
            if(pFrame++==5)pFrame=0;

        // timing loop
        while(game.getTime()-tempTime < myDelay){
            //refreshDisplay();
        };
        tempTime = game.getTime();
  }
    return 1;
}
