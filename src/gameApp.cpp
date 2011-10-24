
//=============================================================================
// gameApp.cpp 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates how to use directx input 
// b. demonstrates how to use directx fonts
// c. demonstrates how to initialize direct3D
// d. provide a framework for creating games
// e. demonstrates how to create an egnine frame and provide a skeleton function of one.
// f. demonstarte the usage of a camera
// g. demonstrates the creation of a surface and of vertex and index buffers
// h. demonstartes how to set the differnt matrices. 
//
// Description:
//--------------
// A simple application that demonstrates how to use directx to surface and how to use the camea.  
// It uses a framework for a general purpose simple game engine
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
#include "gameApp.h"



// DEFINE

#define WND_WIDTH 800
#define WND_HEIGHT 600
#define FRAMES_PER_SECOND 30


/******************************************************************/
/*
Purpose: checks if a specific keyboard key was pressed

Descripton: 

Return:
1 - if pressed
0 - if not pressed

*/


/******************************************************************/
/*
Purpose: the window procedure to be used when creating a window


Descripton: 
This is a single global function.  It should be modified so that message will be processed by the app
by the application.  

Return:
1 - if failed
0 - if successful

*/


LRESULT CALLBACK globalWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int rc = 1;
	switch (msg){
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			// Handle any messages the switch statement didn't
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return rc;
}





/******************************************************************/
/*
Purpose: destructor of the game application


Descripton: 

Return:
1 - if failed
0 - if successful

*/

gameApp::~gameApp(void)
{
	if (md3dDev != NULL) {
		md3dDev->Release();
		md3dDev = NULL;
	}
	if (md3d != NULL) {
		md3d->Release();
		md3d = NULL;
	}
	if (mInput != NULL) {
		delete(mInput);
		mInput = NULL;
	}
}


/******************************************************************/
/*
Purpose: constructor of the game application


Descripton: 

Return:


*/


gameApp::gameApp(HINSTANCE hInstance, char * gameName) : 
  mFullScreen(0), 
  mWndWidth(0), 
  mWndHeight(0)
  ,nearPlane(0), farPlane(0),md3d(NULL)
{
	this->mhinstance = hInstance;
	mFullScreen = false;
	mWndWidth = WND_WIDTH;
	mWndHeight = WND_HEIGHT;
	mFramesPerSecond = FRAMES_PER_SECOND;
	mhwnd = NULL;
	mGameName[GAME_TITLE_SISE] = '\0';
	if (mGameName == NULL) this->mGameName[0] = '\0';
	else strncpy(this->mGameName, gameName,GAME_TITLE_SISE);
	init();

  }

/******************************************************************/
/*
Purpose: initialize the game application devices - window, graphics and input


Descripton: 

Return:
1 - if failed
0 - if successful

*/
int gameApp::init(void)
{
	int rc = 0;
	// initialize the window
	rc = initWindow();
	if (rc != 0) {
		MessageBox(0, "Failed to initalize the game window", 0, 0);
		PostQuitMessage(0);
	}


	// initialize the direct3D
	rc = initD3D();
	if (rc != 0) {
		MessageBox(0, "Failed to initalize the graphics device", 0, 0);
		PostQuitMessage(0);
	}

	// initalize the input and output
	rc = initGameInput(); 
	if (rc != 0) {
		MessageBox(0, "Failed to initalize the input devices", 0, 0);
		PostQuitMessage(0);
	}
	return 0;
}


/******************************************************************/
/*
Purpose: i// initialize the game window (assuming only one window)


Descripton: 

Return:
1 - if failed
0 - if successful

*/

int gameApp::initWindow(void)
{	
	int rc = 0;
	char *winClass = "gameAppClass";
	WNDCLASS wc;


	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= globalWndProc;
	wc.hInstance		= mhinstance;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName	= winClass;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= winClass;


	rc = RegisterClass(&wc);
	if (rc == 0) {
		// could not register the window
		return(1);
	}

	rc = 0;  
	mhwnd = CreateWindow(winClass, mGameName, WS_OVERLAPPEDWINDOW, 10, 10, this->mWndWidth, this->mWndHeight, NULL, NULL, mhinstance, NULL);

   if (mhwnd == NULL) {
	   rc = GetLastError();
	   rc = 1;
   }else {
	   ShowWindow(mhwnd, SW_SHOW);
	   UpdateWindow(mhwnd);
	}

	return(rc);
}

