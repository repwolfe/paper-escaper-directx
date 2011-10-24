
//=============================================================================
// gameIO.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates how to create a general purpose wrapper class for 
//capturing input state 

//
// Description:
//--------------
// A simple warpper class around input and output of directinput.  
// It shows how to initialize and capture the inputs of 
// a mouse and keyboard.

// The class captures the state in one function.  Thus, the user state of 
// pressing keys on the keyboard or the mouse actions are being captured 
// at the same time (as close as possib to the true state of the input.
// It uses a framework for a general purpose simple game engine
//
//Some of the ideas were taken from the boook by Luna
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
#include "stdafx.h"

#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>



class gameIO
{

	// FUNCTIONS
public:
	gameIO(HINSTANCE hinstance, HWND hwnd);
	~gameIO(void);
	int poll(void);
	int mouseDX(void);
	int mouseDY(void);
	int mouseDZ(void);
	D3DXVECTOR3 mouseDelta(void);
	int mouseButtonPressed(int button);
	int keyboardPressed(unsigned char key);

private:
	int initKeyboard(HWND hwnd, long CooperativeFlags);
	int initMouse(HWND hwnd, long CooperativeFlags);


	//MEMBERS
private:

	// keyboard members
	IDirectInputDevice8* mKeyboardDev;  // keyboardDev
	char mKeyboardState[256];			// keyboard state

	// mouse members
	IDirectInputDevice8* mMouseDev;  // mouse device
	DIMOUSESTATE mMouseState;		// mouse state

	// direct input interface
	static IDirectInput8* diCOM;
};
