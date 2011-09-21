// io_demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "io_demo.h"
#include "myGame.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   PSTR		 pCmdLine,
                   int		 nCmdShow)
{

	// create the game
	myGame game(hInstance, "Scrolling Font ");

	// initialize the game
	// note that this is done via a virtual function.  Thus there is no need to update this function
	game.initGame();


	game.gameLoop();



}



