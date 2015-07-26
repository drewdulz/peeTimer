/* Wiring:
 * Pololu switch wired as per this tutorial: http://www.instructables.com/id/Using-the-Pololu-Pushbutton-Power-Switch/?ALLSTEPS
 * LCD wired as per this tutorial: http://diyhacking.com/arduino-lcd-tutorial/
 * Analog A1 goes into toilet
 * 5V goes to hand plate
*/



#include <EEPROM.h>
#include <LiquidCrystal.h>

// Initialize LCD
LiquidCrystal lcd(7,8,9,10,11,12); 

// Define our variables
int highScoreAddress = 0; // The address in the EEPROM where our high score will be stored.
long currentScore = 0;
bool peeStarted = false;
int peeStopped = 0; // Amount of time since pee has stopped
long highScore = EEPROMReadlong(highScoreAddress);
float voltage = 0;
int offPin = 4; // Pololu connected to digital pin 4


void setup() {
  // Set the digital pin as output
  pinMode(offPin, OUTPUT);
  // Show welcome message and the current high score
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.write("Welcome!");
  lcd.setCursor(0,1);
  lcd.write("High Score: ");
  lcd.setCursor(12,1);
  lcd.print(highScore);
}

void endPeeCylce() {
  lcd.clear();
  // Show the score
  lcd.setCursor(0,1);
  lcd.write("Your Score: ");
  lcd.setCursor(12,1);
  lcd.print(currentScore);
  lcd.setCursor(0,0);
  
  if(currentScore > highScore) {
    // Save a new high score
    EEPROMWritelong(highScoreAddress, currentScore); // Divide high score by 4 because we can only store numbers up to 255.
    // Show a congratulations message  
    lcd.write("New high score!");
  } else {
    // Show a message that they didn't get a high score
    lcd.write("Nice pee!");
  }
  delay(20000); // Delay 20 seconds and then turn off
  digitalWrite(offPin, HIGH); // Sets the pin to HIGH and shuts down the Arduino
}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, long value) {
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
long EEPROMReadlong(long address) {
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void loop() {
  voltage = analogRead(A0);
  // If voltage detected, increasse the pee score.
  if(voltage > 1000) {
    if(!peeStarted) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Your score: ");
    }
    peeStarted = true;
    peeStopped = 0;
//    lcd.write(++currentScore);
    lcd.setCursor(0,1);
    lcd.print(++currentScore);
    // Update the screen to show the score
  } else if(peeStarted){ // Only execute this if they have already started peeing.
    // If no voltage for 4 seconds, finish the current Pee cycle.
    if(peeStopped < 40) {
      peeStopped++;
    } else {
      endPeeCylce();
    }
    
  }
  delay(100); //Delay for 1 tenth of a second.
}
