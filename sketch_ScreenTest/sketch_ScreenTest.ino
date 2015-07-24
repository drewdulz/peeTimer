#include <EEPROM.h>
#include <LiquidCrystal.h>

// Initialize LCD
LiquidCrystal lcd(7,8,9,10,11,12); 


// Define our variables
int highScoreAddress = 0; // The address in the EEPROM where our high score will be stored.
int currentScore = 0;
bool peeStarted = false;
int peeStopped = 0; // Amount of time since pee has stopped
byte highScore = EEPROM.read(highScoreAddress);
float voltage = 0;

void setup() {
  // Show welcome message and the current high score
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.write("Welcome");
  lcd.setCursor(0,1);
  lcd.write("High Score: ");
  lcd.setCursor(12,1);
  lcd.print(highScore, DEC);

  EEPROM.write(highScoreAddress, 5);

}

void loop() {
  lcd.clear();
  voltage = analogRead(A0);
  lcd.setCursor(0,0);
  lcd.write("Your score: ");
  lcd.setCursor(0,1);
  lcd.print(voltage);
  
  delay(500);

}
