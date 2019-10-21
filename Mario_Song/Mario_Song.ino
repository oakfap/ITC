#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_D1  37
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_G1  49
#define NOTE_A1  55
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_D2  73
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_G2  98
#define NOTE_A2  110
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_D6  1175
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_G6  1568
#define NOTE_A6  1760
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_D7  2349
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_G7  3136
#define NOTE_A7  3520
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_D8  4699
#define rest    -1
short piezo = 3;
struct R {
  short note;
  short zone;
};
int Notes[8][8] = {
  {NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,  NOTE_C2},
  {NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,  NOTE_C3},
  {NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3,  NOTE_C4},
  {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,  NOTE_C5},
  {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,  NOTE_C6},
  {NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,  NOTE_C7},
  {NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7,  NOTE_C8},
  {NOTE_C8, NOTE_D8, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B0,  NOTE_C1},
};
R Rec[150] ;
int Rec_Length[150];
void setup()
{
  pinMode(piezo, OUTPUT);
  //  digitalWrite(led, LOW);
  Serial.begin(9600);
  for (int i = 4; i <= 11; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT);
  pinMode(13, INPUT_PULLUP);
}
int  debo = 0 ;
short zone = 5 , idx = 0 ;
int noteDis = millis();
void loop()
{
  if (!digitalRead(13)) {
    zone++;
    if (zone > 7 )
      zone = 0;
  }
  for (short i = 0; i <= 7; i++) {
    if (!digitalRead(i + 4)) {
      if (millis() - debo > 100) {
        tone(piezo, Notes[zone][i],500);
        Rec[idx].note = i; Rec[idx].zone = zone;
        debo = millis();
        Rec_Length[idx] = millis() - noteDis ;
        noteDis = millis();
        idx++;
      }
    }
  }
  if (!digitalRead(2)) {
    Serial.print("EEEE");
    for (short i = 0; i <= idx; i++) {
      tone(piezo, Notes[Rec[i].zone][Rec[i].note], 500);
      if (i != idx - 1)
        delay(Rec_Length[i + 1]);
      else delay(500);
    }
    noTone(3);
  }
}
