#include "Machine.h"
#include "Map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Machine::Machine(Coordinate position) : Player(position) {
	isInitialized = false;

}

void Machine::init() {
	if(!isInitialized) {
		// create a smart pointer to the model
		std::shared_ptr<C3DObject> model = std::make_shared<C3DObject>();

		if (!model->Load("../resources/models/penguin.obj")) {
			std::cout << "Problem loading machine player model!" << std::endl;
			return;
		}

		// set the model of the block
		setModelObject(model);

		isInitialized = true;
	}
}

void Machine::move(Map &map) {

	if(map.player()) {
		Coordinate playerPosition = map.player()->position();

		// distance from this machine to the player
		float distanceToPlayer = sqrt(
			pow(position_.x - playerPosition.x, 2.0f) + 
			pow(position_.y - playerPosition.y, 2.0f) +
			pow(position_.z - playerPosition.z, 2.0f));

		if(distanceToPlayer < Block::EDGE_SIZE * 4) {
			// ok, player is near this machine. GO KILL THEM!
			float angle = -1.0f;
			float betterDistance = distanceToPlayer;
			float newDistance = 0; 
		
			// angle = 0
			Player fakeMove = Player(position_);
			fakeMove.rotateModel(angle_ + 0.0f);
			fakeMove.moveForward();

			newDistance = sqrt(
				pow(fakeMove.position().x - playerPosition.x, 2.0f) + 
				pow(fakeMove.position().y - playerPosition.y, 2.0f) +
				pow(fakeMove.position().z - playerPosition.z, 2.0f));

			if(newDistance < betterDistance &&  !map.collidesWithWall(fakeMove)
				&& !map.collidesWithMotionBlock(fakeMove)) {
				betterDistance = newDistance;
				angle = 0.0f;
			}

			// angle = 90
			fakeMove = Player(position_);
			fakeMove.rotateModel(angle_ + 90.0f);
			fakeMove.moveForward();

			newDistance = sqrt(
				pow(fakeMove.position().x - playerPosition.x, 2.0f) + 
				pow(fakeMove.position().y - playerPosition.y, 2.0f) +
				pow(fakeMove.position().z - playerPosition.z, 2.0f));

			if(newDistance < betterDistance &&  !map.collidesWithWall(fakeMove)
				&& !map.collidesWithMotionBlock(fakeMove)) {
				betterDistance = newDistance;
				angle = 90.0f;
			}

			// angle = 180
			fakeMove = Player(position_);
			fakeMove.rotateModel(angle_ + 180.0f);
			fakeMove.moveForward();

			newDistance = sqrt(
				pow(fakeMove.position().x - playerPosition.x, 2.0f) + 
				pow(fakeMove.position().y - playerPosition.y, 2.0f) +
				pow(fakeMove.position().z - playerPosition.z, 2.0f));

			if(newDistance < betterDistance &&  !map.collidesWithWall(fakeMove)
				&& !map.collidesWithMotionBlock(fakeMove)) {
				betterDistance = newDistance;
				angle = 180.0f;
			}

			// angle = 270
			fakeMove = Player(position_);
			fakeMove.rotateModel(angle_ + 270.0f);
			fakeMove.moveForward();

			newDistance = sqrt(
				pow(fakeMove.position().x - playerPosition.x, 2.0f) + 
				pow(fakeMove.position().y - playerPosition.y, 2.0f) +
				pow(fakeMove.position().z - playerPosition.z, 2.0f));

			if(newDistance < betterDistance &&  !map.collidesWithWall(fakeMove)
				&& !map.collidesWithMotionBlock(fakeMove)) {
				betterDistance = newDistance;
				angle = 270.0f;
			}


			// ok, choosed best move. So do it!
			if(angle >= 0.0f) {
				rotateModel(angle);
				moveForward();
			}

		} else {
			// player is pretty far... do whatever you want

			// try to walk forward
			Player fakeMove = Player(position_);
			fakeMove.rotateModel(angle_);
			fakeMove.moveForward();

			bool randomize = std::rand()%100 >= 80; // 20% chance that it will randomize

			if(!randomize && !map.collidesWithWall(fakeMove) && !map.collidesWithMotionBlock(fakeMove)) {
				// if machine can walk forward, walk forward
				moveForward();

			} else {
				// if cannot walk forward, rotate and try to walk
				bool moved = false;
				int available = 4;

				// find a possible move
				while (!moved && available > 0) {
					float angles[] = {0.0f, 90.0f, 180.0f, 270.0f};
					int selected = std::rand() % available;

					Player fakeMove = Player(position_);
					fakeMove.rotateModel(angle_ + angles[selected]);
					fakeMove.moveForward();

					if(!map.collidesWithWall(fakeMove) && !map.collidesWithMotionBlock(fakeMove)) {
						rotateModel(angles[selected]);
						moveForward();
						moved = true;
					} else {
						float tmp = angles[available-1];
						angles[available-1] = angles[selected];
						angles[selected] = tmp;
						available -= 1;
					}
				}
			}
		}
	}
}