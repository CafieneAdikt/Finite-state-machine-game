#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// create new int variable type called state
enum state {START_screen, GAME_screen, PAUSE_screen, END_screen};

state current_state = START_screen; // set initial state


const int INTBUTTON1 = 7; // up button
const int INTBUTTON2 = 3; // down button
const int INTBUTTON3 = 6; // pause button
volatile bool UP_Pressed = false; // flag for up button
volatile bool DWN_Pressed = false; // flag for down button
volatile bool PAUSE_Pressed = false; // flag for pause button
const unsigned long DEBOUNCE_TIME_MS = 200; // debounce time interval
unsigned long previousDebounce = 0; // define and set variable to zero
unsigned long previousMillis = 0; // define and set variable to zero
const unsigned long responsetime = 5000; // minimum time player has to respond by
unsigned long SCORE = 0; // define variable for score
unsigned long Dtime = 0; // random number between 500 and 1500
unsigned long jumporduck = 0; // random number between 1 and 10
volatile bool jumpflag = false; // flag for game command
volatile bool duckflag = false; // flag for game command  


// interrupt routines for buttons

void UPBUTTON_ISR(){
 // send flag high
 UP_Pressed = true;

}

void DOWNBUTTON_ISR(){
  // send flag high
  DWN_Pressed = true;

}

void PAUSEBUTTON_ISR(){
  // send flag high
  PAUSE_Pressed = true;

}

// functions

void STARTscreen(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25,0);
  display.clearDisplay();
  display.println("Jumping");
  display.setCursor(20,20);
  display.println("Stickman");
  display.display();
  delay(500);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(18,45);
  display.println("Press Any Button");
  display.display();
  delay(1000);

}

void PAUSEscreen(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(35,0);
  display.println("PAUSE");
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("SCORE:");
  display.display();
  display.setCursor(80,20);
  display.println(SCORE);
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,37);
  display.println("Press:");
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,47);
  display.println("-Pause to continue");
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,57);
  display.println("-Down to go to start");
  display.display();
  delay(1000);

}

void ENDscreen(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15,0);
  display.println("GAME OVER");
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("SCORE:");
  display.display();
  display.setCursor(80,20);
  display.println(SCORE);
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,40);
  display.println("Press Any Button");
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,50);
  display.println("To go to start");
  display.display();
  delay(1000);

}


void charactergrounded(void) {
    // head
    display.drawCircle(20, 35, 5, WHITE);

    // body
    display.drawPixel(20,41,WHITE);
    display.drawPixel(20,42,WHITE);
    display.drawPixel(20,43,WHITE);
    display.drawPixel(20,44,WHITE);
    display.drawPixel(20,45,WHITE);
    display.drawPixel(20,46,WHITE);
    display.drawPixel(20,47,WHITE);
    display.drawPixel(20,48,WHITE);
    display.drawPixel(20,49,WHITE);
    display.drawPixel(20,50,WHITE);
    display.drawPixel(20,51,WHITE);
    display.drawPixel(20,52,WHITE);
    display.drawPixel(20,53,WHITE);
    display.drawPixel(20,54,WHITE);

    // left leg
    display.drawPixel(19,55,WHITE);
    display.drawPixel(19,56,WHITE);
    display.drawPixel(18,57,WHITE);
    display.drawPixel(18,58,WHITE);
    display.drawPixel(17,59,WHITE);
    display.drawPixel(17,60,WHITE);
    display.drawPixel(16,61,WHITE);
    display.drawPixel(16,62,WHITE);
    display.drawPixel(15,63,WHITE);

    // right leg
    display.drawPixel(21,55,WHITE);
    display.drawPixel(21,56,WHITE);
    display.drawPixel(22,57,WHITE);
    display.drawPixel(22,58,WHITE);
    display.drawPixel(23,59,WHITE);
    display.drawPixel(23,60,WHITE);
    display.drawPixel(24,61,WHITE);
    display.drawPixel(24,62,WHITE);
    display.drawPixel(25,63,WHITE);

    // left arm
    display.drawPixel(19,47,WHITE);
    display.drawPixel(18,47,WHITE);
    display.drawPixel(17,47,WHITE);
    display.drawPixel(16,47,WHITE);
    display.drawPixel(15,47,WHITE);
    display.drawPixel(14,47,WHITE);

    // right arm
    display.drawPixel(21,47,WHITE);
    display.drawPixel(22,47,WHITE);
    display.drawPixel(23,47,WHITE);
    display.drawPixel(24,47,WHITE);
    display.drawPixel(25,47,WHITE);
    display.drawPixel(26,47,WHITE);

    // display character
    display.display();
    delay(1);
    
}


