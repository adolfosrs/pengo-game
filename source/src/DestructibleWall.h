#ifndef DESTRUCTIBLEWALL_H
#define DESTRUCTIBLEWALL_H

#include <gl/glut.h>
#include "bitmap.h"
#include "Block.h"
#include "Coordinate.h"

class DestructibleWall: public Block {
private:
	bool isInitialized;
	GLubyte	    *bits_;
	BITMAPINFO	*info_;
	GLenum      type_;
	GLuint      *texture_;
	GLubyte	    *rgba_;
	GLubyte	    *rgbaptr_;
	GLubyte     *ptr_;

public:
	// constructs a new destructible wall in the position
	DestructibleWall(Coordinate position);

	void loadTexture(GLuint *texture);
	void draw();

	// initialize the model
	void init();
};

#endif