#include "Human.h"

Human::Human(Coordinate position) : Player(position) {
	isInitialized = false;
}

void Human::init() {
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