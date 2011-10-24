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


#include "StdAfx.h"
#include "GameObject.h"

IDirect3DDevice9 *GameObject::md3dDev = NULL;

GameObject::GameObject(void):mRoll(0.0),mPitch(0.0),mYaw(0.0),
mPosition(0.0,0.0,0.0),mDir(0.0,0.0,0.0), mIndBuf(NULL), mVtxBuf(NULL)
{
}

GameObject::~GameObject(void)
{
}

int GameObject::initGeom(void)
{
	return 0;
}

int GameObject::render(int time)
{

	return 0;
}

int GameObject::setd3dDev(IDirect3DDevice9 *d3dDev)
{
	if (md3dDev == NULL) {
		md3dDev = d3dDev;
	} else return (1);			//Device already set
	return 0;
}

