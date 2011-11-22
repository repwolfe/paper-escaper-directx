#include "stdafx.h"

#include "PaperSheet.h"

IDirect3DVertexDeclaration9* PaperSheet::paperDecl = NULL;
LPDIRECT3DTEXTURE9 PaperSheet::holeTexture = NULL;
paperVertex* PaperSheet::vertices = new paperVertex[VTX_NUM];

const float PaperSheet::sharedPitch = -90.0f;
const float PaperSheet::paperRatio = 11 / 8.5f;	// paper ratio
const float PaperSheet::sharedScaleY = 2000.0f;
const float PaperSheet::sharedScaleX = sharedScaleY * paperRatio;
const float PaperSheet::sharedScaleZ = 0.5f;
const D3DXVECTOR3 PaperSheet::_sharedPosition
	= D3DXVECTOR3(0, sharedScaleY, sharedScaleX * 0.75f);	// move the sheet so the origin is the middle
															// of where the sheet falls

PaperSheet::PaperSheet(bool shouldHaveHole) 
{
	mPitch = sharedPitch;
	mScaleY = sharedScaleY;
	mScaleX = sharedScaleX;
	mScaleZ = sharedScaleZ;

	mPosition = _sharedPosition;

	rotating = false;
	deleteMe = false;
	floor	 = false;
	this->shouldHaveHole = shouldHaveHole;

	// Todo: load random picture
	D3DXCreateTextureFromFile(md3dDev, "sheet.png", &frontTexture);
	D3DXCreateTextureFromFile(md3dDev, "sheet2.png", &backTexture);

	initGeom();
}

PaperSheet::~PaperSheet() {
	mVtxBuf->Release();
	paperDecl->Release();
	frontTexture->Release();
	backTexture->Release();
}

void PaperSheet::setupVertices()
{
	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

	// Front Face (1-2-3-4)
	// Even though the vertices are 1 2 3 4, it renders the texture in this order: 1, 3, 2, 4
	vertices[0]		= paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color, D3DXVECTOR2(0,0));
	vertices[1]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f, -1.0f), color, D3DXVECTOR2(1,0));
	vertices[2]		= paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color, D3DXVECTOR2(0,1));
	vertices[3]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color, D3DXVECTOR2(1,1));

	// Back Face (6-5-8-7)
	// Even though the vertices are 5 6 7 8, it renders the texture in this order: 5, 7, 6, 8
	vertices[4]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color, D3DXVECTOR2(0,0));
	vertices[5]		= paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color, D3DXVECTOR2(1,0));
	vertices[6]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f,  1.0f), color, D3DXVECTOR2(0,1));
	vertices[7]		= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color, D3DXVECTOR2(1,1));
}

void PaperSheet::releaseVertices()
{
	delete[] vertices;
}

/**
 * All sheets use the same hole texture as an alpha mask
 */
