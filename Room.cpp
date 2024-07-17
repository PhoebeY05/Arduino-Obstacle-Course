// Room.cpp
#include <stdio.h>
#include <stdlib.h>
#include "Room.h"
#include <Time.h>

//declaration for generate_room
int x = 0;
int y = 0;

Room::Room(int number) { //init function
    this->room_number = number;
}

void Room::increase(){ //increase room number until 8 where it restarts
    if (this->room_number < 8) {
      this->room_number++;
    }
    else {
      this->room_number = 0;
    }
}
void Room::seed(int time) { //create seed for generate_room function
  srand(time);
}

void Room::generate_room(int index){
  bool invalid = true; //continue generating new coordinates until they are valid
  while (invalid) {
	  //16x2 LCD: 0 <= x <= 15, 0 <= y <= 1
    x = rand() % 15 + 1; //random integer from 1 to 15 (inclusive)
    y = rand() % 2; //random number that is 0 or 1 
    bool passed = true; //check if new obstacle is at an impossible position
    for (int i = 0; i < this->room_number; i++) { /*iterating through all
	  the coordinates in room_coords*/
      int x1 = this->room_coords[i][0]; 
      int y1 = this->room_coords[i][1];
      if (x == x1) { 
		      //on top or on each other
          passed = false;
      }
      else if ((x == x1 + 1 || x == x1 - 1) && y != y1) { 
	      //adjcent horizontally and opposite vertically (i.e. one is up, one is down)
          passed = false;
      }
      else {
          continue;
      }
    }
    if (passed) { //add new coordinates if they are not in an impossible position
      this->room_coords[index][0] = x;
      this->room_coords[index][1] = y;
      invalid = false;
    }
  }
}

void Room::reset_room() { //reset function for game over
  this->room_number = 0;
}

