#define _USE_MATH_DEFINES
#include <cmath>
#include "Camera.h"
#include "Vetor3D.h"
#include "MathConstants.h"

Camera::Camera(Coordinate eye, Coordinate lookAt, Coordinate up) : eye_(eye), lookAt_(lookAt), up_(up) {
}

void Camera::moveForward(float step) {
	Coordinate aux, previousDirection;
	float futureX, futureZ;

	previousDirection.x = lookAt_.x - eye_.x;
	previousDirection.y = lookAt_.y - eye_.y;
	previousDirection.z = lookAt_.z - eye_.z;

	aux.x = previousDirection.x;
	aux.y = previousDirection.y;
	aux.z = previousDirection.z;

	float auxArray[] = {aux.x, aux.y, aux.z};
	Normaliza3D(auxArray);
	aux.x = auxArray[0];
	aux.y = auxArray[1];
	aux.z = auxArray[2];

	futureX = eye_.x + aux.x * step;
	futureZ = eye_.z + aux.z * step;

	eye_.x = futureX;
	eye_.z = futureZ;

	lookAt_.x = eye_.x + previousDirection.x;
	lookAt_.y = eye_.y + previousDirection.y;
	lookAt_.z = eye_.z + previousDirection.z;
}

void Camera::moveBackward(float step) {
	moveForward(-step);
}

void Camera::rotate(float angle) {
	Coordinate aux;

	aux.x = lookAt_.x - eye_.x;
	aux.y = lookAt_.y - eye_.y;
	aux.z = lookAt_.z - eye_.z;

	float auxArray[] = {aux.x, aux.y, aux.z};
	RotacionaEmY3D(auxArray, angle*(PI/180));
	Normaliza3D(auxArray);
	aux.x = auxArray[0];
	aux.y = auxArray[1];
	aux.z = auxArray[2];

	lookAt_.x = eye_.x + aux.x;
	lookAt_.y = eye_.y + aux.y;
	lookAt_.z = eye_.z + aux.z;
}