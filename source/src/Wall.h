#ifndef WALL_H
#define WALL_H

#include "Block.h"
#include "Coordinate.h"

class Wall: public Block {
private:
	bool isInitialized;

public:
	// constructs a new indestructible wall in the position
	Wall(Coordinate position);

	// initialize the model
	void init();
};

#endif