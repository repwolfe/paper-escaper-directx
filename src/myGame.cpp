#include "StdAfx.h"
#include "myGame.h"
#include <stdio.h>



myGame::myGame(HINSTANCE hInstance, char* gameName):gameApp(hInstance, gameName)
, x(0), fontCourier(NULL), d3dSprite(NULL)
{
}

myGame::~myGame(void)
{
	if (fontCourier != NULL) {
		fontCourier->Release();
		fontCourier = NULL;
	}

	if (d3dSprite != NULL) {
		d3dSprite->Release();
		d3dSprite = NULL;
	}
}



int myGame::updateGameState(long time)
{
	int rc = 0;

	// add code to update the game state

	// check if text is drawn within the boundary
	if (x >= this->getWndWidth()) dx = -1;
	if (x <= 0) dx = 1;

	if (y >= this->getWndHeight()) dy = -1;
	if (y <= 0) dy = 1;

	// change the location of the string
	x += dx;
	y += dy;

	textBox.top = y-25;
	textBox.left = x-160;
	textBox.bottom = y+25;
	textBox.right = x+160;


	sprintf(s,"Scrolling Font: game time/frame # %ld \n location = (%d,%d)",time, x, y);
	

	// poll the input
	mInput->poll();
	
	// check if escape key was pressed
	if (mInput->keyboardPressed(DIK_ESCAPE)) {
		rc = 1;
	}

	if (mInput->mouseButtonPressed(0)) dx *= -1;
	if (mInput->mouseButtonPressed(1)) dy *= -1;

	return(rc);
}

int myGame::renderFrame(long time)
{

	// add code to render a frame

	md3dDev->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0, 0), 1.0f, 0);
	md3dDev->BeginScene();


	fontCourier->DrawText(NULL, s, -1, &textBox, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 0, 0)); 

	md3dDev->EndScene();
	md3dDev->Present(0, 0, 0, 0);

	return(0);

}

int myGame::initGame(void)
{

	int rc = 0;
	// add code to initialize the game

	// create a font

	rc = D3DXCreateFont(md3dDev,
						10, // height of font
						0,	// use the default width
						FW_MEDIUM+50,
						//FW_NORMAL,	// normal font weight
						1, // no Mipmap
						FALSE, // italic
						DEFAULT_CHARSET, // default character set
						OUT_DEFAULT_PRECIS, // default precision
						DEFAULT_QUALITY, // default quality
						DEFAULT_PITCH ||FF_DONTCARE, // more defaults...
						"Courier",	// typeface “Courier"
						&fontCourier); 


	if (rc != S_OK) {
		rc = 1;
	}

	rc = D3DXCreateSprite(md3dDev, &d3dSprite);
//	D3DXCreateTextureFromFile(md3dDev, L"textures.png", &mySprite);

	x = this->getWndWidth() / 2;
	y = this->getWndHeight() / 2;
	dx = 1;
	dy = 1;

	textBox.top = 0;
	textBox.left = 0;
	textBox.bottom = 50;
	textBox.right = 280;


	// get the dimension of the window
	return 0;
}