void characterjump(void) {
    // grounded
    display.drawCircle(20, 35, 5, WHITE);
    display.drawPixel(20,41,WHITE);
    display.drawPixel(20,42,WHITE);
    display.drawPixel(20,43,WHITE);
    display.drawPixel(20,44,WHITE);
    display.drawPixel(20,45,WHITE);
    display.drawPixel(20,46,WHITE);
    display.drawPixel(20,47,WHITE);
    display.drawPixel(20,48,WHITE);
    display.drawPixel(20,49,WHITE);
    display.drawPixel(20,50,WHITE);
    display.drawPixel(20,51,WHITE);
    display.drawPixel(20,52,WHITE);
    display.drawPixel(20,53,WHITE);
    display.drawPixel(20,54,WHITE);

    // left leg
    display.drawPixel(19,55,WHITE);
    display.drawPixel(19,56,WHITE);
    display.drawPixel(18,57,WHITE);
    display.drawPixel(18,58,WHITE);
    display.drawPixel(17,59,WHITE);
    display.drawPixel(17,60,WHITE);
    display.drawPixel(16,61,WHITE);
    display.drawPixel(16,62,WHITE);
    display.drawPixel(15,63,WHITE);

    // right leg
    display.drawPixel(21,55,WHITE);
    display.drawPixel(21,56,WHITE);
    display.drawPixel(22,57,WHITE);
    display.drawPixel(22,58,WHITE);
    display.drawPixel(23,59,WHITE);
    display.drawPixel(23,60,WHITE);
    display.drawPixel(24,61,WHITE);
    display.drawPixel(24,62,WHITE);
    display.drawPixel(25,63,WHITE);

    // left arm
    display.drawPixel(19,47,WHITE);
    display.drawPixel(18,47,WHITE);
    display.drawPixel(17,47,WHITE);
    display.drawPixel(16,47,WHITE);
    display.drawPixel(15,47,WHITE);
    display.drawPixel(14,47,WHITE);

    // right arm
    display.drawPixel(21,47,WHITE);
    display.drawPixel(22,47,WHITE);
    display.drawPixel(23,47,WHITE);
    display.drawPixel(24,47,WHITE);
    display.drawPixel(25,47,WHITE);
    display.drawPixel(26,47,WHITE);

    // display character
    display.display();
    delay(250);
    
    // write over in BLACK
    display.drawCircle(20, 35, 5, BLACK);
    display.drawPixel(20,41,BLACK);
    display.drawPixel(20,42,BLACK);
    display.drawPixel(20,43,BLACK);
    display.drawPixel(20,44,BLACK);
    display.drawPixel(20,45,BLACK);
    display.drawPixel(20,46,BLACK);
    display.drawPixel(20,47,BLACK);
    display.drawPixel(20,48,BLACK);
    display.drawPixel(20,49,BLACK);
    display.drawPixel(20,50,BLACK);
    display.drawPixel(20,51,BLACK);
    display.drawPixel(20,52,BLACK);
    display.drawPixel(20,53,BLACK);
    display.drawPixel(20,54,BLACK);

    // left leg
    display.drawPixel(19,55,BLACK);
    display.drawPixel(19,56,BLACK);
    display.drawPixel(18,57,BLACK);
    display.drawPixel(18,58,BLACK);
    display.drawPixel(17,59,BLACK);
    display.drawPixel(17,60,BLACK);
    display.drawPixel(16,61,BLACK);
    display.drawPixel(16,62,BLACK);
    display.drawPixel(15,63,BLACK);

    // right leg
    display.drawPixel(21,55,BLACK);
    display.drawPixel(21,56,BLACK);
    display.drawPixel(22,57,BLACK);
    display.drawPixel(22,58,BLACK);
    display.drawPixel(23,59,BLACK);
    display.drawPixel(23,60,BLACK);
    display.drawPixel(24,61,BLACK);
    display.drawPixel(24,62,BLACK);
    display.drawPixel(25,63,BLACK);

    // left arm
    display.drawPixel(19,47,BLACK);
    display.drawPixel(18,47,BLACK);
    display.drawPixel(17,47,BLACK);
    display.drawPixel(16,47,BLACK);
    display.drawPixel(15,47,BLACK);
    display.drawPixel(14,47,BLACK);

    // right arm
    display.drawPixel(21,47,BLACK);
    display.drawPixel(22,47,BLACK);
    display.drawPixel(23,47,BLACK);
    display.drawPixel(24,47,BLACK);
    display.drawPixel(25,47,BLACK);
    display.drawPixel(26,47,BLACK);


    // jump halway up
    display.drawCircle(20, 15, 5, WHITE);
    display.drawPixel(20,21,WHITE);
    display.drawPixel(20,22,WHITE);
    display.drawPixel(20,23,WHITE);
    display.drawPixel(20,24,WHITE);
    display.drawPixel(20,25,WHITE);
    display.drawPixel(20,26,WHITE);
    display.drawPixel(20,27,WHITE);
    display.drawPixel(20,28,WHITE);
    display.drawPixel(20,29,WHITE);
    display.drawPixel(20,30,WHITE);
    display.drawPixel(20,31,WHITE);
    display.drawPixel(20,32,WHITE);
    display.drawPixel(20,33,WHITE);
    display.drawPixel(20,34,WHITE);

    // left leg
    display.drawPixel(19,35,WHITE);
    display.drawPixel(19,36,WHITE);
    display.drawPixel(18,37,WHITE);
    display.drawPixel(18,38,WHITE);
    display.drawPixel(17,39,WHITE);
    display.drawPixel(17,40,WHITE);
    display.drawPixel(16,41,WHITE);
    display.drawPixel(16,42,WHITE);
    display.drawPixel(15,43,WHITE);

    // right leg
    display.drawPixel(21,35,WHITE);
    display.drawPixel(21,36,WHITE);
    display.drawPixel(22,37,WHITE);
    display.drawPixel(22,38,WHITE);
    display.drawPixel(23,39,WHITE);
    display.drawPixel(23,40,WHITE);
    display.drawPixel(24,41,WHITE);
    display.drawPixel(24,42,WHITE);
    display.drawPixel(25,43,WHITE);

    // left arm
    display.drawPixel(19,27,WHITE);
    display.drawPixel(18,27,WHITE);
    display.drawPixel(17,27,WHITE);
    display.drawPixel(16,27,WHITE);
    display.drawPixel(15,27,WHITE);
    display.drawPixel(14,27,WHITE);

    // right arm
    display.drawPixel(21,27,WHITE);
    display.drawPixel(22,27,WHITE);
    display.drawPixel(23,27,WHITE);
    display.drawPixel(24,27,WHITE);
    display.drawPixel(25,27,WHITE);
    display.drawPixel(26,27,WHITE);

    // display character
    display.display();
    delay(250);

    // write over in black
     // jump halway up
    display.drawCircle(20, 15, 5, BLACK);
    display.drawPixel(20,21,BLACK);
    display.drawPixel(20,22,BLACK);
    display.drawPixel(20,23,BLACK);
    display.drawPixel(20,24,BLACK);
    display.drawPixel(20,25,BLACK);
    display.drawPixel(20,26,BLACK);
    display.drawPixel(20,27,BLACK);
    display.drawPixel(20,28,BLACK);
    display.drawPixel(20,29,BLACK);
    display.drawPixel(20,30,BLACK);
    display.drawPixel(20,31,BLACK);
    display.drawPixel(20,32,BLACK);
    display.drawPixel(20,33,BLACK);
    display.drawPixel(20,34,BLACK);

    // left leg
    display.drawPixel(19,35,BLACK);
    display.drawPixel(19,36,BLACK);
    display.drawPixel(18,37,BLACK);
    display.drawPixel(18,38,BLACK);
    display.drawPixel(17,39,BLACK);
    display.drawPixel(17,40,BLACK);
    display.drawPixel(16,41,BLACK);
    display.drawPixel(16,42,BLACK);
    display.drawPixel(15,43,BLACK);

    // right leg
    display.drawPixel(21,35,BLACK);
    display.drawPixel(21,36,BLACK);
    display.drawPixel(22,37,BLACK);
    display.drawPixel(22,38,BLACK);
    display.drawPixel(23,39,BLACK);
    display.drawPixel(23,40,BLACK);
    display.drawPixel(24,41,BLACK);
    display.drawPixel(24,42,BLACK);
    display.drawPixel(25,43,BLACK);

    // left arm
    display.drawPixel(19,27,BLACK);
    display.drawPixel(18,27,BLACK);
    display.drawPixel(17,27,BLACK);
    display.drawPixel(16,27,BLACK);
    display.drawPixel(15,27,BLACK);
    display.drawPixel(14,27,BLACK);

    // right arm
    display.drawPixel(21,27,BLACK);
    display.drawPixel(22,27,BLACK);
    display.drawPixel(23,27,BLACK);
    display.drawPixel(24,27,BLACK);
    display.drawPixel(25,27,BLACK);
    display.drawPixel(26,27,BLACK);

      // jump all way up
    display.drawCircle(20, 5, 5, WHITE);
    display.drawPixel(20,11,WHITE);
    display.drawPixel(20,12,WHITE);
    display.drawPixel(20,13,WHITE);
    display.drawPixel(20,14,WHITE);
    display.drawPixel(20,15,WHITE);
    display.drawPixel(20,16,WHITE);
    display.drawPixel(20,17,WHITE);
    display.drawPixel(20,18,WHITE);
    display.drawPixel(20,19,WHITE);
    display.drawPixel(20,20,WHITE);
    display.drawPixel(20,21,WHITE);
    display.drawPixel(20,22,WHITE);
    display.drawPixel(20,23,WHITE);
    display.drawPixel(20,24,WHITE);

    // left leg
    display.drawPixel(19,25,WHITE);
    display.drawPixel(19,26,WHITE);
    display.drawPixel(18,27,WHITE);
    display.drawPixel(18,28,WHITE);
    display.drawPixel(17,29,WHITE);
    display.drawPixel(17,30,WHITE);
    display.drawPixel(16,31,WHITE);
    display.drawPixel(16,32,WHITE);
    display.drawPixel(15,33,WHITE);

    // right leg
    display.drawPixel(21,25,WHITE);
    display.drawPixel(21,26,WHITE);
    display.drawPixel(22,27,WHITE);
    display.drawPixel(22,28,WHITE);
    display.drawPixel(23,29,WHITE);
    display.drawPixel(23,30,WHITE);
    display.drawPixel(24,31,WHITE);
    display.drawPixel(24,32,WHITE);
    display.drawPixel(25,33,WHITE);

    // left arm
    display.drawPixel(19,17,WHITE);
    display.drawPixel(18,17,WHITE);
    display.drawPixel(17,17,WHITE);
    display.drawPixel(16,17,WHITE);
    display.drawPixel(15,17,WHITE);
    display.drawPixel(14,17,WHITE);

    // right arm
    display.drawPixel(21,17,WHITE);
    display.drawPixel(22,17,WHITE);
    display.drawPixel(23,17,WHITE);
    display.drawPixel(24,17,WHITE);
    display.drawPixel(25,17,WHITE);
    display.drawPixel(26,17,WHITE);

    // display character
    display.display();
    delay(250);

    // write over in black
       // jump all way up
    display.drawCircle(20, 5, 5, BLACK);
    display.drawPixel(20,11,BLACK);
    display.drawPixel(20,12,BLACK);
    display.drawPixel(20,13,BLACK);
    display.drawPixel(20,14,BLACK);
    display.drawPixel(20,15,BLACK);
    display.drawPixel(20,16,BLACK);
    display.drawPixel(20,17,BLACK);
    display.drawPixel(20,18,BLACK);
    display.drawPixel(20,19,BLACK);
    display.drawPixel(20,20,BLACK);
    display.drawPixel(20,21,BLACK);
    display.drawPixel(20,22,BLACK);
    display.drawPixel(20,23,BLACK);
    display.drawPixel(20,24,BLACK);

    // left leg
    display.drawPixel(19,25,BLACK);
    display.drawPixel(19,26,BLACK);
    display.drawPixel(18,27,BLACK);
    display.drawPixel(18,28,BLACK);
    display.drawPixel(17,29,BLACK);
    display.drawPixel(17,30,BLACK);
    display.drawPixel(16,31,BLACK);
    display.drawPixel(16,32,BLACK);
    display.drawPixel(15,33,BLACK);

    // right leg
    display.drawPixel(21,25,BLACK);
    display.drawPixel(21,26,BLACK);
    display.drawPixel(22,27,BLACK);
    display.drawPixel(22,28,BLACK);
    display.drawPixel(23,29,BLACK);
    display.drawPixel(23,30,BLACK);
    display.drawPixel(24,31,BLACK);
    display.drawPixel(24,32,BLACK);
    display.drawPixel(25,33,BLACK);

    // left arm
    display.drawPixel(19,17,BLACK);
    display.drawPixel(18,17,BLACK);
    display.drawPixel(17,17,BLACK);
    display.drawPixel(16,17,BLACK);
    display.drawPixel(15,17,BLACK);
    display.drawPixel(14,17,BLACK);

    // right arm
    display.drawPixel(21,17,BLACK);
    display.drawPixel(22,17,BLACK);
    display.drawPixel(23,17,BLACK);
    display.drawPixel(24,17,BLACK);
    display.drawPixel(25,17,BLACK);
    display.drawPixel(26,17,BLACK);


    // jump halway up
    display.drawCircle(20, 15, 5, WHITE);
    display.drawPixel(20,21,WHITE);
    display.drawPixel(20,22,WHITE);
    display.drawPixel(20,23,WHITE);
    display.drawPixel(20,24,WHITE);
    display.drawPixel(20,25,WHITE);
    display.drawPixel(20,26,WHITE);
    display.drawPixel(20,27,WHITE);
    display.drawPixel(20,28,WHITE);
    display.drawPixel(20,29,WHITE);
    display.drawPixel(20,30,WHITE);
    display.drawPixel(20,31,WHITE);
    display.drawPixel(20,32,WHITE);
    display.drawPixel(20,33,WHITE);
    display.drawPixel(20,34,WHITE);

    // left leg
    display.drawPixel(19,35,WHITE);
    display.drawPixel(19,36,WHITE);
    display.drawPixel(18,37,WHITE);
    display.drawPixel(18,38,WHITE);
    display.drawPixel(17,39,WHITE);
    display.drawPixel(17,40,WHITE);
    display.drawPixel(16,41,WHITE);
    display.drawPixel(16,42,WHITE);
    display.drawPixel(15,43,WHITE);

    // right leg
    display.drawPixel(21,35,WHITE);
    display.drawPixel(21,36,WHITE);
    display.drawPixel(22,37,WHITE);
    display.drawPixel(22,38,WHITE);
    display.drawPixel(23,39,WHITE);
    display.drawPixel(23,40,WHITE);
    display.drawPixel(24,41,WHITE);
    display.drawPixel(24,42,WHITE);
    display.drawPixel(25,43,WHITE);

    // left arm
    display.drawPixel(19,27,WHITE);
    display.drawPixel(18,27,WHITE);
    display.drawPixel(17,27,WHITE);
    display.drawPixel(16,27,WHITE);
    display.drawPixel(15,27,WHITE);
    display.drawPixel(14,27,WHITE);

    // right arm
    display.drawPixel(21,27,WHITE);
    display.drawPixel(22,27,WHITE);
    display.drawPixel(23,27,WHITE);
    display.drawPixel(24,27,WHITE);
    display.drawPixel(25,27,WHITE);
    display.drawPixel(26,27,WHITE);

    // display character
    display.display();
    delay(250);

    // write over in black
     // jump halway up
    display.drawCircle(20, 15, 5, BLACK);
    display.drawPixel(20,21,BLACK);
    display.drawPixel(20,22,BLACK);
    display.drawPixel(20,23,BLACK);
    display.drawPixel(20,24,BLACK);
    display.drawPixel(20,25,BLACK);
    display.drawPixel(20,26,BLACK);
    display.drawPixel(20,27,BLACK);
    display.drawPixel(20,28,BLACK);
    display.drawPixel(20,29,BLACK);
    display.drawPixel(20,30,BLACK);
    display.drawPixel(20,31,BLACK);
    display.drawPixel(20,32,BLACK);
    display.drawPixel(20,33,BLACK);
    display.drawPixel(20,34,BLACK);

    // left leg
    display.drawPixel(19,35,BLACK);
    display.drawPixel(19,36,BLACK);
    display.drawPixel(18,37,BLACK);
    display.drawPixel(18,38,BLACK);
    display.drawPixel(17,39,BLACK);
    display.drawPixel(17,40,BLACK);
    display.drawPixel(16,41,BLACK);
    display.drawPixel(16,42,BLACK);
    display.drawPixel(15,43,BLACK);

    // right leg
    display.drawPixel(21,35,BLACK);
    display.drawPixel(21,36,BLACK);
    display.drawPixel(22,37,BLACK);
    display.drawPixel(22,38,BLACK);
    display.drawPixel(23,39,BLACK);
    display.drawPixel(23,40,BLACK);
    display.drawPixel(24,41,BLACK);
    display.drawPixel(24,42,BLACK);
    display.drawPixel(25,43,BLACK);

    // left arm
    display.drawPixel(19,27,BLACK);
    display.drawPixel(18,27,BLACK);
    display.drawPixel(17,27,BLACK);
    display.drawPixel(16,27,BLACK);
    display.drawPixel(15,27,BLACK);
    display.drawPixel(14,27,BLACK);

    // right arm
    display.drawPixel(21,27,BLACK);
    display.drawPixel(22,27,BLACK);
    display.drawPixel(23,27,BLACK);
    display.drawPixel(24,27,BLACK);
    display.drawPixel(25,27,BLACK);
    display.drawPixel(26,27,BLACK); 


    // grounded
    display.drawCircle(20, 35, 5, WHITE);
    display.drawPixel(20,41,WHITE);
    display.drawPixel(20,42,WHITE);
    display.drawPixel(20,43,WHITE);
    display.drawPixel(20,44,WHITE);
    display.drawPixel(20,45,WHITE);
    display.drawPixel(20,46,WHITE);
    display.drawPixel(20,47,WHITE);
    display.drawPixel(20,48,WHITE);
    display.drawPixel(20,49,WHITE);
    display.drawPixel(20,50,WHITE);
    display.drawPixel(20,51,WHITE);
    display.drawPixel(20,52,WHITE);
    display.drawPixel(20,53,WHITE);
    display.drawPixel(20,54,WHITE);

    // left leg
    display.drawPixel(19,55,WHITE);
    display.drawPixel(19,56,WHITE);
    display.drawPixel(18,57,WHITE);
    display.drawPixel(18,58,WHITE);
    display.drawPixel(17,59,WHITE);
    display.drawPixel(17,60,WHITE);
    display.drawPixel(16,61,WHITE);
    display.drawPixel(16,62,WHITE);
    display.drawPixel(15,63,WHITE);

    // right leg
    display.drawPixel(21,55,WHITE);
    display.drawPixel(21,56,WHITE);
    display.drawPixel(22,57,WHITE);
    display.drawPixel(22,58,WHITE);
    display.drawPixel(23,59,WHITE);
    display.drawPixel(23,60,WHITE);
    display.drawPixel(24,61,WHITE);
    display.drawPixel(24,62,WHITE);
    display.drawPixel(25,63,WHITE);

    // left arm
    display.drawPixel(19,47,WHITE);
    display.drawPixel(18,47,WHITE);
    display.drawPixel(17,47,WHITE);
    display.drawPixel(16,47,WHITE);
    display.drawPixel(15,47,WHITE);
    display.drawPixel(14,47,WHITE);

    // right arm
    display.drawPixel(21,47,WHITE);
    display.drawPixel(22,47,WHITE);
    display.drawPixel(23,47,WHITE);
    display.drawPixel(24,47,WHITE);
    display.drawPixel(25,47,WHITE);
    display.drawPixel(26,47,WHITE);

    // display character
    display.display();
    delay(250);


}