void PaperSheet::loadHole()
{
	D3DXCreateTextureFromFile(md3dDev, "hole.png", &holeTexture);
	md3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	md3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void PaperSheet::releaseHole()
{
	holeTexture->Release();
}

int PaperSheet::initGeom() {
	struct cubeVertex *vtx = NULL;
	long *ind = NULL;

	md3dDev->CreateVertexBuffer(VTX_NUM*sizeof(struct paperVertex),D3DUSAGE_WRITEONLY,
										NULL,D3DPOOL_MANAGED,&mVtxBuf, NULL);

	///// Vertex Declaration
	paperVertex v;
	D3DVERTEXELEMENT9 decl[] = {
		{0,0,D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0,0,D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0,0,D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	decl[0].Offset = (char *) &v.pos - (char *) &v;
	decl[1].Offset = (char *) &v.color - (char *) &v;
	decl[2].Offset = (char *) &v.texCoor - (char *) &v;
	
	md3dDev->CreateVertexDeclaration(decl, &paperDecl);

	// Make sheet transparent
	if (shouldHaveHole) {
		createAlphaMask();
	}

	// lock the buffer and copy the vertices (locking the entire array)		
	mVtxBuf->Lock(0,0,(void**)&vtx, 0);
	memcpy(vtx, vertices,VTX_NUM*sizeof(paperVertex));
	mVtxBuf->Unlock();

	return 0;
}

/**
 * Loops through the hole texture and sets the alpha values
 * of a random spot of the sheet to be the color values of the hole
 *
 * Meaning that where the hole is black, the sheet will be transparent
 * and white the sheet is visible
 *
 * Does it for both the front texture and back texture so it is fully see through
 */
void PaperSheet::createAlphaMask()
{
	D3DLOCKED_RECT frontRec, backRec, holeRec;
	D3DSURFACE_DESC frontDesc, backDesc, holeDesc;
	IDirect3DSurface9 *frontSurf, *backSurf, *holeSurf;

	frontTexture->GetSurfaceLevel(0, &frontSurf);
	backTexture->GetSurfaceLevel(0, &backSurf);
	holeTexture->GetSurfaceLevel(0, &holeSurf);

	frontSurf->GetDesc(&frontDesc);
	backSurf->GetDesc(&backDesc);
	holeSurf->GetDesc(&holeDesc);

	UINT frontWidth = frontDesc.Width;
	UINT frontHeight = frontDesc.Height;

	UINT backWidth = backDesc.Width;
	UINT backHeight = backDesc.Height;

	UINT holeWidth = holeDesc.Width;
	UINT holeHeight = holeDesc.Height;

	// Needs to be the same in order to make sense, if wrong do nothing
	if (frontWidth != backWidth || frontHeight != backHeight)
	{
		return;
	}

	// TODO: Make LockRect only lock relevant pixels
	frontSurf->LockRect(&frontRec, NULL, 0);
	backSurf->LockRect(&backRec, NULL, 0);
	holeSurf->LockRect(&holeRec, NULL, 0);

	BYTE* frontBuffer = (BYTE*)(frontRec.pBits);
	BYTE* backBuffer = (BYTE*)(backRec.pBits);
	BYTE* holeBuffer = (BYTE*)(holeRec.pBits);

	const int margin = 25;	// distance from edge
	UINT startingY = rand() % (frontHeight - holeHeight - margin) + margin;
	UINT startingX = rand() % (frontWidth - holeWidth - margin) + margin;

	holeCenter = D3DXVECTOR2(startingX + holeWidth / 2.0f, startingY + holeHeight / 2.0f);
	this->holeHeight = holeHeight;
	this->holeWidth = holeWidth;
	
	for (UINT y = startingY; y < startingY + holeHeight; ++y) {
		for (UINT x = startingX; x < startingX + holeWidth; ++x) {
			DWORD index = (x * 4) + (y * frontRec.Pitch);
			DWORD hIndex = ((x - startingX) * 4) + ((y - startingY) * holeRec.Pitch);
			// since the hole is grayscale, RGB values should all be the same, so 255 = black, 0 = white
			frontBuffer[index+3] = (BYTE)holeBuffer[hIndex];		// Alpha
		}
	}
	
	for (UINT y = startingY; y < startingY + holeHeight; ++y) {
		for (UINT x = backWidth - startingX - holeWidth; x < backWidth - startingX; ++x) {
			DWORD index = (x * 4) + (y * frontRec.Pitch);
			DWORD hIndex = ((x + startingX + holeWidth - backWidth) * 4) + ((y - startingY) * holeRec.Pitch);
			// since the hole is grayscale, RGB values should all be the same, so 255 = black, 0 = white
			backBuffer[index+3] = (BYTE)holeBuffer[hIndex];		// Alpha
		}
	}
	
	// TODO: Check if desc and surfaces need to be released
	holeSurf->UnlockRect();
	backSurf->UnlockRect();
	frontSurf->UnlockRect();
}

int PaperSheet::render(int time)
{
	D3DXMATRIX worldMat, viewMat, matTransform, matProjection, matScale, matTranslate,  matRotation;

	// Scale
	D3DXMatrixScaling(&matScale,mScaleX, mScaleY, mScaleZ);
	worldMat = matScale;

	// Translate in order to rotate at bottom
	D3DXMatrixTranslation(&matTranslate, 0, mScaleY, 0);
	worldMat *= matTranslate;

	// Rotation
	float angleRad = D3DXToRadian(mPitch);
	D3DXVECTOR3 rotVector(-1.0,0.0,0.0);
	D3DXMatrixRotationAxis(&matRotation, &rotVector, angleRad);
	worldMat *= matRotation;

	// Translate back
	D3DXMatrixTranslation(&matTranslate, 0, -mScaleY, 0);
	worldMat *= matTranslate;
	
	// Real Translation
	D3DXMatrixTranslation(&matTranslate, mPosition.x, mPosition.y, mPosition.z);
	worldMat *= matTranslate;

	md3dDev->SetTransform(D3DTS_WORLD, &worldMat);
	
	md3dDev->SetStreamSource(0, mVtxBuf, 0, sizeof(paperVertex));
	md3dDev->SetVertexDeclaration(paperDecl);

	//alpha blending enabled
	md3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//source blend factor
	md3dDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//destination blend factor
	md3dDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//alpha from texture
	md3dDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

	md3dDev->SetTexture(0, frontTexture);
	md3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	md3dDev->SetTexture(0, backTexture);
	md3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	return 0;
}

int PaperSheet::updateState() {
	if (rotating) {
		mPitch += 0.5f;
		if (mPitch == 90.0f) {
			rotating = false;
			deleteMe = true;
		}
	}

	return 0;
}

void PaperSheet::startRotating() {
	rotating = true;
}

bool PaperSheet::shouldDelete() {
	return deleteMe;
}

void PaperSheet::setPitch(float newValue) {
	mPitch = newValue;
}

void PaperSheet::setFloor() {
	floor = true;
}

/**
 * Takes a 2D position (ignoring 3D height)
 * and returns true if the position is inside the corresponding
 * pixels of the hole in the sheet
 * false otherwise
 */
bool PaperSheet::isInHole(float x, float y) {
	// Determine if the position vector is in the area of the hole

	// TODO: Somehow convert world position into pixel position
	return false;
}

D3DXVECTOR2 PaperSheet::getHoleCenter() {
	return holeCenter;
}