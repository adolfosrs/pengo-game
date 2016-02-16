#ifndef MOTIONBLOCK_H
#define MOTIONBLOCK_H

#include "Block.h"
#include "Coordinate.h"
#include "bitmap.h"

class MotionBlock: public Block {
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

	bool isMoving_;
	int index_;
	float angleToGo_;
	bool isFixed_;
	bool hasItem_;
	int itemType_;
public:

	MotionBlock(Coordinate position);

	MotionBlock();
	bool hasItem();
	void setHasItem(bool hasItem);
	int itemType();
	void setItemType(int itemType);
	bool isMoving();
	void setIsMoving(bool isMoving);
	void setIsFixed(bool isFixed);
	bool isFixed();
	int index();
	void setIndex(int index);
	// initialize the model
	void init();
	void loadTexture(GLuint *texture);
	void draw();
	void setAngleToGo(float angle);
	void moveForward();
	void moveBackward();
};

#endif