#include "stdafx.h"

#include "PaperSheet.h"

IDirect3DVertexDeclaration9* PaperSheet::paperDecl = NULL;
LPDIRECT3DTEXTURE9 PaperSheet::gTexture = NULL;
LPD3DXSPRITE PaperSheet::d3dSprite = NULL;

const float PaperSheet::sharedPitch = 90.0f;
const float PaperSheet::paperRatio = 11 / 8.5f;	// paper ratio
const float PaperSheet::sharedScaleY = 2000.0f;
const float PaperSheet::sharedScaleX = sharedScaleY * paperRatio;
const float PaperSheet::sharedScaleZ = 0.5f;
const D3DXVECTOR3 PaperSheet::_sharedPosition
	= D3DXVECTOR3(0, -sharedScaleY, sharedScaleX * 0.75f);	// move the sheet so the origin is the middle
															// of where the sheet falls

PaperSheet::PaperSheet() 
{
	mPitch = sharedPitch;
	mScaleY = sharedScaleY;
	mScaleX = sharedScaleX;
	mScaleZ = sharedScaleZ;

	mPosition = _sharedPosition;

	rotating = false;
	deleteMe = false;

	D3DCOLOR color1 = D3DCOLOR_XRGB(255, 255, 255);
	D3DCOLOR color2	= D3DCOLOR_XRGB(200, 200, 200);
	vertices[0] = paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color1);
	vertices[1] = paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1);
	vertices[2] = paperVertex(D3DXVECTOR3( 1.0f,  1.0f, -1.0f), color1);
	vertices[3] = paperVertex(D3DXVECTOR3(1.0f, -1.0f, -1.0f), color1);
	vertices[4] = paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1);
	vertices[5] = paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color2);
	vertices[6] = paperVertex(D3DXVECTOR3(1.0f,  1.0f,  1.0f), color2);
	vertices[7] = paperVertex(D3DXVECTOR3(1.0f, -1.0f,  1.0f), color1);

	indices[0] = 0; indices[1] = 1; indices[2] = 2; // Front face
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	indices[6] = 4; indices[7]  = 6; indices[8]  = 5; // Backface
	indices[9] = 4; indices[10] = 7; indices[11] = 6;
	indices[12] = 4; indices[13] = 5; indices[14] = 1; // Left face
	indices[15] = 4; indices[16] = 1; indices[17] = 0;
	indices[18] = 3; indices[19] = 2; indices[20] = 6; // Right face
	indices[21] = 3; indices[22] = 6; indices[23] = 7;
	indices[24] = 1; indices[25] = 5; indices[26] = 6; // Top face
	indices[27] = 1; indices[28] = 6; indices[29] = 2;
	indices[30] = 4; indices[31] = 0; indices[32] = 3; // Bottom face
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	initGeom();
}

PaperSheet::~PaperSheet() {
	mVtxBuf->Release();
	paperDecl->Release();
	mIndBuf->Release();
	if (gTexture != NULL) { gTexture->Release(); }
	if (d3dSprite != NULL) { d3dSprite->Release(); }
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
		D3DDECL_END()
	};
	decl[0].Offset = (char *) &v.pos - (char *) &v;
	decl[1].Offset = (char *) &v.color - (char *) &v;
	
	md3dDev->CreateVertexDeclaration(decl, &paperDecl);

	// lock the buffer and copy the vertices (locking the entire array)		
	mVtxBuf->Lock(0,0,(void**)&vtx, 0);
	memcpy(vtx, vertices,VTX_NUM*sizeof(paperVertex));
	mVtxBuf->Unlock();

	///// Index Buffer

	md3dDev->CreateIndexBuffer(NUM_TRIANGLES*3*sizeof(long),
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX32,D3DPOOL_MANAGED,
							   &mIndBuf,
							   NULL);

	// lock the buffer and copy the indices (locking the entire array)
	mIndBuf->Lock(0, 0, (void**)&ind, 0);
	memcpy(ind, indices, (NUM_TRIANGLES*3)*sizeof(long));
	mIndBuf->Unlock();

	// Set up Texture
	if (d3dSprite == NULL && gTexture == NULL) {
		D3DXCreateSprite(md3dDev, &d3dSprite);
		D3DXCreateTextureFromFileEx(md3dDev, "sheet.png", D3DX_DEFAULT_NONPOW2 , D3DX_DEFAULT_NONPOW2 , D3DX_DEFAULT, 0, 
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &gTexture);
	}

	return 0;
}

int PaperSheet::render(int time)
{
	D3DXMATRIX worldMat, viewMat, matTransform, matProjection, matScale, matTranslate,  matRotation;

	// Scale
	D3DXMatrixScaling(&matScale,mScaleX, mScaleY, mScaleZ);
	worldMat = matScale;

	// Translate in order to rotate at bottom
	D3DXMatrixTranslation(&matTranslate, 0, -mScaleY, 0);
	worldMat *= matTranslate;

	// Rotation
	float angleRad = D3DXToRadian(mPitch);
	D3DXVECTOR3 rotVector(-1.0,0.0,0.0);
	D3DXMatrixRotationAxis(&matRotation, &rotVector, angleRad);
	worldMat *= matRotation;

	// Translate back
	D3DXMatrixTranslation(&matTranslate, 0, mScaleY, 0);
	worldMat *= matTranslate;
	
	// Real Translation
	D3DXMatrixTranslation(&matTranslate, mPosition.x, mPosition.y, mPosition.z);
	worldMat *= matTranslate;

	md3dDev->SetTransform(D3DTS_WORLD, &worldMat);
	
	md3dDev->SetStreamSource(0, mVtxBuf, 0, sizeof(paperVertex));
	md3dDev->SetVertexDeclaration(paperDecl);
	md3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	md3dDev->SetIndices(mIndBuf);
	md3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VTX_NUM, 0, NUM_TRIANGLES);

	//d3dSprite->Begin(NULL);
	//d3dSprite->Draw(gTexture, NULL, NULL, NULL, D3DCOLOR_ARGB(0,255,255,255));
	//d3dSprite->End();

	return 0;
}

int PaperSheet::updateState() {
	if (rotating) {
		mPitch += 0.5f;
		if (mPitch == 270.0f) {
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