/******************************************************************/
/*
Purpose: initializes the graphics device

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::initD3D(void)
{
	int rc = 0;;

	// get the com object if it does not exist 
	if (md3d == NULL) {// check if the com object was already initialized
		// initialize the com object
		md3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (md3d == NULL) {
			rc = 1;
			return(rc);
		}
	}

	// initial the direct3D9 parameters
	ZeroMemory(&md3dpp, sizeof(md3dpp));
	md3dpp.BackBufferWidth = mWndWidth;			// the width of the back buffer
	md3dpp.BackBufferHeight = mWndHeight;		// the height of the back buffer
	md3dpp.Windowed = !mFullScreen;				// using windowed version
	md3dpp.hDeviceWindow = mhwnd;				// pass the handle to the window
	md3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
	//md3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//md3dpp.EnableAutoDepthStencil = TRUE;		// allow depth buffer
	md3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;  // stencil buffer format




	// create a directx device which will enable drawing on the screen.
	rc = md3d->CreateDevice(D3DADAPTER_DEFAULT,		// this is the main adapter
							 D3DDEVTYPE_HAL,			// use hardware abstraction layer
							 mhwnd,					// the window to be rendered
							 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							 //D3DCREATE_HARDWARE_VERTEXPROCESSING,
							 &md3dpp,				// address of the d3d parameters structures 
							 &md3dDev);				// address of the allocated device

	// Turn off culling, so we see the front and back of the triangle
	md3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	// Turn off D3D lighting, since we are providing our own vertex colors
	md3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );


	if (rc != S_OK) {
		// release d3d (the com object
		md3d->Release();
		md3d = NULL;
		rc = 1;
	}
	GameObject::setd3dDev(md3dDev);
	return(0);
}
/******************************************************************/
/*
Purpose: initializes the game input devices

Descripton: 

Return:
1 - if failed
0 - if successful

*/


int gameApp::initGameInput(void)
{
	
	mInput = new gameIO(mhinstance, mhwnd);
	if (mInput == NULL) return(1);
	else return 0;
}


/******************************************************************/
/*
Purpose: executes the message loop

Descripton: 
This function is a virtual function and can be replaced by a similar function in the derived class.



Return:
1 - if failed
0 - if successful

*/


int gameApp::gameLoop(void)
{
	int rc = 0;
	// this struct holds Windows event messages
	MSG msg;    // this struct holds Windows event messages

	// variables for measuring the time
	static DWORD LastRenderTime = GetTickCount();
	DWORD ElapsedTime = 0;
	DWORD TimePerFrame = 1000 / mFramesPerSecond;
	DWORD CurrentTime = 0;

	static long time = 0;		// "time" of the game - number of frames since the begining of the game

	// Enter the infinite message loop
	while(TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If the message is WM_QUIT, exit the while loop
			if (msg.message == WM_QUIT)
				break;

			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}

		// update the game state
		// note that this is done via a virtual function
		rc = updateGameState(time);
		if (rc != 0) {
			// quit
			PostQuitMessage(0);
		}

		// render the next frame  and present the game to user
		// note that this is done via a virtual function
		renderFrame(time);
		time++;
		

		// determine whether to sleep 
		CurrentTime = GetTickCount();
		ElapsedTime = CurrentTime - LastRenderTime;
		if (ElapsedTime < TimePerFrame) {
			// wait for next round
			Sleep(TimePerFrame - ElapsedTime);
		}
		LastRenderTime = GetTickCount();

	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;	
	return 0;
}


/******************************************************************/
/*
Purpose: updates the game state

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::updateGameState(long time)
{
	return 0;
}


/******************************************************************/
/*
Purpose: renders one frame

Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameApp::renderFrame(int time)
{
	return 0;
}

/******************************************************************/
/*
Purpose: initialize the game

Descripton: 

Return:
1 - if failed
0 - if successful

*/

int gameApp::initGame(void)
{
	return 0;
}

/******************************************************************/
/*
Purpose: window width

Descripton: 

Return:
the window width

*/

int gameApp::getWndWidth(void)
{
	return (this->mWndWidth);
}

/******************************************************************/
/*
Purpose: window width

Descripton: 

Return:
the window height

*/
int gameApp::getWndHeight(void)
{
	return (this->mWndHeight);
}




D3DMATRIX * gameApp::getProjMat(D3DXMATRIX * matProj)
{
	return(D3DXMatrixPerspectiveFovLH(matProj,fieldOfView, aspectRatio, nearPlane, farPlane));
}




void gameApp::setProj(float nearPlane, float farPlane, float fieldOfView, float aspectRatio)

{
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
}
