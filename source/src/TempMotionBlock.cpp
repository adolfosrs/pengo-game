#include "TempMotionBlock.h"
#include "MathConstants.h"
#include <iostream>

TempMotionBlock::TempMotionBlock(Coordinate position) : Block(position, Block::EDGE_SIZE) {
	isInitialized = false;
}

TempMotionBlock::TempMotionBlock() : Block(Block::EDGE_SIZE) {
	isInitialized = false;
}

void TempMotionBlock::moveForward() {
	float x = 0.02f * sin(angle_*PI/180) * 2;
	float z = -0.02f * cos(angle_*PI/180) * 2;

	translate(x, 0.0f, z);
}

void TempMotionBlock::init() {
	if(!isInitialized) {
		// create a smart pointer to the model
		std::shared_ptr<C3DObject> model = std::make_shared<C3DObject>();
		
		model->Load("../resources/models/crate.obj");

		// set the model of the block
		setModelObject(model);

		isInitialized = true;
	}
}

void TempMotionBlock::loadTexture(GLuint *texture) {

	bits_ = LoadDIBitmap("..\\resources\\models\\crate.bmp", &info_);
	if (bits_ == (GLubyte *)0)
	{
		std::cout << "Error loading player texture!" << std::endl;
		return;
	}


	// Figure out the type of texture
	if (info_->bmiHeader.biHeight == 1)
		type_ = GL_TEXTURE_1D;
	else
		type_ = GL_TEXTURE_2D;

	// Create and bind a texture object
	texture_ = texture;
	glGenTextures(1, texture_);
	glBindTexture(type_, *texture_);

	// Create an RGBA image
	rgba_ = (GLubyte *)malloc(info_->bmiHeader.biWidth * info_->bmiHeader.biHeight * 4);

	int i = info_->bmiHeader.biWidth * info_->bmiHeader.biHeight;
	for( rgbaptr_ = rgba_, ptr_ = bits_;  i > 0; i--, rgbaptr_ += 4, ptr_ += 3)
	{
		rgbaptr_[0] = ptr_[2];     // windows BMP = BGR
		rgbaptr_[1] = ptr_[1];
		rgbaptr_[2] = ptr_[0];
		rgbaptr_[3] = (ptr_[0] + ptr_[1] + ptr_[2]) / 3;
	}

	// Set texture parameters
	glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type_, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(type_, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(type_, 0, 4, info_->bmiHeader.biWidth, info_->bmiHeader.biHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, rgba_ );
}

void TempMotionBlock::draw() {
	if(modelObject_) {
		glPushMatrix();
		glBindTexture(type_, *texture_);
		glShadeModel(GL_SMOOTH);
		glEnable(type_);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// treat position as a real position, not a position variation
		glTranslatef(position_.x, position_.y, position_.z);
		//glRotatef(-angle_, 0.0f, 1.0f, 0.0f);
		modelObject_->DrawWithTexture();

		glDisable(type_);

		glPopMatrix();
	}
}