
//=============================================================================
// gameIO.cpp 
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
// Usage of code for other purposes is not allowed with a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================



#include "StdAfx.h"
#include "gameIO.h"



// defines



// static variables

IDirectInput8* gameIO::diCOM = NULL;





/******************************************************************/
/*
Purpose: initialized the IO (keyboard and a mouse)

Descripton: 

Return:

*/


gameIO::gameIO(HINSTANCE hinstance, HWND hwnd): mKeyboardDev(NULL), mMouseDev(NULL)
{
	int rc = 0;
	if (this->diCOM == NULL) {
		// initialize the com interface
		rc = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&diCOM, NULL);
		
		// error
		if (rc != DI_OK) diCOM = NULL;
	}

	// initialize the keyboard
	rc = initKeyboard(hwnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	if (rc != 0) mKeyboardDev = NULL;


	// initialize the mouse
	rc = initMouse(hwnd, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	if (rc != 0) mMouseDev = NULL;


}


/******************************************************************/
/*
Purpose: release the resources

Descripton:

Return:

*/


gameIO::~gameIO(void)
{
	if (mKeyboardDev != NULL) {
		mKeyboardDev->Unacquire();
		mKeyboardDev->Release();
		mKeyboardDev = NULL;
	}

	if (mMouseDev != NULL) {
		mMouseDev->Unacquire();
		mMouseDev->Release();
		mMouseDev = NULL;
	}

	if (diCOM != NULL) {
		diCOM->Release();
		diCOM = NULL;
	}
}

/******************************************************************/
/*
Purpose: initializes the keyboard

Descripton:

Return: 
0 if OK
1 if error

*/


int gameIO::initKeyboard(HWND hwnd, long CooperativeFlags)
{

	if (diCOM == NULL) return(1);	// error

	// get the device
	diCOM->CreateDevice(GUID_SysKeyboard, &mKeyboardDev, NULL);
	if (mKeyboardDev == NULL) {
		return(1);
	}

	// set the coopertive flags
	mKeyboardDev->SetCooperativeLevel(hwnd, CooperativeFlags);

	// set the keyboard status strucure
	ZeroMemory(mKeyboardState, sizeof(char) * 256);
	mKeyboardDev->SetDataFormat(&c_dfDIKeyboard);

	// acquire the keyboard
	mKeyboardDev->Acquire();

	return(0);
}


/******************************************************************/
/*
Purpose: initialize the mouse device

Descripton:

Return: 
0 if OK
1 if error

*/


int gameIO::initMouse(HWND hwnd, long CooperativeFlags)
{


	if (diCOM == NULL) return(1);	// error

	// get the device
	diCOM->CreateDevice(GUID_SysMouse, &mMouseDev, NULL);
	if (mMouseDev == NULL) {
		return(1);
	}

	// set the coopertive flags
	mMouseDev->SetCooperativeLevel(hwnd, CooperativeFlags);

	// set the keyboard status strucure
	ZeroMemory(&mMouseState, sizeof(DIMOUSESTATE));
	mMouseDev->SetDataFormat(&c_dfDIMouse); // set the data format

	// acquire the keyboard
	mMouseDev->Acquire();

	return(0);
}



/******************************************************************/
/*
Purpose: get the state of the keyboard and mouse

Descripton:

Return:
0 - OK
1 - could not get data try again
-1 - error with device


Note:
should be devided into three functions.
*/


int gameIO::poll(void)
{
	int rc = 0;
	int count; 

	// get the keyboard state
	rc = mKeyboardDev->GetDeviceState(256, mKeyboardState);
	count = 0;
	while (rc != DI_OK) {
		switch (rc) {
			case DI_OK: 
				break;
			case DIERR_INPUTLOST:
			case DIERR_NOTACQUIRED:
				mKeyboardDev->Acquire();
				break;
			case E_PENDING:
				break;
			case DIERR_INVALIDPARAM:
			case DIERR_NOTINITIALIZED:
			default:
				// error has occured
				return(-1);
		}
		rc = mKeyboardDev->GetDeviceState(256,mKeyboardState);
		count++;
		if (count > 100) return(1);
	}
	


	// get the mouse state
	count = 0;
	rc = mMouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);
	count = 0;
	while (rc != DI_OK) {
		switch (rc) {
			case DI_OK: 
				break;
			case DIERR_INPUTLOST:
			case DIERR_NOTACQUIRED:
				mMouseDev->Acquire();
				break;
			case E_PENDING:
				break;
			case DIERR_INVALIDPARAM:
			case DIERR_NOTINITIALIZED:
			default:
				// error has occured
				return(-1);
		}
		rc = mMouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);
		count++;
		if (count > 100) return(1);
	}
	

	return 0;
}


/******************************************************************/
/*
Purpose: returns the change along the x coordinate

Descripton:

Return:

*/


int gameIO::mouseDX(void)
{
	return(mMouseState.lX);
}

/******************************************************************/
/*
Purpose:returns the change along the y coordinate

Descripton:

Return:

*/


int gameIO::mouseDY(void)
{
	return(mMouseState.lY);
}

/******************************************************************/
/*
Purpose:returns the change along the z coordinate

Descripton:

Return:

*/


int gameIO::mouseDZ(void)
{
	return(mMouseState.lZ);
}


/******************************************************************/
/*
Purpose: returns a vector of the mouse move

Descripton:

Return:

*/



D3DXVECTOR3 gameIO::mouseDelta(void)
{
	return D3DXVECTOR3((float)mMouseState.lX,(float)mMouseState.lY,(float)mMouseState.lZ);
}

/******************************************************************/
/*
Purpose: checks if a specific mouse button was pressed

Descripton: check if a given mouse button was pressed

Return:
1 - if pressed
0 - if not pressed

*/


int gameIO::mouseButtonPressed(int button)
{
	return(mMouseState.rgbButtons[button] & 0x80);
}

/******************************************************************/
/*
Purpose: checks if a specific keyboard key was pressed

Descripton: 

Return:
1 - if pressed
0 - if not pressed

*/



int gameIO::keyboardPressed(unsigned char key)
{
	return(mKeyboardState[key] & 0x80);
}

