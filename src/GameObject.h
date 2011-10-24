//=============================================================================
// gameObtjec.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. handles the common properties and functios of a game object 

//
// Description:
//--------------

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

#include "d3dx9.h"
#include "d3dx9math.h"

class GameObject
{
	// functions
public:
	GameObject(void);
	~GameObject(void);
	virtual int initGeom(void);
	virtual int render(int time);


	//members
public:
	
	D3DXVECTOR3 mPosition;	// position of object
	D3DXVECTOR3 mDir;	// dirction of object
	float mRoll;			// angle of rotation around z-axis
	float mPitch;		// angle of rotation around x-axis
	float mYaw;			// angle of rotation around y-axis
	float mScaleX;		// scale in x
	float mScaleY;		// scale in x
	float mScaleZ;		// scale in x

	static IDirect3DDevice9 *md3dDev;
	static int setd3dDev(IDirect3DDevice9 *d3dDev);  // setting the d3dDev global variable


	IDirect3DIndexBuffer9 *mIndBuf;
	IDirect3DVertexBuffer9 *mVtxBuf;

};
