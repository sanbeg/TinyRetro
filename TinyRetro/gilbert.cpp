//        >>>>>  T-I-N-Y  G-I-L-B-E-R-T  for ATTINY85  GPL v3 <<<<
//                     Copyright (c) Daniel C 2017
//             Contact EMAIL: electro_l.i.b@tinyjoypad.com
//           https://sites.google.com/view/arduino-collection

//  Tiny Gilbert is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//the code work at 16 MHZ internal
//and use ssd1306xled Library for SSD1306 oled display 128x64
#include "src/video/ssd1306xled.h"
#include "Beeper.h"
#include "Control.h"

namespace gilbert {

#include "gilbert_spritebank.h"

const uint8_t MAX_TIMER = 20; // was 60, controls key spinning effect.
const uint8_t TIMER_INC = 2;  // was 4, controls walking animation

uint8_t Map[8][34] = {{0}};
uint8_t timer = 0; // 0 -60, or less
int8_t scrool = 0;
int8_t step4 = 0; // -1 - 4 (then trimmed to 0 - 3
uint8_t MainAnim = 0; // 0 - 2 -> sprite frame
bool LorR = 1; // 0 - 1 -> sprite direction
int8_t Jump = 0;  // 0 - 3
bool  jumpcancel = 0; // 0 - 1
//const float VSlide[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
uint8_t key[20][2] = {{0}};
uint8_t keyS = 0;
float VSlideOut = 0;
//uint8_t LevelMult = 0; // could be local?
uint8_t levelType = 0; // current level 0 - 9 (10?)
uint8_t ByteMem = 0;
bool visible = 1; // 0 - 1
uint8_t injur = 0; // health -> 0 -30
uint8_t LIVE = 0; // lives -> 0 - 7

Beeper<> beeper;

float vslide(int n) {
  return (float) (1 << n);
}

void setup() {
  //_delay_ms(40);
  SSD1306.ssd1306_init();
  //_delay_ms(40);
  control::setup();
  beeper.setup();
}

uint8_t delKey(uint8_t Xin, uint8_t Yin) {
  for (int8_t x = 0; x < 23; x++) {
    if ((key[x][0] == 0) && (key[x][1] == 0)) {
      return 11;
    }
    if ((key[x][0] == Xin) && (key[x][1] == Yin)) {
      return 0;
    }
  }
  return 11;
}

void Sound(uint8_t freq, uint8_t dur) {
#if 0
  for (uint8_t t = 0; t < dur; t++) {
    if (freq != 0) PORTB = PORTB | 0b00010000;
    for (uint8_t t = 0; t < (255 - freq); t++) {
      _delay_us(1);
    }
    PORTB = PORTB & 0b11101111;
    for (uint8_t t = 0; t < (255 - freq); t++) {
      _delay_us(1);
    }
  }
#else
  beeper.beep(freq, dur);
#endif
}


void sound(uint8_t SND) {
  if (SND == 1) {
    Sound(210, 10);
    Sound(240, 2);
    Sound(180, 5);
  }
  if (SND == 2) {
    for (uint8_t x = 255; x > 2; x--) {
      Sound(x, 1);
    }
  }
}

/** Local prototypes **/
void ResetVar();
void ResetVarNextLevel();
void NextLevel();
void TinyMainShift(DriftSprite * DSprite);
void SpriteShiftInitialise(DriftSprite* DSprite);
void Tiny_Flip(DriftSprite* DSprite);
int8_t CollisionCheck(DriftSprite* DSprite);
void ScrollUpdate(DriftSprite* DSprite);
void UpdateVerticalSlide(DriftSprite* DSprite);
void GravityUpdate(DriftSprite* DSprite);
void JumpProcedure(DriftSprite* DSprite);

/*
   1 = left, 3 = right => PB5, A0
   2 =  up, 4 = down => PB3, A3
   5 = action, PB1, 1
*/

void loop() {
  //digitalWrite(3, LOW);
RESTARTGAME:
  sound(1); sound(2); sound(2); sound(1); sound(2); sound(2);
  _delay_ms(200);
  ResetVar();
  SSD1306.ssd1306_fillscreen(0xff);
  SSD1306.ssd1306_draw_bmp(32, 4, 96, 8, start);
  _delay_ms(400);
  while (1) {
    if (control::isPressed(control::BTN_A)) {
      sound(1);
      break;
    }
  }
RESTARTLEVEL:
  ResetVarNextLevel();
NEXTLEVEL:
  if (levelType > 9) {
    goto RESTARTGAME;
  }
  _delay_ms(400);
  DriftSprite MainSprite;
  SpriteShiftInitialise(&MainSprite);
  SSD1306.ssd1306_fillscreen(0x00);
#define RBACKUP  if  ((CollisionCheck(&MainSprite)==1)){MainSprite.x4decalage--;}
#define LBACKUP  if  ((CollisionCheck(&MainSprite)==1)) {MainSprite.x4decalage++;}
  while (1) {
    if (control::isPressed(control::BTN_R))  {
      if ((timer % TIMER_INC == 0) && (++MainAnim > 2)) {
        MainAnim = 0;
      }
      LorR = 0;
      MainSprite.x4decalage++;
      RBACKUP;
      if (MainSprite.x4decalage > 3) {
        MainSprite.x4decalage = 0;
        MainSprite.MainPositionOnGridH++;
      }
    }
    if (control::isPressed(control::BTN_L)) {
      if ((timer % TIMER_INC == 0) && (++MainAnim > 2)) {
        MainAnim = 0;
      }
      LorR = 1;
      MainSprite.x4decalage--;
      LBACKUP;
      if (MainSprite.x4decalage < 0) {
        MainSprite.x4decalage = 3;
        MainSprite.MainPositionOnGridH--;
      }
    }
    ScrollUpdate(&MainSprite);
#define exclude(Spick) (ByteMem==Spick)
#define SpritePickup (exclude(11))
    if (MainSprite.MainPositionOnGridV >= 7) {
      sound(2);
      LIVE--;
      if (LIVE == 0) {
        goto RESTARTGAME;
      }
      goto RESTARTLEVEL;
    }
    uint8_t LevelMult = 0;
    for (uint8_t x = 0; x < 33; x++) {
#define LevelS (x+scrool)/4 //(x+scrool)/4
      switch (levelType) {
        case (0): LevelMult = pgm_read_byte(&Level0[LevelS]); break;
        case (1): LevelMult = pgm_read_byte(&Level1[LevelS]); break;
        case (2): LevelMult = pgm_read_byte(&Level2[LevelS]); break;
        case (3): LevelMult = pgm_read_byte(&Level3[LevelS]); break;
        case (4): LevelMult = pgm_read_byte(&Level4[LevelS]); break;
        case (5): LevelMult = pgm_read_byte(&Level5[LevelS]); break;
        case (6): LevelMult = pgm_read_byte(&Level6[LevelS]); break;
        case (7): LevelMult = pgm_read_byte(&Level7[LevelS]); break;
        case (8): LevelMult = pgm_read_byte(&Level8[LevelS]); break;
        case (9): LevelMult = pgm_read_byte(&Level9[LevelS]); break;
        default: goto RESTARTGAME;
      }
#define LevelShift (((x+scrool)%4)+(LevelMult*4))
      ByteMem = pgm_read_byte(&map1couche2[LevelShift]);
      if ((SpritePickup)) {
        Map[1][x] = delKey(x + scrool, 1);
      } else {
        Map[1][x] = ByteMem;
      }
      ByteMem = pgm_read_byte(&map1couche3[LevelShift]);
      if ((SpritePickup)) {
        Map[2][x] = delKey(x + scrool, 2);
      } else {
        Map[2][x] = ByteMem;
      }
      ByteMem = pgm_read_byte(&map1couche4[LevelShift]);
      if ((SpritePickup)) {
        Map[3][x] = delKey(x + scrool, 3);
      } else {
        Map[3][x] = ByteMem;
      }
      ByteMem = pgm_read_byte(&map1couche5[LevelShift]);
      if ((SpritePickup)) {
        Map[4][x] = delKey(x + scrool, 4);
      } else {
        Map[4][x] = ByteMem;
      }
      ByteMem = pgm_read_byte(&map1couche6[LevelShift]);
      if ((SpritePickup)) {
        Map[5][x] = delKey(x + scrool, 5);
      } else {
        Map[5][x] = ByteMem;
      }
      ByteMem = pgm_read_byte(&map1couche7[LevelShift]);
      if ((SpritePickup)) {
        Map[6][x] = delKey(x + scrool, 6);
      } else {
        Map[6][x] = ByteMem;
      }
      ByteMem = pgm_read_byte(&map1couche8[LevelShift]);
      if ((SpritePickup)) {
        Map[7][x] = delKey(x + scrool, 7);
      } else {
        Map[7][x] = ByteMem;
      }
    }
    if (Jump == 0) {
      GravityUpdate(&MainSprite);
    }
    if (control::isPressed(control::BTN_A) && (Jump == 0) && (jumpcancel == 0) && (CollisionCheck(&MainSprite) == 0)) {
      if (MainSprite.y8decalage == 0) {
        Jump = 3;
      }
    }
    if (!control::isPressed(control::BTN_A)) {
      jumpcancel = 0;
    }
    if (Jump > 0) {
      JumpProcedure(&MainSprite);
    }
#define pickup(Vadd,Hadd,SPRITE) (Map[MainSprite.MainPositionOnGridV+Vadd][MainSprite.MainPositionOnGridH+Hadd]==SPRITE)
#define Pictup2(SP) ((pickup(0,0,SP))||(pickup(0,1,SP))||(pickup(0,2,SP)))
#define Pictup4(SP2) (Pictup2(SP2)||(pickup(1,0,SP2))||(pickup(1,1,SP2))||(pickup(1,2,SP2)))
    if ((MainSprite.y8decalage == 0)) {
      if ( pickup(0, 0, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH;
        keyS++;
        sound(1);
      }
      if ( pickup(0, 1, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH + 1;
        keyS++;
        sound(1);
      }
      if ( pickup(0, 2, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH + 2;
        keyS++;
        sound(1);
      }
      if ((Pictup2(8)) && (injur == 0)) {
        LIVE--;
        if (LIVE == 0) {
          goto RESTARTGAME;
        } Jump = 2;
        injur = 30;
        sound(2);
      }
      if (Pictup2(13) || Pictup2(14)) {
        if ((pgm_read_byte(&KeyinLevel[levelType]) == keyS) && (/*(Jump > 0) ||*/ control::isPressed(control::BTN_U))) {
          NextLevel();
          goto NEXTLEVEL;
        }
      }
      Map[MainSprite.MainPositionOnGridV][MainSprite.MainPositionOnGridH] = MainSprite.DriftGrid[0][0];
      Map[MainSprite.MainPositionOnGridV][MainSprite.MainPositionOnGridH + 1] = MainSprite.DriftGrid[0][1];
    } else {
      if ( pickup(0, 0, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH;
        keyS++;
        sound(1);
      }
      if ( pickup(0, 1, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH + 1;
        keyS++;
        sound(1);
      }
      if ( pickup(0, 2, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH + 2;
        keyS++;
        sound(1);
      }
      if ( pickup(1, 0, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV + 1;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH;
        keyS++;
        sound(1);
      }
      if ( pickup(1, 1, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV + 1;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH + 1;
        keyS++;
        sound(1);
      }
      if ( pickup(1, 2, 11)) {
        key[keyS][1] = MainSprite.MainPositionOnGridV + 1;
        key[keyS][0] = scrool + MainSprite.MainPositionOnGridH + 2;
        keyS++;
        sound(1);
      }
      if ((Pictup2(8)) && (injur == 0)) {
        LIVE--;
        if (LIVE == 0) {
          goto RESTARTGAME;
        } Jump = 2;
        injur = 30;
        sound(2);
      }
      if (Pictup4(13) || Pictup4(14)) {
        if ((pgm_read_byte(&KeyinLevel[levelType]) == keyS) && (/*(Jump > 0) ||*/ control::isPressed(control::BTN_U))) {
          NextLevel();
          goto NEXTLEVEL;
        }
      }
      Map[MainSprite.MainPositionOnGridV][MainSprite.MainPositionOnGridH] = MainSprite.DriftGrid[0][0];
      Map[MainSprite.MainPositionOnGridV][MainSprite.MainPositionOnGridH + 1] = MainSprite.DriftGrid[0][1];
      Map[MainSprite.MainPositionOnGridV + 1][MainSprite.MainPositionOnGridH] = MainSprite.DriftGrid[1][0];
      Map[MainSprite.MainPositionOnGridV + 1][MainSprite.MainPositionOnGridH + 1] = MainSprite.DriftGrid[1][1];
    }
    if (timer % 2 == 0)  {
      if (injur > 0) {
        if (visible == 1) {
          visible = 0;
        } else {
          visible = 1;
        } injur--;
      }
    }
    UpdateVerticalSlide(&MainSprite);
    Tiny_Flip(&MainSprite);
    timer++;
    if (timer > MAX_TIMER) {
      timer = 0;
    }
  }
}
////////////////////////////////// main end /////////////////////////////////

void ScrollUpdate(DriftSprite* DSprite) {
  if ((DSprite->MainPositionOnGridH) < 10) {
    if ((scrool > 0) && (step4 <= 3)) {
      step4 = step4 + 1;
    } if ((step4 > 3) && (scrool > 0)) {
      step4 = 0;
      scrool--;
      DSprite->MainPositionOnGridH = DSprite->MainPositionOnGridH + 1;
    }
  }
  if ((DSprite->MainPositionOnGridH) > 18) {
    step4 = step4 - 1;
    if (step4 < 0) {
      step4 = 3;
      scrool++;
      DSprite->MainPositionOnGridH = DSprite->MainPositionOnGridH - 1;
    }
  }
}

void UpdateVerticalSlide(DriftSprite* DSprite) {
  if (DSprite->y8decalage == 0) {
    if (visible == 1) {
      DSprite->DriftGrid[0][0] = 5;
      DSprite->DriftGrid[0][1] = 6;
    } else {
      DSprite->DriftGrid[0][0] = 0;
      DSprite->DriftGrid[0][1] = 0;
    }
  } else {
    if (visible == 1) {
      DSprite->DriftGrid[0][0] = 5;
      DSprite->DriftGrid[0][1] = 6;
      DSprite->DriftGrid[1][0] = 55;
      DSprite->DriftGrid[1][1] = 66;
    } else {
      DSprite->DriftGrid[0][0] = 0;
      DSprite->DriftGrid[0][1] = 0;
      DSprite->DriftGrid[1][0] = 0;
      DSprite->DriftGrid[1][1] = 0;
    }
  }
}

void GravityUpdate(DriftSprite* DSprite) {
  int8_t memy8decalage = DSprite->y8decalage;
  int8_t memMainPositionOnGridV = DSprite->MainPositionOnGridV;
  DSprite->y8decalage = DSprite->y8decalage + 2;
  if (DSprite->y8decalage > 7) {
    DSprite->y8decalage = 0;
    DSprite->MainPositionOnGridV++;
  }
  if (CollisionCheck(DSprite) >= 1) {
    DSprite->MainPositionOnGridV = memMainPositionOnGridV;
    DSprite->y8decalage = memy8decalage;
  }
}

void JumpProcedure(DriftSprite* DSprite) {
  int8_t memo2 = 0;
  if ((Jump > 0)) {
    memo2 = DSprite->MainPositionOnGridV;
    DSprite->y8decalage = DSprite->y8decalage - (Jump * 2);
    if (DSprite->y8decalage < 0)   {
      DSprite->y8decalage = 7; DSprite->MainPositionOnGridV--; Jump--;
      if ((DSprite->MainPositionOnGridV <= 0) || (jumpcancel == 1) || (CollisionCheck(DSprite) >= 1)) {
        Jump = 0;
        jumpcancel = 1;
        DSprite->y8decalage = 0;
        DSprite->MainPositionOnGridV = memo2;
        Jump = 0; jumpcancel = 1;
      }
      if (Jump == 0) {
        jumpcancel = 1;
      }
    }
  }
}

int8_t CollisionCheck(DriftSprite* DSprite) {
  int8_t xscan = 0, yscan = 0;
  //varable de la grille
#define MX DSprite->MainPositionOnGridH
#define MY DSprite->MainPositionOnGridV
#define MXDRIFT DSprite->x4decalage
#define MYDRIFT DSprite->y8decalage
#define x1 (MX*4)+MXDRIFT
#define y1 (MY*8)+MYDRIFT
#define x2 (MX*4)+MXDRIFT+7
#define y2 (MY*8)+MYDRIFT
#define x3 (MX*4)+MXDRIFT
#define y3 (MY*8)+MYDRIFT+7
#define x4 (MX*4)+MXDRIFT+7
#define y4 (MY*8)+MYDRIFT+7
#define bx1 (MX+xscan)*4
#define by1 (MY+yscan)*8
#define bx2 ((MX+xscan)*4)+3
#define by2 (MY+yscan)*8
#define bx3 (MX+xscan)*4
#define by3 ((MY+yscan)*8)+7
#define bx4 ((MX+xscan)*4)+3
#define by4 ((MY+yscan)*8)+7
#define NoTested ((Map[MY+yscan][MX+xscan]!=8)&&(Map[MY+yscan][MX+xscan]!=0)&&(Map[MY+yscan][MX+xscan]!=13)&&(Map[MY+yscan][MX+xscan]!=14)&&(Map[MY+yscan][MX+xscan]!=11)&&(Map[MY+yscan][MX+xscan]!=5)&&(Map[MY+yscan][MX+xscan]!=6)&&(Map[MY+yscan][MX+xscan]!=55)&&(Map[MY+yscan][MX+xscan]!=66))
  for (yscan = -1; yscan < 3; yscan++) {
    for (xscan = -1; xscan < 3; xscan++) {
      if (NoTested) {
        if ((x1 > bx2) || (x2 < bx1) || (y1 > by3) || (y3 < by1)) {
        } else {
          return 1;
        }
      }
    }
  }
  return 0;
}

void ResetVar(void) {
  ResetVarNextLevel();
  levelType = 0;
  LIVE = 7;
}

void ResetVarNextLevel(void) {
  scrool = 0;
  step4 = 0;
  MainAnim = 0;
  LorR = 1;
  Jump = 0;
  jumpcancel = 0;
  VSlideOut = 0;
  for (uint8_t x = 0; x < 20; x++) {
    key[x][0] = 0;
    key[x][1] = 0;
  }
  keyS = 0;
  //LevelMult = 0;
}

void NextLevel(void) {
  ResetVarNextLevel();
  levelType++;
  sound(2); sound(2); sound(2); sound(2);
}

void TinyMainShift(DriftSprite* DSprite) {
  DSprite->DriftGrid[0][0] = 5;
  DSprite->DriftGrid[0][1] = 6;
  DSprite->DriftGrid[1][0] = 5;
  DSprite->DriftGrid[1][1] = 6;
}

void SpriteShiftInitialise(DriftSprite* DSprite) {
  DSprite->DriftGrid[0][0] = 0; //bas gauche
  DSprite->DriftGrid[0][1] = 0; //bas droite
  DSprite->DriftGrid[1][0] = 0; //haut gauche
  DSprite->DriftGrid[1][1] = 0; //haut droite
  DSprite->x4decalage = 0;
  DSprite->y8decalage = 0;
  DSprite->MainPositionOnGridH = 11;
  DSprite->MainPositionOnGridV = 3;
}

void Tiny_Flip(DriftSprite* DSprite) {
  uint8_t nn, x, m, n, t, Start, decal;
  uint8_t while1 = 1;
#define PrecessQuit nn++;if (nn>=127) {while1=0;break;}
  SSD1306.ssd1306_send_command(0xb0 + 0); // page0 - page1
  SSD1306.ssd1306_send_command(0x00);   // low column start address
  SSD1306.ssd1306_send_command(0x10);   // high column start address
#if 1 // try skipping all of top, see if that helps locking
  SSD1306.ssd1306_send_data_start();
  for (x = 0; x < LIVE - 1; x++) {
    // draw extra lives
    for (t = 0; t < 4; t++) {
      SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite26[t])); // back
    }
    for (t = 0; t < 4; t++) {
      SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite27[t])); // front
    }
  }
  for (x = 0; x < 11 + (3 - (LIVE - 1)); x++) {
    for (t = 0; t < 8; t++) {
      SSD1306.ssd1306_send_byte(0x00); // put a bunch of spaces between lives & key.
    }
  }
    SSD1306.ssd1306_send_data_stop();


    SSD1306.ssd1306_send_data_start();
  if ((pgm_read_byte(&KeyinLevel[levelType]) == keyS) && (/*timer <= 30*/ (timer / 8) % 2 )) {
    for (x = 0; x < 4; x++) {
      SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite12[x]));
    }
  } else {
    for (x = 0; x < 4; x++) {
      for (t = 0; t < 4; t++) {
        SSD1306.ssd1306_send_byte(0x00); //blank key for blink effect.
      }
    }
  }
  SSD1306.ssd1306_send_data_stop();
#endif
  for (m = 1; m < 8; m++)
  {
    SSD1306.ssd1306_send_command(0xb0 + m);
    SSD1306.ssd1306_send_command(0x00);
    SSD1306.ssd1306_send_command(0x10);
    SSD1306.ssd1306_send_data_start();
    n = 0;
    nn = 0;
    while1 = 1;
    Start = 4 - step4;
#define decalIN for(decal=0;decal<DSprite->x4decalage;decal++){SSD1306.ssd1306_send_byte(0X00);PrecessQuit}// for main sprite scr pix2pix
#define decalOUT for(decal=0;decal<4-DSprite->x4decalage;decal++){SSD1306.ssd1306_send_byte(0X00);PrecessQuit}// for main sprite scr pix2pix
    while (while1) {
      if ((Map[m][n] == 7) && (while1 != 0)) {
        for (t = Start; t < 4; t++) {
          SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite7[t]));

          PrecessQuit
        } Start = 0;
        // main sprite
      } else if (((Map[m][n] == 5) || (Map[m][n] == 55)) && (while1 != 0)) {
        if (Map[m][n] == 55) {
          VSlideOut = ((100 / vslide(8 - DSprite->y8decalage)) / 100);
        } else {
          VSlideOut = vslide(DSprite->y8decalage);
        }
        decalIN;

        const uint8_t * sprite;
        if (LorR == 1) {
          switch (MainAnim) {
            case 0:
              sprite = sprite20;
              break;
            case 1:
              sprite = sprite22;
              break;
            case 2:
              sprite = sprite24;
              break;
          }
        } else {
          switch (MainAnim) {
            case 0:
              sprite = sprite26;
              break;
            case 1:
              sprite = sprite28;
              break;
            case 2:
              sprite = sprite30;
              break;
          }
        }
        for (t = 0; t < 4; t++) {
          SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite[t])*VSlideOut); // draw left half of char
          PrecessQuit
        } Start = 0;


      } else if (((Map[m][n] == 6) || (Map[m][n] == 66)) && (while1 != 0)) {
        if (Map[m][n] == 66) {
          VSlideOut = ((100 / vslide(8 - DSprite->y8decalage)) / 100);
        } else {
          VSlideOut = vslide(DSprite->y8decalage);
        }

        const uint8_t * sprite = 0;
        if (LorR == 1) {
          switch (MainAnim) {
            case 0:
              sprite = sprite21;
              break;
            case 1:
              sprite = sprite23;
              break;
            case 2:
              sprite = sprite25;
              break;
          }
        } else {
          switch (MainAnim) {
            case 0:
              sprite = sprite27;
              break;
            case 1:
              sprite = sprite29;
              break;
            case 2:
              sprite = sprite31;
              break;
          }
        }

        if (sprite) {
          for (t = 0; t < 4; t++) {
            SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite[t])*VSlideOut); //right side of char
            PrecessQuit
            Start = DSprite->x4decalage;
          }
        }

