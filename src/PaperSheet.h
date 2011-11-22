#ifndef PAPER_SHEET_H
#define PAPER_SHEET_H

#include "GameObject.h"

#define NUM_TRIANGLES 12			// 6 sides of a cube * 2 triangles per cube
#define VTX_NUM NUM_TRIANGLES * 2
#define NUM_TEXTURES 5

struct paperVertex {
	D3DXVECTOR3 pos;
	DWORD color;        // The vertex color
	D3DXVECTOR2 texCoor;
	D3DXVECTOR2 texCoor2;
	paperVertex(D3DXVECTOR3 p, DWORD c, D3DXVECTOR2 t, D3DXVECTOR2 t2) {
		pos = p; color = c; texCoor = t; texCoor2 = t2;
	}
	paperVertex() {
		pos = D3DXVECTOR3(0,0,0); color = 0; texCoor = texCoor2 = D3DXVECTOR2(0,0);
	}
};

class PaperSheet : GameObject
{
public:
	PaperSheet();
	~PaperSheet();

	virtual int initGeom();
	virtual int render(int time);
	virtual int updateState();

	void startRotating();
	bool shouldDelete();

	static void setupVertices();
	static void releaseVertices();
	static void loadHole();
	static void releaseHole();

	void setPitch(float newValue);
	void setFloor();
	
	static const float sharedPitch;
	static const float paperRatio;	// paper ratio
	static const float sharedScaleY;
	static const float sharedScaleX;
	static const float sharedScaleZ;

private:
	void randomizeHole();
	void createAlphaMask();

	static paperVertex* vertices;
	//long indices[NUM_TRIANGLES * 3];		// 3 vertices per triangle

	static IDirect3DVertexDeclaration9* paperDecl;
	static const D3DXVECTOR3 _sharedPosition;

	LPDIRECT3DTEXTURE9 frontTexture;					// current front texture
	LPDIRECT3DTEXTURE9 backTexture;						// current back texture
	static LPDIRECT3DTEXTURE9 holeTexture;
	static D3DXVECTOR3 texCenter;
	static D3DXVECTOR3 texPos;

	bool rotating;
	bool deleteMe;
	bool floor;
};

#endif