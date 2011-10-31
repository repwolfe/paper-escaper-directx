//=============================================================================
// myGame.cpp
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates the usage of the gameIO and gameApp framework 
//
//
//
// Description:
//--------------
// A simple application that draws text on the screen.  
// The application draws a text on the screen.  The text shows the text location 
// and the current frame number.  The text scrolls on the screen.
// The text scrolling changes direction when:
// a. the text reaches a boundary of the window
//
// b. the user presses the left mouse button - the left mouse button changes x-direction
// and the right mouse button changes the y-direction
//
// c. The escape key leaves the application
//
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
#include "myGame.h"

#include <stdio.h>





myGame::myGame(HINSTANCE hInstance, char* gameName):gameApp(hInstance, gameName)
, x(0), fontCourier(NULL), previous(NULL)
{

}

myGame::~myGame(void)
{
	if (fontCourier != NULL) {
		fontCourier->Release();
		fontCourier = NULL;
	}
	
	while (sheets.empty() == false) {
		delete sheets.front();
		sheets.pop_front();
	}

	delete previous;
}



int myGame::updateGameState(long time)
{
	int rc = 0;
	// poll the input
	mInput->poll();
	
	// check if escape key was pressed
	if (mInput->keyboardPressed(DIK_ESCAPE)) {
		rc = 1;
	} else {
		if (mInput->keyboardPressed(DIK_LEFT)) {
			// move left
			cam.yaw(-2.0f); // CCW
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_RIGHT)) {
			// move right
			cam.yaw(2.0f); // CW
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_UP)) {
			// pull up
			cam.pitch(2.0f);
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_DOWN)) {
			// dive
			cam.pitch(-2.0f);
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_W)) {
			// speed up
			cam.moveForward(5.0f);
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_S)) {
			// slow down
			cam.moveForward(-5.0f);
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_A)) {
			//roll left
			cam.roll(2.0f);  //CCW
			rc = 0;
		}
		if (mInput->keyboardPressed(DIK_D)) {
			// roll write
			cam.roll(-2.0f);  //CW
			rc = 0;
		}
	}

	std::list<PaperSheet*>::const_iterator iter = sheets.begin();
	while (iter != sheets.end()) {
		(*iter)->updateState();
		++iter;
	}

	// Check if last sheet is done rotating
	if (sheets.back()->shouldDelete()) {
		delete previous;					// delete the previously stored sheet
		previous = sheets.back();
		sheets.pop_back();
		if (!sheets.empty()) {
			sheets.back()->startRotating();		// start rotating the new last sheet
		}
	}

	// move the camera
	cam.moveForward(cam.getSpeed());

	return(rc);
}
/**************************************************************************/



 // renders all objects
int myGame::renderFrame(int time)
{

	int i = 0;
	int rc;
	static float step = 1;
	float rad = 0;
	D3DXMATRIX worldMat, viewMat, matTransform, matProjection, matScale, matTranslate,  matRotation;


	//// set up the camera location
	//D3DXMatrixLookAtLH(&viewMat, 
	//&D3DXVECTOR3(40.0f, 40.0f, 40.0f), // camera loc
	//&D3DXVECTOR3(150.0f, 0.0f, -150.0f),  // look-at target 
	//&D3DXVECTOR3(0.0f, 1.0f, 0.0f)); // up Veotor
	//D3DXMatrixLookAtLH(&viewMat,&D3DXVECTOR3(50,1000,50), &D3DXVECTOR3(500,0,500), &D3DXVECTOR3(0,1,0));


	// set the camera matrix
	cam.getViewMatrix(&viewMat);  // nuss1
	// inform direct3d about the view transformation
	rc = md3dDev->SetTransform(D3DTS_VIEW,&viewMat);


	//// set up the projection transformation
	//D3DXMatrixPerspectiveFovLH(&matProjection,  
	//	D3DXToRadian(80),			// field of view
	//	400/300, 					// aspect ratio
	//	1.0f, 1500.0f); 			// near and far planes

	getProjMat(&matProjection);
	// inform direc3d about the proejctions tranfsormation
	md3dDev->SetTransform(D3DTS_PROJECTION, &matProjection); 



	// clear the window to a deep blue
	rc = md3dDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);
	if (rc != D3D_OK) {
		i++;
	}
	md3dDev->BeginScene();    // begins the 3D scene

	std::list<PaperSheet*>::const_iterator iter = sheets.begin();
	while (iter != sheets.end()) {
		(*iter)->render(time);
		++iter;
	}

	if (previous != NULL) {
		previous->render(time);
	}

	md3dDev->EndScene();    // ends the 3D scene

	rc = md3dDev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
	if (rc != D3D_OK) {
		i++;
	}

	 return 0;
 }



int myGame::initGame(void)
{
	// set the intial location of the camera
	cam.setCamera(D3DXVECTOR3(-40,70,-40), D3DXVECTOR3(50,0,50), D3DXVECTOR3(0,1,0));
	//cam.setCamera(D3DXVECTOR3(0,0,1), D3DXVECTOR3(0,0,-1), D3DXVECTOR3(0,1,0));

	// initialize the projection matrix
	setProj(1.0,5000.0,D3DXToRadian(80),((float) this->mWndWidth)/this->mWndHeight);

	x = this->getWndWidth() / 2;
	y = this->getWndHeight() / 2;
	dx = 1;
	dy = 1;

	textBox.top = 0;
	textBox.left = 0;
	textBox.bottom = 50;
	textBox.right = 280;
	
	for (int i = 0; i < 125; ++i)
	{
		sheets.push_back(new PaperSheet());
	}

	sheets.back()->startRotating();
	

	return 0;
}

// sets the matrices of the view points and the projection
int myGame::setMatrices(void)
{

	// get the input


	// process input

	// if roll then change the up vector rotation

	// if pitch then change the lookAtVector rise angle

	// if yaw then change the looAtVector


	return 0;
}
