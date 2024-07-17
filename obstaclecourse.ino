#include <LiquidCrystal.h>
#include <ezButton.h>
#include "Room.h"

#define VRX_PIN  A0 //Arduino pin connected to VRX pin
#define VRY_PIN  A1 //Arduino pin connected to VRY pin
#define SW_PIN   13  //Arduino pin connected to SW  pin

#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

int xValue = 0 ; // To store value of the X axis
int yValue = 0 ; // To store value of the Y axis
int command = COMMAND_NO;
int room = 0;
Room new_room(room);
bool cleared = false;

const int rs = 12, 
          en = 3, 
          d4 = 6, 
          d5 = 7, 
          d6 = 8, 
          d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i = 0; // x coordinate of lcd
int j = 1; // y coordinate of lcd
int input = 0; //for incoming serial data

//player character
byte stand[8] = { 
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b01010,
  0b01010
};

///clear character (completely empty)
byte empty[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

//obstacle character (completely filled)
byte obstacle[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// Room current = Room;

void setup() {
  Serial.begin(9600);
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2); 
  // create a new character
  lcd.createChar(1, stand);
  lcd.createChar(2, empty);
  lcd.createChar(3, obstacle);
  // set the cursor to the top left
  lcd.setCursor(0, 1);
  lcd.write(1);
}

void loop() {
  int delayTime = 300; 
  new_room.seed(millis()); //create seed for current iteration
  if (cleared) { //code for initialising next level
    if (new_room.room_number == 0) { //if there is no next level
      lcd.clear();
      lcd.print("You Won!"); 
      delay(5000);
      lcd.clear();
      //reinitialise coordinates of player
      i = 0;
      j = 1;
    }
    for (int i = 0; i < new_room.room_number; i++) { //if there is next level
      new_room.generate_room(i); //generate new coordinates
    }
    for (int a = 0; a < new_room.room_number; a++) { //initialise obstacles
      int row = new_room.room_coords[a][0];
      int col = new_room.room_coords[a][1];
      delay(10);
      lcd.setCursor(row, col);
      lcd.write(3); //creating obstacle character at specified coordinate
    }
    //create player (stand character) at specified coordinates
    lcd.setCursor(i, j);
    lcd.write(1);
    cleared = false;
  }
  else {
		 //check if player bumps into obstacle
     for (int a = 0; a < new_room.room_number; a++) {//iterate through every obstacle
      if (i == new_room.room_coords[a][0] && j == new_room.room_coords[a][1]) { /*check if
	    player coordinates overlaps with any obstacle's coordinates*/
        lcd.clear();
        lcd.print("GAME OVER");
        delay(5000);
        lcd.clear();
        //reinitialise coordinates of player
        i = 0;
        j = 1;
        //create player (stand character) at specified coordinates
        lcd.setCursor(i, j);
        lcd.write(1);
        //reset room_number attribute
        new_room.reset_room();
      }
     }
  }
  
  // read analog X and Y analog values
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);
  
  // converts the analog value to commands
  // reset commands
  command = COMMAND_NO;

  // check left/right commands
  if (xValue < LEFT_THRESHOLD)
    command = command | COMMAND_LEFT;
  else if (xValue > RIGHT_THRESHOLD)
    command = command | COMMAND_RIGHT;

  // check up/down commands
  if (yValue < UP_THRESHOLD)
    command = command | COMMAND_UP;
  else if (yValue > DOWN_THRESHOLD)
    command = command | COMMAND_DOWN;

  // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS

  //print command to serial and process command
  if (command & COMMAND_UP) {
    Serial.println("COMMAND UP");
    if (j != 0){ //if not at top row
      //erase previous coordinate's player (stand character)
      lcd.setCursor(i, j);
      lcd.write(2);
      //move player's coordinates to top row
      j = 0;
      //create player (stand character) at specified coordinates
      lcd.setCursor(i, j);
      lcd.write(1);
    }
    //don't do anything if already at top row
  }

  if (command & COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
    if (i != 0){ //if not at leftmost column
      //erase previous coordinate's player (stand character)
      lcd.setCursor(i, j);
      lcd.write(2);
      //move player's coordinates to the left by one
      i -= 1;
      //create player (stand character) at specified coordinates
      lcd.setCursor(i, j);
      lcd.write(1);
    }
    //don't do anything if already at leftmost column
  }
  
  if (command & COMMAND_DOWN) {
    Serial.println("COMMAND DOWN");
    if (j != 1){ //if not at bottom row
      //erase previous coordinate's player (stand character)
      lcd.setCursor(i, j);
      lcd.write(2);
      //move player's coordinates to top row
      j = 1;
      //create player (stand character) at specified coordinates
      lcd.setCursor(i, j);
      lcd.write(1);
    }
    //don't do anything if already at bottom row
  }
  if (command & COMMAND_RIGHT) {
    Serial.println("COMMAND RIGHT");
    if (i == 15){ //if at rightmost column
	    //erase previous coordinate's player (stand character)
      lcd.setCursor(i, j);
      lcd.write(2);
      //move player to start of new room
      i = 0;
      cleared = true;
      //increase room_number attribute
      new_room.increase();
      lcd.clear();
      delayTime = 500;
    }
    else {
	    //erase previous coordinate's player (stand character)
	    lcd.setCursor(i, j);
      lcd.write(2);
      //move player's coordinates to the right by one
      i += 1;
    }
    //create player (stand character) at specified coordinates
    lcd.setCursor(i, j);
    lcd.write(1);
  }  
  delay(delayTime);
}