void characterduck(void) {
   // grounded
    display.drawCircle(20, 35, 5, WHITE);
    display.drawPixel(20,41,WHITE);
    display.drawPixel(20,42,WHITE);
    display.drawPixel(20,43,WHITE);
    display.drawPixel(20,44,WHITE);
    display.drawPixel(20,45,WHITE);
    display.drawPixel(20,46,WHITE);
    display.drawPixel(20,47,WHITE);
    display.drawPixel(20,48,WHITE);
    display.drawPixel(20,49,WHITE);
    display.drawPixel(20,50,WHITE);
    display.drawPixel(20,51,WHITE);
    display.drawPixel(20,52,WHITE);
    display.drawPixel(20,53,WHITE);
    display.drawPixel(20,54,WHITE);

    // left leg
    display.drawPixel(19,55,WHITE);
    display.drawPixel(19,56,WHITE);
    display.drawPixel(18,57,WHITE);
    display.drawPixel(18,58,WHITE);
    display.drawPixel(17,59,WHITE);
    display.drawPixel(17,60,WHITE);
    display.drawPixel(16,61,WHITE);
    display.drawPixel(16,62,WHITE);
    display.drawPixel(15,63,WHITE);

    // right leg
    display.drawPixel(21,55,WHITE);
    display.drawPixel(21,56,WHITE);
    display.drawPixel(22,57,WHITE);
    display.drawPixel(22,58,WHITE);
    display.drawPixel(23,59,WHITE);
    display.drawPixel(23,60,WHITE);
    display.drawPixel(24,61,WHITE);
    display.drawPixel(24,62,WHITE);
    display.drawPixel(25,63,WHITE);

    // left arm
    display.drawPixel(19,47,WHITE);
    display.drawPixel(18,47,WHITE);
    display.drawPixel(17,47,WHITE);
    display.drawPixel(16,47,WHITE);
    display.drawPixel(15,47,WHITE);
    display.drawPixel(14,47,WHITE);

    // right arm
    display.drawPixel(21,47,WHITE);
    display.drawPixel(22,47,WHITE);
    display.drawPixel(23,47,WHITE);
    display.drawPixel(24,47,WHITE);
    display.drawPixel(25,47,WHITE);
    display.drawPixel(26,47,WHITE);

    // display character
    display.display();
    delay(250);
    
    // write over in BLACK
    display.drawCircle(20, 35, 5, BLACK);
    display.drawPixel(20,41,BLACK);
    display.drawPixel(20,42,BLACK);
    display.drawPixel(20,43,BLACK);
    display.drawPixel(20,44,BLACK);
    display.drawPixel(20,45,BLACK);
    display.drawPixel(20,46,BLACK);
    display.drawPixel(20,47,BLACK);
    display.drawPixel(20,48,BLACK);
    display.drawPixel(20,49,BLACK);
    display.drawPixel(20,50,BLACK);
    display.drawPixel(20,51,BLACK);
    display.drawPixel(20,52,BLACK);
    display.drawPixel(20,53,BLACK);
    display.drawPixel(20,54,BLACK);

    // left leg
    display.drawPixel(19,55,BLACK);
    display.drawPixel(19,56,BLACK);
    display.drawPixel(18,57,BLACK);
    display.drawPixel(18,58,BLACK);
    display.drawPixel(17,59,BLACK);
    display.drawPixel(17,60,BLACK);
    display.drawPixel(16,61,BLACK);
    display.drawPixel(16,62,BLACK);
    display.drawPixel(15,63,BLACK);

    // right leg
    display.drawPixel(21,55,BLACK);
    display.drawPixel(21,56,BLACK);
    display.drawPixel(22,57,BLACK);
    display.drawPixel(22,58,BLACK);
    display.drawPixel(23,59,BLACK);
    display.drawPixel(23,60,BLACK);
    display.drawPixel(24,61,BLACK);
    display.drawPixel(24,62,BLACK);
    display.drawPixel(25,63,BLACK);

    // left arm
    display.drawPixel(19,47,BLACK);
    display.drawPixel(18,47,BLACK);
    display.drawPixel(17,47,BLACK);
    display.drawPixel(16,47,BLACK);
    display.drawPixel(15,47,BLACK);
    display.drawPixel(14,47,BLACK);

    // right arm
    display.drawPixel(21,47,BLACK);
    display.drawPixel(22,47,BLACK);
    display.drawPixel(23,47,BLACK);
    display.drawPixel(24,47,BLACK);
    display.drawPixel(25,47,BLACK);
    display.drawPixel(26,47,BLACK);

    // head on ground to mimic ducking
    display.drawCircle(20, 58, 5, WHITE);

    // display character
    display.display();
    delay(250);

    // display character grounded again
    display.drawCircle(20, 58, 5, BLACK);
    // grounded
    display.drawCircle(20, 35, 5, WHITE);
    display.drawPixel(20,41,WHITE);
    display.drawPixel(20,42,WHITE);
    display.drawPixel(20,43,WHITE);
    display.drawPixel(20,44,WHITE);
    display.drawPixel(20,45,WHITE);
    display.drawPixel(20,46,WHITE);
    display.drawPixel(20,47,WHITE);
    display.drawPixel(20,48,WHITE);
    display.drawPixel(20,49,WHITE);
    display.drawPixel(20,50,WHITE);
    display.drawPixel(20,51,WHITE);
    display.drawPixel(20,52,WHITE);
    display.drawPixel(20,53,WHITE);
    display.drawPixel(20,54,WHITE);

    // left leg
    display.drawPixel(19,55,WHITE);
    display.drawPixel(19,56,WHITE);
    display.drawPixel(18,57,WHITE);
    display.drawPixel(18,58,WHITE);
    display.drawPixel(17,59,WHITE);
    display.drawPixel(17,60,WHITE);
    display.drawPixel(16,61,WHITE);
    display.drawPixel(16,62,WHITE);
    display.drawPixel(15,63,WHITE);

    // right leg
    display.drawPixel(21,55,WHITE);
    display.drawPixel(21,56,WHITE);
    display.drawPixel(22,57,WHITE);
    display.drawPixel(22,58,WHITE);
    display.drawPixel(23,59,WHITE);
    display.drawPixel(23,60,WHITE);
    display.drawPixel(24,61,WHITE);
    display.drawPixel(24,62,WHITE);
    display.drawPixel(25,63,WHITE);

    // left arm
    display.drawPixel(19,47,WHITE);
    display.drawPixel(18,47,WHITE);
    display.drawPixel(17,47,WHITE);
    display.drawPixel(16,47,WHITE);
    display.drawPixel(15,47,WHITE);
    display.drawPixel(14,47,WHITE);

    // right arm
    display.drawPixel(21,47,WHITE);
    display.drawPixel(22,47,WHITE);
    display.drawPixel(23,47,WHITE);
    display.drawPixel(24,47,WHITE);
    display.drawPixel(25,47,WHITE);
    display.drawPixel(26,47,WHITE);

    // display character
    display.display();
    delay(250);



}

