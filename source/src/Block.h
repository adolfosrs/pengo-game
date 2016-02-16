#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <memory>
#include "3DObject.h"
#include "Coordinate.h"

class Block {
private:
	// indicates if the block has an model object
	bool hasModelObject_;

protected:
	// size of the collision cube
	float collisionSideSize_;

	// the model that will be printed in the block
	std::shared_ptr<C3DObject> modelObject_;

	// previous translation executed
	Coordinate lastTranslation_;

	/// position
	Coordinate position_;

	//Coordinate forwardPosition_;

	/// orientation in degrees
	float angle_;

public:
	// block side size
	static const float EDGE_SIZE;

	// construct a new block in the given position
	Block(Coordinate position, float collisionSideSize);

	// creates a block without position. The position will be origin (0,0,0)
	Block(float collisionSideSize);

	// construct a new block with the model modelObject and that is
	// centered in position
	Block(std::shared_ptr<C3DObject> modelObject, Coordinate position, float collisionSideSize);
	~Block();

	// set the position of the block
	void setPosition(Coordinate position);

	// get the angle of the block
	float angle();

	// set the model object of this block
	void setModelObject(std::shared_ptr<C3DObject> modelObject);

	// return the size of the side of the collision cube
	float collisionSideSize();

	// check if this block has a model
	bool hasModelObject();

	// return the position of the block center
	Coordinate position();

	/// draw the model
	virtual void draw();

	/// check if this block collides with another block
	bool collidesWith(Block &block);

	/// rotate the model in the block
	void rotateModel(float fAngle);

	/// translate the block
	void translate(float fX, float fY, float fZ);
};

#endif