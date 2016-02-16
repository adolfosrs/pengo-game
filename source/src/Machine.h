#ifndef MACHINE_H
#define MACHINE_H

#include "Player.h"
#include "Coordinate.h"

class Map;

class Machine: public Player {
private:
	bool isInitialized;


public:
	// constructs a new machine player in the position
	Machine(Coordinate position);

	// initialize the model
	void init();

	// generate a move for the machine
	void move(Map &map);
};

#endif