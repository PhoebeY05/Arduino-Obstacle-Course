#ifndef Room_h
#define Room_h
#include "Arduino.h"

class Room{
	public:
		//attributes
	  int room_number = 0;
	  int room_coords[8][2] = {0};
	  //init
	  Room(int number);
	  //methods
	  void increase();
	  void Room::seed(int time);
	  void generate_room(int index);
	  void reset_room();
};
#endif
