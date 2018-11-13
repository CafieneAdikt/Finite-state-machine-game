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
const int INTBUTTON2 = 4; // down button
const int INTBUTTON3 = 6; // pause button
volatile bool UP_Pressed = false; // flag for up button
volatile bool DWN_Pressed = false; // flag for down button
volatile bool PAUSE_Pressed = false; // flag for pause button
const unsigned long DEBOUNCE_TIME_MS = 500; // debounce time interval
unsigned long previousDebounce = 0; // define and set variable to zero
unsigned long previousMillis = 0; // define and set variable to zero


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


void STARTscreen(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25,0);
  display.clearDisplay();
  display.println("Fleeing    Ninja");
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
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(25,10);
  display.clearDisplay();
  display.println("PAUSE");
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15,45);
  display.println("SCORE:");
  display.display();
  delay(2000);

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


void setup() {

    Serial.begin(9600);

  pinMode(INTBUTTON1, INPUT_PULLUP); // up button
  pinMode(INTBUTTON2, INPUT_PULLUP); // dwn button
  pinMode(INTBUTTON3, INPUT_PULLUP); // pause button
  attachInterrupt(digitalPinToInterrupt(INTBUTTON1), UPBUTTON_ISR, FALLING); // set up interrupt routines
  attachInterrupt(digitalPinToInterrupt(INTBUTTON2), DOWNBUTTON_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(INTBUTTON3), PAUSEBUTTON_ISR, FALLING);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
 
  display.display();

  // Clear the buffer.
  display.clearDisplay();

}


void loop() {

  unsigned long currentmillis = millis(); // recheck and set the time variable for debouncing

  switch(current_state) {
    case START_screen:
      // actions
      STARTscreen();

      // if any button is pressed start game
      if ((UP_Pressed &&
      ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS))) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = GAME_screen; // go to game screen
      UP_Pressed = false; // reset flags
      DWN_Pressed = false;
      PAUSE_Pressed = false; 
      }
      break;

    case GAME_screen:
      // actions
      charactergrounded();

      // if some event then state change
      if (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = PAUSE_screen; // go to pause screen
      PAUSE_Pressed = false; // reset flag
      }
      // if you die in game go to end screen

      break;

    case PAUSE_screen:
      // actions
      PAUSEscreen(); 

      // if some event then state change
      if (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = GAME_screen; // go back to game srceen state
      PAUSE_Pressed = false; // reset flag
      }
      // if down button pressed end game
      if (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis;   // set new debounce time
      current_state = GAME_screen; // go to start screen
      DWN_Pressed = false; // reset flag
      }
      break;

    case END_screen:
      // actions

      // if pause play button is pressed play another game
      if (PAUSE_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = GAME_screen;
      PAUSE_Pressed = false; // reset flag
      }
      // if up of down button is pressed go to start sceen
      if ((UP_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS)) || (DWN_Pressed && ((currentmillis - previousDebounce) >= DEBOUNCE_TIME_MS))) {
      previousDebounce = currentmillis;   // set new debounce time
      display.clearDisplay(); // clear screen
      current_state = START_screen;
      UP_Pressed = false; // reset flag
      DWN_Pressed = false; // reset flag
      }
      break;
  }

}