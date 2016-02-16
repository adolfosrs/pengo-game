#ifndef PLAYER_H
#define PLAYER_H

#include <gl/glut.h>
#include "bitmap.h"
#include "Block.h"
#include "Coordinate.h"
#include "Item.h"

class Player: public Block {
private:
	GLubyte	    *bits_;
	BITMAPINFO	*info_;
	GLenum      type_;
	GLuint      *texture_;
	GLubyte	    *rgba_;
	GLubyte	    *rgbaptr_;
	GLubyte     *ptr_;
	
	// power ups
	int mBlocks_;
	float speed_;

public:
	Player(Coordinate position);

	void loadTexture(GLuint *texture);

	// draw the player with its texture
	void draw();

	// move the player to front
	void moveForward();

	// move the player to behind
	void moveBackward();

	// add an item to the player
	void addPowerUp(ItemType itemType);

	int mBlocks();
};

#endif