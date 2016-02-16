#include "Block.h"
#include "Vetor3D.h"
#include <gl/glut.h>
#include <cmath>
#include <iostream>

const float Block::EDGE_SIZE = 2.0f;

Block::Block(Coordinate position, float collisionSideSize) : position_(position), collisionSideSize_(collisionSideSize) {
	hasModelObject_ = false;
	lastTranslation_ = position;
	angle_ = 0;
}

Block::Block(float collisionSideSize) : collisionSideSize_(collisionSideSize) {
	position_.x = 0;
	position_.y = 0;
	position_.z = 0;
}

Block::Block(std::shared_ptr<C3DObject> modelObject, Coordinate position, float collisionSideSize)
	: modelObject_(modelObject), position_(position), collisionSideSize_(collisionSideSize)  {
	hasModelObject_ = true;
	lastTranslation_ = position;
	angle_ = 0;
}

Block::~Block() {
}

void Block::setPosition(Coordinate position) {
	position_ = position;
}

float Block::collisionSideSize() {
	return collisionSideSize_;
}

void Block::setModelObject(std::shared_ptr<C3DObject> modelObject) {
	hasModelObject_ = true;
	modelObject_ = modelObject;
}

bool Block::hasModelObject() {
	return hasModelObject_;
}

Coordinate Block::position() {
	return position_;
}


float Block::angle() {
	return angle_;
}

void Block::draw() {
	if(modelObject_) {
		glPushMatrix();

		// treat position as a real position, not a position variation
		glTranslatef(position_.x, position_.y, position_.z);
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(angle_, 0.0f, 1.0f, 0.0f);
		modelObject_->Draw();

		glPopMatrix();
	}
}

bool Block::collidesWith(Block &block) {
	// verify if they don't collide in x
	if ((position_.x + collisionSideSize_/2) <= (block.position().x - collisionSideSize_/2)) { 
		return false;   
	}     
	if ((position_.x - collisionSideSize_/2) >= (block.position().x + collisionSideSize_/2)) {         
		return false;  
	}

	// verify if they don't collide in z
	if ((position_.z - collisionSideSize_/2) >= (block.position().z + collisionSideSize_/2)) {     
		return false;   
	} 
	if ((position_.z + collisionSideSize_/2) <= (block.position().z - collisionSideSize_/2)) {        
		return false; 
	}

	return true;
}

void Block::rotateModel(float angle) {
	angle_ += angle;

	// keep the angles allways between 0 and 360
	if(angle_ > 360.0f) {
		angle_ -= 360.0f * ((int)angle_ / 360);
	}
	if(angle_ < 0) {
		angle_ += 360.0f * (((int)angle_ / 360) + 1);
	}
}

void Block::translate(float x, float y, float z) {
	lastTranslation_.x = x;
	lastTranslation_.y = y;
	lastTranslation_.z = z;
	position_.x += x;
	position_.y += y;
	position_.z += z;
}