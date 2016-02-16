#ifndef TEMPMOTIONBLOCK_H
#define TEMPMOTIONBLOCK_H

#include "Coordinate.h"
#include "bitmap.h"
#include "Block.h"

class TempMotionBlock: public Block {
private:
	bool isInitialized;

	// texture stuff
	GLubyte	    *bits_;
	BITMAPINFO	*info_;
	GLenum      type_;
	GLuint      *texture_;
	GLubyte	    *rgba_;
	GLubyte	    *rgbaptr_;
	GLubyte     *ptr_;

public:

	TempMotionBlock(Coordinate position);

	TempMotionBlock();

	// initialize the model
	void init();
	void loadTexture(GLuint *texture);
	void draw();

	void moveForward();
};

#endif