#include "LedControl.h"
#include "FontLEDClock.h"
#define Collapse field[Bullets[i].device][Bullets[i].x][Bullets[i].y]
#define BossHit Bullets[i].device==3&&BossHitBox[Bullets[i].x][Bullets[i].y]

// Joy Pin
short up_button = 2;
short down_button = 4;
short left_button = 5;
short right_button = 3;
short start_button = 6;
short select_button = 7;
short joystick_button = 8;
short joystick_axis_x = A0;
short joystick_axis_y = A1;
short buttons[] = {up_button, down_button, left_button, right_button, start_button, select_button, joystick_button};
short totalEnemy = 5, totalBullet = 3, enemySpeed = 600;
using namespace std;

// Game Variable
char Hp = '8';
short score = 0 ;

struct Obj {
  short x, y , device = 1;
  bool visible = false;
};

Obj ship;
// Portrait of Object
bool shipIm[8][8] {   {0, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
bool bulletIm[8][8] {   {1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
bool enemyIm[8][8] {   { 1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
bool bossIm[8][8] {   { 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

// array of hitbox
bool field[4][8][8] ;
bool BossHitBox[8][8];
Obj Bullets[15];
Obj Enemies[15];
Obj boss; short BossHp = 10;
LedControl lc = LedControl(11, 13, 10, 4 ); // DIN,CLK,CS,Number of LED Module
unsigned long delaytime = 500 ; // time to updates of the display
void setup() {
  randomSeed(5);
  for (short i; i < 7; i++)
  {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  Serial.begin(9600);
  short devices = lc.getDeviceCount ();

  for (short address = 0 ; address < devices; address ++) {
    lc.shutdown(address, false);
    lc.setIntensity(address, 8);
    lc.clearDisplay(address);
  }
  ship.x = 6 ;
  ship.y = 0;
  for (int i = 1; i <= totalEnemy; i++) {
    Obj enemy;
    enemy.x = rand() % 7;
    enemy.device = 4;
    enemy.y = rand() % 7;
    enemy.visible = true;
    Dis(enemy, 2);
    Enemies[i] = enemy;
    field[Enemies[i].device][Enemies[i].x][Enemies[i].y] = 1;
  }
}

long debo = 0;
long Bdebo = 0;
void loop() {
  short devices = lc.getDeviceCount ();
  // Bullet Move //
  for (short i = 1; i <= totalBullet; i++) {
    if (Bullets[i].visible) {
      Del(Bullets[i], 1);
      Bullets[i].x--;
      if (Bullets[i].x == -1) {
        Bullets[i].x = 7;
        Bullets[i].device++;
        if (Bullets[i].device >= 5)
          Bullets[i].visible = false;
      }
      if (Collapse) {
        Del(Bullets[i], 1);
        Collapse = 0;
        Bullets[i].visible = !Bullets[i].visible;
        score++;
      } else
        Dis(Bullets[i], 1);
      if (BossHit) {
        BossHp--;
        Del(Bullets[i], 1);
        Bullets[i].visible = !Bullets[i].visible;
      }
    }
  }
  // Enemy Move//
  if (millis() - delaytime > enemySpeed) {
    for (short i = 1; i <= totalEnemy; i++) {
      if (Enemies[i].visible) {
        if (field[Enemies[i].device][Enemies[i].x][Enemies[i].y]) {
          Del(Enemies[i], 2);
          field[Enemies[i].device][Enemies[i].x][Enemies[i].y] = 0;
          Enemies[i].x++;
          int lr = random() % 2;
          if (lr) Enemies[i].y++;
          else Enemies[i].y--;

          if (Enemies[i].y < 0) Enemies[i].y = 0;
          if (Enemies[i].y > 6) Enemies[i].y = 6;

          if (Enemies[i].x == 8) {
            Enemies[i].x = 0;
            Enemies[i].device--;
            if (Enemies[i].device <= 0) {
              Hp--;
              Enemies[i].visible = false;
              Del(Enemies[i], 2);
            }
          }
          field[Enemies[i].device][Enemies[i].x][Enemies[i].y] = 1;
          Dis(Enemies[i], 2);
        } else {
          Del(Enemies[i], 2);
          Enemies[i].visible = !Enemies[i].visible;

        }
      }
      delaytime  = millis();
    }
  }
  // Ship control //
  if (millis() - debo > 200) {
    if (analogRead(joystick_axis_x) <= 300) { //  Left
      debo = millis();
      Del(ship, 0);
      ship.y++;
    } else if (analogRead(joystick_axis_x) >= 400) { // Right
      debo = millis();
      Del(ship, 0);
      ship.y--;
    }
  }
  Dis(ship, 0);
  // firing
  if (millis() - Bdebo > 200)
    if (!digitalRead(right_button)) {
      for (int i = 1 ; i <= totalBullet ; i++) {
        if (!Bullets[i].visible) {
          Bdebo = millis();
          Obj bullet;
          bullet.y = ship.y + 1;
          bullet.x = 5;
          bullet.visible = true;
          Dis(bullet, 1);
          Bullets[i] = bullet;
          break;
        }
      }
    }
  //spawn new
  for (int i = 1; i <= totalEnemy; i++) {
    if (!Enemies[i].visible) {
      Obj enemy;
      enemy.x = rand() % 7;
      enemy.device = 4;
      enemy.y = rand() % 7;
      enemy.visible = true;
      Dis(enemy, 2);
      Enemies[i] = enemy;
      field[Enemies[i].device][Enemies[i].x][Enemies[i].y] = 1;
    }
  }
  // Display HP
  print_normal_char(26, 1, Hp, 1);

  if (Hp == '0') {
    char T[] = "Lose";
    game(T);
  }
  if (BossHp <= 0) {
    char T[] = "WIN";
    game(T);
  }
  if (score == 15) {
    totalEnemy = 10;
    totalBullet = 5;
    enemySpeed = 400;
    BossHp = 10;
    boss.x = 0; boss.device = 3 ;
    boss.y = 2; boss.visible = true;
    Dis(boss, 3);
  }
  // BOSS
  if (boss.visible) {
    Del(boss, 3);
    for (int i = 0 ; i < 2; i++) {
      for (int j = 0 ; j < 4; j++) {
        if (bossIm[i][j]) {
          BossHitBox[boss.x + i][boss.y + j] = 0;
        }
      }
    }
    int lr = random() % 5;
    if (lr == 1) boss.y++;
    else if (lr == 0) boss.y--;

    if (boss.y < 0) boss.y = 0;
    if (boss.y > 4) boss.y = 4;
    for (int i = 0 ; i < 2; i++) {
      for (int j = 0 ; j < 4; j++) {
        if (bossIm[i][j]) {
          BossHitBox[boss.x + i][boss.y + j] = 1;
        }
      }
    }
    Dis(boss, 3);
  }
  delay(100);
  //Serial.println(BossHp);
}

void Del(Obj o, short mode) {
  for (short i = 0; i < 8; i++)
    for (short j = 0; j < 8; j++) {
      if (mode == 0)
        if (shipIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, false);
        }
      if (mode == 1)
        if (bulletIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, false);
        }
      if (mode == 2)
        if (enemyIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, false);
        }
      if (mode == 3)
        if (bossIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, false);
        }
    }
}
void Dis(Obj o, short mode) {
  for (short i = 0; i < 8; i++)
    for (short j = 0; j < 8; j++) {
      if (mode == 0)
        if (shipIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, true);
        }
      if (mode == 1)
        if (bulletIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, true);
        }
      if (mode == 2)
        if (enemyIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, true);
        }
      if (mode == 3)
        if (bossIm[i][j]) {
          lc.setLed(o.device, o.y + j, o.x + i, true);
        }
    }
}


// FONT DISPLAY
void plot (uint8_t x, uint8_t y, uint8_t value, bool mode) {

  //select which matrix depending on the x coord
  uint8_t address;
  if (x >= 0 && x <= 7)   {
    address = 3;
  }
  if (x >= 8 && x <= 15)  {
    address = 2;
    x = x - 8;
  }
  if (x >= 16 && x <= 23) {
    address = 1;
    x = x - 16;
  }
  if (x >= 24 && x <= 31) {
    address = 0;
    x = x - 24;
  }
  if (mode) {
    if (value == 1) {
      lc.setLed(address, 7 - x, y, true);
    } else {
      lc.setLed(address, 7 - x, y, false);
    }
  } else if (!mode) {
    if (value == 1) {
      lc.setLed(address, y, x, true);
    } else {
      lc.setLed(address, y, x, false);
    }
  }
}


void print_normal_char(uint8_t x, uint8_t y, char c, bool mode)
{

  uint8_t dots;
  if (c >= 'A' && c <= 'Z' ) {
    c &= 0x1F;  // A-Z maps to 1-26
  }
  else if (c >= 'a' && c <= 'z') {
    c = (c - 'a') + 41;  // A-Z maps to 41-67
  }
  else if (c >= '0' && c <= '9') {
    c = (c - '0') + 31;
  }
  else if (c == ' ') {
    c = 0;  // space
  }
  else if (c == '.') {
    c = 27;  // full stop
  }
  else if (c == '\'') {
    c = 28;  // single quote mark
  }
  else if (c == ':') {
    c = 29;  // clock_mode selector arrow
  }
  else if (c == '>') {
    c = 30;  // clock_mode selector arrow
  }
  else if (c >= -80 && c <= -67) {
    c *= -1;
  }

  for (uint8_t col = 0; col < 5; col++) {
    dots = pgm_read_byte_near(&myfont[c][col]);
    for (uint8_t row = 0; row < 7; row++) {
      if (dots & (64 >> row)) {   // only 7 rows.
        plot(x + col, y + row, 1, mode);
      } else {
        plot(x + col, y + row, 0, mode);
      }
    }
  }
}

void clear_display() {
  for (uint8_t address = 0; address < 4; address++) {
    lc.clearDisplay(address);
  }
}
void game(char T[]) {
  clear_display();
  short j = 0;
  while (T[j]) {
    print_normal_char((j * 6) + 4, 1, T[j], 0);
    j++;
  } delay(10000);
  enemySpeed = 600;
  boss.visible = false;
  totalEnemy = 5;
  totalBullet = 3;
  Hp = '8';
  BossHp = 15;
  score = 0;
  clear_display();

}
