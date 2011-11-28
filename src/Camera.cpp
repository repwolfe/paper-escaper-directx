
//=============================================================================
// Camera.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a Camera
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a Camera 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================


#include "StdAfx.h"
#include "Camera.h"


/******************************************************************/
/*
Purpose: Camera constructor


Descripton: 

Return:


*/

Camera::Camera(void): position(0.0,0.0,-50.0), lookAtVector(0.0,0.0,1.0), upVector(0.0,1.0,0.0)
, speed(0)
{

}

/******************************************************************/
/*
Purpose: Camera destructor


Descripton: 

Return:


*/
Camera::~Camera(void)
{
}


/******************************************************************/
/*
Purpose: change the orientation of the Camera (roll transformation)


Descripton: 

Return:
1 - if failed
0 - if successful

*/


int Camera::roll(float angleDeg)
{
	//removed roll
	/*float angleRad = D3DXToRadian(angleDeg);
	D3DXVECTOR3 rotVector(0.0,0.0,0.0);


	// get rotation axis
	rotVector = lookAtVector;

	updateOrientation(rotVector, angleRad);

	*/
	return 0;
	
}


/******************************************************************/
/*
Purpose: change the orientation of the Camera (pitch transformation)


Descripton: 

Return:
1 - if failed
0 - if successful

*/

int Camera::pitch(float angleDeg)
{
	float angleRad = D3DXToRadian(angleDeg);
	D3DXVECTOR3 rotVector(0.0,0.0,0.0);

	// get rotation axis
	D3DXVec3Cross(&rotVector, &lookAtVector, &upVector);
	
	updateOrientation(rotVector, angleRad);

	//undo rotation
	//D3DXVECTOR3 midVector = lookAtVector + upVector;
	D3DXVECTOR3 xVector(1.0,0.0,0.0);
	float dot = D3DXVec3Dot(&xVector, &lookAtVector);
	dot = acos(dot);
	if(dot < 0 && dot > (3.14/2) ){
		updateOrientation(rotVector, -angleRad);
	}

	return 0;
}

/******************************************************************/
/*
Purpose: change the orientation of the Camera (yaw transformation)


Descripton: 

Return:
1 - if failed
0 - if successful

*/



int Camera::yaw(float angleDeg)
{
	float angleRad = D3DXToRadian(angleDeg);
	D3DXVECTOR3 rotVector(0.0,1.0,0.0);

	// get rotation axis
	//rotVector = upVector;

	updateOrientation(rotVector, angleRad);
	return 0;
}

/******************************************************************/
/*
Purpose: obtains the current position of the Camera


Descripton: 

Return:
D3DXVECTOR3 - Camera position

*/

D3DXVECTOR3 Camera::getPosition(void)
{
	return (position);
}


/******************************************************************/
/*
Purpose: obtains the lookAt point of the Camera


Descripton: 

Return:
D3DXVECTOR3 - Camera lookAt point

*/


D3DXVECTOR3 Camera::getLookAtPoint(void)
{
	return (position + lookAtVector);
}

/******************************************************************/
/*
Purpose: obtains the Camera's up vector


Descripton: 

Return:
D3DXVECTOR3 - Camera upVector

*/



D3DXVECTOR3 Camera::getUpVector(void)
{
	return (upVector);
}


/******************************************************************/
/*
Purpose: changes the Camera's position relative to its current position


Descripton: 

Return:
0 - success

*/


int Camera::changePositionDelta(float dx, float dy, float dz)
{
	position.x += dx;
	position.y += dy;
	position.z += dz;

	return 0;
}


/******************************************************************/
/*
Purpose: changes the Camera's position relative to its current position


Descripton: 

Return:
0 - success

*/


int Camera::changePositionDelta(D3DXVECTOR3 *dv)
{
	position += *dv;
	return 0;
}

/******************************************************************/
/*
Purpose: changes the Camera's position to a new position


Descripton: 

Return:
0 - success

*/


int Camera::changeAbsPoition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return 0;
}