void jumpcommand(){
  // display jump on screen
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(80,30);
  display.println("JUMP");
  display.display();
}

void duckcommand(){
  // display duck on screen
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(80,30);
  display.println("DUCK");
  display.display();
}



void setup() {

    Serial.begin(9600); // set up serial com speed

  pinMode(INTBUTTON1, INPUT_PULLUP); // up button
  pinMode(INTBUTTON2, INPUT_PULLUP); // dwn button
  pinMode(INTBUTTON3, INPUT_PULLUP); // pause button
  attachInterrupt(digitalPinToInterrupt(INTBUTTON1), UPBUTTON_ISR, FALLING); // set up interrupt routines
  attachInterrupt(digitalPinToInterrupt(INTBUTTON2), DOWNBUTTON_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(INTBUTTON3), PAUSEBUTTON_ISR, FALLING);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // clear the buffer
  display.clearDisplay();

}


void loop() {

  unsigned long currentmillis = millis(); // recheck and set the time variable for debouncing

// state machine
  switch(current_state) {
    case START_screen:
      // reset flags and score
      UP_Pressed = false; 
      DWN_Pressed = false;
      PAUSE_Pressed = false; 
      jumpflag = false;
      duckflag = false;
      SCORE = 0;

      // actions
      STARTscreen();

      // if any button is pressed start game
      if ((UP_Pressed &&
       ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS))) {
       previousDebounce = currentmillis;   // set new debounce time
       display.clearDisplay(); // clear screen
       previousMillis = currentmillis; // reset response time
       current_state = GAME_screen; // go to game screen
      }
      break;

    case GAME_screen:
      PAUSE_Pressed = false; // reset flags
      UP_Pressed = false;
      DWN_Pressed = false;

      // reset delay and jumporduck variables
      Dtime = rand()%1000+1000;
      jumporduck = rand()%10+1;

      // actions
      charactergrounded();

      // random time delay before command
      delay(Dtime);
   
    // one nested if statement to avoid a second command before the first is completed
    if (jumpflag == false && duckflag == false){
      // if statement to randomly chose between jump or duck
      if (jumporduck == 1 || jumporduck == 2 || jumporduck == 3 || jumporduck == 4 || jumporduck == 5){
        jumpflag = true; // send flag true
        jumpcommand(); // tell player to jump
      }
      else
      {
        duckflag = true; // send flag true
        duckcommand(); // tell player to duck
      }
    }

      // make character jump
      if (UP_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
        previousDebounce = currentmillis; // set new debounce time
        previousMillis = currentmillis; // set response time
        display.clearDisplay(); // reset display
        SCORE = SCORE+1; // increase score by one
        characterjump();
        jumpflag = false;
        //UP_Pressed = false; // reset flag
      }

      // make character duck
      if (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
        previousDebounce = currentmillis; // set new debounce time
        previousMillis = currentmillis; // set response time
        display.clearDisplay(); // reset display
        SCORE = SCORE+1; // increase score by one
        characterduck();
        duckflag = false;
        //DWN_Pressed = false; // reset flag
      }

      // if player takes too long to respond or presses wrong button they go to the end screen
      if (((currentmillis - previousMillis) >= responsetime) || (DWN_Pressed && jumpflag) || (UP_Pressed && duckflag)){
        previousMillis = currentmillis; // set response time 
        display.clearDisplay(); // clear screen
        current_state = END_screen; // go to end screen
      }

      // if some event then state change
      if (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis; // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = PAUSE_screen; // go to pause screen
      }
    
      break;

    case PAUSE_screen:
    PAUSE_Pressed = false; // reset flags
    DWN_Pressed = false; 
    jumpflag = false;
    duckflag = false;

      // actions
      PAUSEscreen(); 

      // if some event then state change
      if (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      previousMillis = currentmillis; // reset response time
      current_state = GAME_screen; // go back to game srceen state
      }
      // if down button pressed end game
      if (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis;   // set new debounce time
      current_state = START_screen; // go to start screen
      }
      break;

    case END_screen:
      UP_Pressed = false; // reset flag
      DWN_Pressed = false; // reset flag
      PAUSE_Pressed = false; // reset flag
      // actions
      ENDscreen();

      // if any button is pressed go to start sceen
      if ((UP_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) ) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = START_screen;
      }
      break;
  }

}