        //fin main sprite

      } else if (while1 != 0) {
        const uint8_t * sprite = 0;
        switch (Map[m][n]) {
          case 1:
            sprite = sprite1;
            break;
          case 2:
            sprite = sprite2;
            break;
          case 3:
            sprite = sprite3;
            break;
          case 4:
            sprite = sprite4;
            break;
          case 8:
            sprite = ((timer%(MAX_TIMER/5)) > MAX_TIMER / 10) ? sprite8 : sprite9;
            break;
          case 15:
            sprite = sprite15;
            break;
          case 16:
            sprite = sprite16;
            break;
          case 11:
            sprite = (timer > MAX_TIMER / 2) ? sprite11 : sprite12;
            break;
          case 13:
            sprite = sprite13;
            break;
          case 14:
            sprite = sprite14;
            break;
          default:
            sprite = 0;
            for (t = Start; t < 4; t++) {
              SSD1306.ssd1306_send_byte(0x00);
              PrecessQuit
            }
            Start = 0;
            break;
        }
        if (sprite) {
          for (t = Start; t < 4; t++) {
            //if (control::isPressed(control::BTN_B))
            SSD1306.ssd1306_send_byte(pgm_read_byte(&sprite[t])); //draw block
            PrecessQuit
          }
          Start = 0;
        }
      }

      if (while1 != 0) {
        n++;
      }
    }
    SSD1306.ssd1306_send_data_stop();
  }
}

} // namespace gilbert
