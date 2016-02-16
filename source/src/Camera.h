#ifndef CAMERA_H
#define CAMERA_H

#include "Coordinate.h"

class Camera {
private:
	// camera position
	Coordinate eye_;

	// look at vector
	Coordinate lookAt_;

	// up vector
	Coordinate up_;

public:
	Camera(Coordinate eye, Coordinate lookAt, Coordinate up);

	// move the camera forward
	void moveForward(float step);

	// move the camera forward
	void moveBackward(float step);

	// rotate the camera in the Y axis. Angle is interpreted in degrees.
	void rotate(float angle);

};

#endif