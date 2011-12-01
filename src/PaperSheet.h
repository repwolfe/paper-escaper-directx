#ifndef PAPER_SHEET_H
#define PAPER_SHEET_H

#include "GameObject.h"

#define NUM_TRIANGLES 4			// 2 sides of a cube * 2 triangles per cube
#define VTX_NUM NUM_TRIANGLES * 2
#define NUM_HOLES 3

struct paperVertex {
	D3DXVECTOR3 pos;
	DWORD color;        // The vertex color
	D3DXVECTOR2 texCoor;
	paperVertex(D3DXVECTOR3 p, DWORD c, D3DXVECTOR2 t) {
		pos = p; color = c; texCoor = t;
	}
	paperVertex() {
		pos = D3DXVECTOR3(0,0,0); color = 0; texCoor = D3DXVECTOR2(0,0);
	}
};

class PaperSheet : public GameObject
{
public:
	PaperSheet(bool shouldHaveHole);
	~PaperSheet();

	virtual int initGeom();
	virtual int render(int time);
	virtual int updateState();

	void startRotating();
	bool shouldDelete();

	static void setupVertices();
	static void releaseVertices();
	static void loadHoles();
	static void releaseHoles();

	void setPitch(float newValue);
	void setFloor();

	bool isInHole(float x, float y);
	D3DXVECTOR2 getHoleCenter();
	D3DXVECTOR2 getHoleCenterPixel();
	
	static const float sharedPitch;
	static const float paperRatio;	// paper ratio
	static const float sharedScaleY;
	static const float sharedScaleX;
	static const float sharedScaleZ;

private:
	void createAlphaMask();

	static paperVertex* vertices;

	static IDirect3DVertexDeclaration9* paperDecl;
	static const D3DXVECTOR3 _sharedPosition;

	LPDIRECT3DTEXTURE9 frontTexture;					// current front texture
	LPDIRECT3DTEXTURE9 backTexture;						// current back texture
	static LPDIRECT3DTEXTURE9* holeTextures;			// all the possible hole textures
	LPDIRECT3DTEXTURE9 holeTexture;						// the sheets chosen hole texture
	static D3DXVECTOR3 texCenter;
	static D3DXVECTOR3 texPos;

	D3DXVECTOR2 holeCenter;			// The location of the center of the hole on the quad (assuming bottom left is 0,0)
	D3DXVECTOR2 holeCenterPixel;
	D3DXVECTOR2 holeWorldLocation;	// The location of the center of the hole in regards to the world coordinates
	UINT holeHeight;			// the height of the hole on the sheet texture
	UINT holeWidth;				// the width of the hole on the sheet texture

	bool rotating;
	bool deleteMe;
	bool floor;
	bool shouldHaveHole;
};

#endif