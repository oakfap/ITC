#define Pbutton 12
#define Sbutton 13

long int Player = 1 , System = 0 ;
unsigned int debo = 0 ;
void setup() {
  Serial.begin(9600);
  for (int i = 2 ; i <= 11; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(Pbutton , INPUT_PULLUP);
  pinMode(Sbutton , INPUT_PULLUP);
  DisplayNum(1);
}

int NumArr[10][7] = {
  {0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 1, 1, 1},
  {0, 0, 1, 0, 0, 1, 0},
  {0, 0, 0, 0, 1, 1, 0},
  {1, 0, 0, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 0},
};

int LedTimer[3] = {0, 0, 0};
int GameTimer = 0 ;
void loop() {
  // Serial.println((String)Player + " : " +  (String)System);
  for (int i = 0 ; i < 3; i++)
  {
    bool logic = HIGH;
    if (i == 1) {
      logic = LOW;
    }
    if (millis() - LedTimer[i] >= 1500) digitalWrite(i + 9, logic);
  }
  if (millis() - GameTimer >= 1500) {
    reset();
    digitalWrite(1, HIGH);
    DisplayNum(Player);
  }
  Serial.println(digitalRead(Pbutton));
  if (!digitalRead(Pbutton)) {
    if (millis() - debo > 250) {
      Player++;
      // dicing
      if (Player > 6) Player = 1 ;
      DisplayNum(Player);
    }
    debo = millis() ;
  }
  // Serial.println(digitalRead(Sbutton));
  if (!digitalRead(Sbutton)) {
    if (millis() - debo > 250) {
      // 7 segment
      int lT = 0 ;
      for (int i = 0 ; i < 15 ; i++ ) {
          System = random(1,7);
          DisplayNum(System);    
      }
      GameTimer = millis();

      // Traffic Led
      digitalWrite(10, HIGH);
      LedTimer[1] = millis();
      if (Player == System) {
        digitalWrite(11, LOW);
        LedTimer[2] = millis();
      } else {
        digitalWrite(9, LOW);
        LedTimer[0] = millis();
      }

    }
    debo = millis();
  }
}
void reset() {
  for (int i = 2; i <= 8; i++) digitalWrite(i, HIGH);
}

void DisplayNum(long int number) {
  reset();
  for (int i = 0 ; i < 7 ; i++)
  {
    digitalWrite(i + 2, NumArr[number][i]);
  }
}