/******************************************************************/
/*
Purpose: changes the Camera's position to a new position


Descripton: 

Return:
0 - success

*/

int Camera::changeAbsPosition(D3DXVECTOR3 *v)
{
	position = *v;
	return 0;
}

/******************************************************************/
/*
Purpose: changes the Camera's position along the LootAt vector


Descripton: move the Camera forward by the numUnits along the looAtVector

Return:
the new position

*/


D3DXVECTOR3 Camera::moveForward(float numUnits)
{
	D3DXVECTOR3 vec;
	D3DXVec3Normalize(&vec, &lookAtVector);	// make vec length 1
	vec.y = 0;
	D3DXVec3Normalize(&vec, &vec);
	position += vec * numUnits;				// add numUnits lengthed vec
	if(position.x > maxx){
		position.x = maxx;
	}else if(position.x < minx){
		position.x = minx;
	}
	if(position.z > maxz){
		position.z = maxz;
	}else if(position.z < minz){
		position.z = minz;
	}
	return (position);
}

D3DXVECTOR3 Camera::moveRight(float numUnits)
{
	D3DXVECTOR3 vec;
	//D3DXVec3Normalize(&vec, &lookAtVector);	// make vec length 1
	D3DXVec3Cross(&vec, &lookAtVector, &upVector);
	vec.y = 0;
	position += vec * numUnits;				// add numUnits lengthed vec
	if(position.x > maxx){
		position.x = maxx;
	}else if(position.x < minx){
		position.x = minx;
	}
	if(position.z > maxz){
		position.z = maxz;
	}else if(position.z < minz){
		position.z = minz;
	}
	return (position);
}
/******************************************************************/
/*
Purpose: 


Descripton: 

Return:


*/


int Camera::updateOrientation(D3DXVECTOR3 rotVector, float angleRad)
{

	D3DXVECTOR3 xaxis(0.0,0.0,0.0);

	// create rotation matrix
	D3DXMatrixRotationAxis(&rotMat, &rotVector,angleRad);

	// rotate the Camera (up vector and/or lookAtVector)
	D3DXVec3TransformCoord(&upVector, &upVector, &rotMat);
	D3DXVec3TransformCoord(&lookAtVector, &lookAtVector, &rotMat);

	// update the upVector
	D3DXVec3Cross(&xaxis, &upVector, &lookAtVector);
	D3DXVec3Cross(&upVector,  &lookAtVector, &xaxis);
	D3DXVec3Normalize(&upVector, &upVector);
	D3DXVec3Normalize(&lookAtVector, &lookAtVector);

	return 0;
}

/******************************************************************/
/*
Purpose: obtains the view transformation matrix


Descripton: 

Return:
the transformation matrix

*/



D3DMATRIX * Camera::getViewMatrix(D3DXMATRIX * viewMatrix)
{
	D3DXVECTOR3 lookAt;
	
	lookAt = position+lookAtVector;

	return(D3DXMatrixLookAtLH(viewMatrix, &position,&lookAt, &upVector));
}

/******************************************************************/
/*
Purpose: set the Camera parameters


Descripton: 

Return:


*/


void Camera::setCamera(D3DXVECTOR3 position, D3DXVECTOR3 lookAtPoint, D3DXVECTOR3 upVector)
{
	this->position = position;
	this->lookAtVector = lookAtPoint - position;
	this->upVector = upVector;
	D3DXVec3Normalize(&this->upVector, &this->upVector);
	D3DXVec3Normalize(&this->lookAtVector, &this->lookAtVector);

}
// change the speed of the Camera motion
int Camera::updateSpeed(float speed)
{
	this->speed += speed;
	return 0;
}

float Camera::getSpeed(void)
{
	return(speed);
}

int Camera::setBoundingBox(float BBminx, float BBmaxx, float BBminz, float BBmaxz){
	minx = BBminx;
	maxx = BBmaxx;
	minz = BBminz;
	maxz = BBmaxz;
	return 0;
}