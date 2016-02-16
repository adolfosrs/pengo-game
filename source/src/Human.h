#ifndef HUMAN_H
#define HUMAN_H

#include "Player.h"

class Human: public Player {
private:
	bool isInitialized;

public:
	Human(Coordinate position);
	void init();
};

#endif
