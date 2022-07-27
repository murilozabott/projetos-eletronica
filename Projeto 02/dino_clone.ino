#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define none 0
#define up 1
#define right 2
#define down 3
#define left 4
#define select 5

byte empty[8] = {
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};

byte dino1[8] = {
 B00000,
 B00111,
 B00111,
 B00110,
 B00111,
 B10110,
 B11110,
 B00010
};

byte dino2[8] = {
 B00000,
 B00111,
 B00111,
 B00110,
 B00111,
 B10110,
 B11110,
 B00100
};

byte crouch1[8] = {
 B00000,
 B00000,
 B00111,
 B00111,
 B00111,
 B10110,
 B11110,
 B00010
};

byte crouch2[8] = {
 B00000,
 B00000,
 B00111,
 B00111,
 B00111,
 B10110,
 B11110,
 B00100
};

byte cactus[8] = {
  B00000,
  B00100,
  B00100,
  B10101,
  B10111,
  B11100,
  B00100,
  B00100
};

byte bird[8] = {
  B11011,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  
  lcd.createChar(1, dino1);
  lcd.createChar(2, dino2);
  lcd.createChar(3, crouch1);
  lcd.createChar(4, crouch2);
  lcd.createChar(5, cactus);
  lcd.createChar(6, bird);
  lcd.createChar(8, empty);
  lcd.begin(16, 2);
  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("Jogo do dino");
  lcd.setCursor(0,1);
  lcd.print("Clique p comecar");
}

int score = 0;
int highscore = 0;
bool gameStarted = false;

long jumpstarted;
int jumptime = 600;

int leg = 0;
int isJumping = 0;
int input;
int keyup = 0;
int keydown = 0;

int obsDelay = 0;

int track[20] = {
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

void loop(){
  input = readKey();

  if(!gameStarted){
    if(input != none){
      lcd.clear();
      gameStarted = true;
      score = 0;
      draw();
    }
  } else {
    if((input == up) && !keyup)jumpstarted = millis();
    keydown = (input == down);
    keyup = (input == up);
    
    left_shift();
    draw();
    score += 1;
    if(!obsDelay) generateObstacle();
    obsDelay -= 1;

    isJumping = ((millis() - jumpstarted < jumptime) && keyup);
    lcd.setCursor(1,isJumping^1);
    lcd.write(keydown*2 + 1 + leg);
    leg ^= 1;

    if(!isJumping && track[1] == 5) handle_gameover();
    if(!isJumping && !keydown && track[1]==6) handle_gameover();
  }

  delay(150);
}

int readKey(){
    int b = analogRead(A0);
    
    if (b > 1000) return none;
    if (b < 50) return right;
    if (b < 180) return up;
    if (b < 330) return down;
    if (b < 520) return left;
    if (b < 700) return select;

    //Fallback
    return none;
}

void draw(){
  lcd.setCursor(4, 0);
  lcd.print("HI ");
  lcd.print(highscore);

  lcd.setCursor(11, 0);
  lcd.print(score);

  if(!isJumping){
    lcd.setCursor(1,0);
    lcd.write(8);
  }
  
  for(int i = 0; i < 16; i++){
    lcd.setCursor(i,1);
    lcd.write(track[i]);
  }
}

void left_shift(){
  for(int i = 0; i <= 18; i++){
    track[i] = track[i+1];
  }
  track[19] = 8;
}

void generateObstacle(){
  int type = random(5,7);
  int count;

  if(type == 5) count = random(1,4);
  else count = 1;

  for(int i = 0; i < count; i++){
    track[16+i] = type;
  }

  obsDelay = count + random(5,8);
}

void handle_gameover(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Game Over");
  lcd.setCursor(0,1);
  lcd.print("Score = ");
  lcd.print(score);
  gameStarted = false;

  for(int i = 0; i < 20; i++){
    track[i] = 8;
  }

  if(score > highscore) highscore = score;
  delay(2000);
}
