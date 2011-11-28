//=============================================================================
// camera.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a camera
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a camera 
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

#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
public:
	Camera(void);
	~Camera(void);
	int roll(float angleDeg);
	int pitch(float angleDeg);
	int yaw(float angleDeg);
	D3DXVECTOR3 getPosition(void);
	D3DXVECTOR3 getLookAtPoint(void);
	D3DXVECTOR3 getUpVector(void);
	int changePositionDelta(float dx, float dy, float dz);	// change position by relative amount
	int changePositionDelta(D3DXVECTOR3 *dv);				// change position by relative amount
	int changeAbsPoition(float x, float y, float z);		// change to a new position in space
	int changeAbsPosition(D3DXVECTOR3 *v);					// change to a new position in space
	D3DXVECTOR3 moveForward(float numUnits);  // moves the camera forward by the numUnits units along the lookAtVector
	D3DXVECTOR3 moveRight(float numUnits);
	void setCamera(D3DXVECTOR3 position, D3DXVECTOR3 lookAtPoint, D3DXVECTOR3 upVector);
	int updateSpeed(float speed);
	float getSpeed(void);
	int setBoundingBox(float minx, float maxx, float minz, float maxz);
	void setDeveloperMode();


private:
	int updateOrientation(D3DXVECTOR3 rotVector, float angleRad); // update the camera's orientation in space

	

public:
	D3DXMATRIX rotMat;
	D3DXVECTOR3 position;
	D3DXVECTOR3 upVector;
	D3DXVECTOR3 lookAtVector;
	D3DMATRIX * getViewMatrix(D3DXMATRIX * viewMatrix);
	float speed, minx, maxx,  minz,  maxz;
	// change the speed of the camera motion
	bool devMode;
};
