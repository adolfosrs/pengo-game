#ifndef MAP_H
#define MAP_H

#include <memory>
#include <vector>
#include "Block.h"
#include "Machine.h"
#include "Human.h"
#include "MotionBlock.h"
#include "Wall.h"
#include "DestructibleWall.h"
#include "Item.h"

class Map {
private:
	std::vector<std::shared_ptr<Machine>> enemies_;
	std::shared_ptr<Human> player_;
	std::vector<std::shared_ptr<Wall>> walls_;
	std::vector<std::shared_ptr<DestructibleWall>> destructibleWalls_;
	std::vector<std::shared_ptr<MotionBlock>> motionBlocks_;
	std::vector<std::shared_ptr<Item>> items_;
	float width_;
	float height_;

public:
	Map();
	float width();
	float height();
	std::shared_ptr<Human> player();
	std::vector<std::shared_ptr<Machine>> enemies();
	std::vector<std::shared_ptr<Wall>> walls();
	std::vector<std::shared_ptr<MotionBlock>> motionBlocks();
	std::vector<std::shared_ptr<Item>> items();
	std::vector<std::shared_ptr<DestructibleWall>> destructibleWalls();
	std::shared_ptr<MotionBlock> Map::getCollidedMotionBlock(Block &block);
	bool collidesWithAnotherMBlock(std::shared_ptr<MotionBlock> &block);

	void load();
	void reload();
	void draw();
	bool collidesWithSomething(Block &block);
	bool collidesWithWall(Block &block);
	bool collidesWithEnemy(Block &block);
	bool collidesWithPlayer(Block &block);
	std::shared_ptr<MotionBlock> Map::initMotionBlock(Coordinate position, int index);
	std::shared_ptr<Item> initItem(Coordinate position, int itemType);
	//void initMotionBlock(Coordinate position, int index);
	//void initMotionBlocks();
	void pushMotionBlock(MotionBlock mBlock);
	bool collidesWithMotionBlock(Block &block);
	void eraseCollidedMotionBlock(std::shared_ptr<MotionBlock> &block);
	void killWhatCollidesWith(Block &block);
	void collectItems();
	void executeMachine();
};

#endif