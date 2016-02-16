#ifndef ITEM_H
#define ITEM_H

#include "Block.h"
#include "Coordinate.h"
#include "bitmap.h"

enum ItemType {FASTER_ITEM, MBLOCK_ITEM};

class Item: public Block {
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
	
	ItemType itemType_;

public:
	// constructs a new item in the position
	Item(Coordinate position, ItemType itemType);

	// initialize the model
	void init();

	ItemType itemType();
	//void loadTexture(GLuint *texture);
	//void draw();
};

#endif