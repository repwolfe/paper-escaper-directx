#include "stdafx.h"

#include "PaperSheet.h"

IDirect3DVertexDeclaration9* PaperSheet::paperDecl = NULL;
LPDIRECT3DTEXTURE9* PaperSheet::textures = new LPDIRECT3DTEXTURE9[NUM_TEXTURES];	// possible textures to use
LPDIRECT3DTEXTURE9 PaperSheet::holeTexture = NULL;
paperVertex* PaperSheet::vertices = new paperVertex[VTX_NUM];

const float PaperSheet::sharedPitch = /*0.0f*/90.0f;
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
	floor	 = false;

	gTexture = textures[0];	

	/*
	// Only 8 vertices that unfortunately have to be repeated several times in order to use texture coordinates
	// Besides each vertex is the actual vertex that it corresponds to
	// Each face of the sheet is represented as two triangles, where the vertices are listed clockwise
	// Front face
	vertices[0]		= paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(0,0));	// 1
	vertices[1]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(1,0));	// 2
	vertices[2]		= paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// 3
	
	vertices[3]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(1,0));	// 2
	vertices[4]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,1));	// 4
	vertices[5]		= paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// 3
	
	// Right face
	vertices[6]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(0,0));	// 2
	vertices[7]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 5
	vertices[8]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,1));	// 6
	
	vertices[9]		= paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 5
	vertices[10]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(1,1));	// 6
	vertices[11]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,0));	// 4
	
	// Back face
	vertices[12]	= paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(0,0));	// 5
	vertices[13]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 7
	vertices[14]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,1));	// 6
	
	vertices[15]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 7
	vertices[16]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(1,1));	// 8
	vertices[17]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,1));	// 6
	
	// Left face
	vertices[18]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(0,0));	// 7
	vertices[19]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(1,0));	// 1
	vertices[20]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,1));	// 8
	
	vertices[21]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(1,0));	// 1
	vertices[22]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,1));	// 3
	vertices[23]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,1));	// 8
	
	// Top face
	vertices[24]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(0,0));	// 7
	vertices[25]	= paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 5
	vertices[26]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// 1
	
	vertices[27]	= paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 5
	vertices[28]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(1,1));	// 2
	vertices[29]	= paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// 1
	
	// Bottom face	
	vertices[30]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,0));	// 6
	vertices[31]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 8
	vertices[32]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// 4
	
	vertices[33]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(1,0));	// 8
	vertices[34]	= paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,1));	// 3
	vertices[35]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// 4
	
	*/
	/*
	vertices[0] = paperVertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,0));	// bottom left corner of tex
	vertices[1] = paperVertex(D3DXVECTOR3(-1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(0,0));	// top left corner of tex
	vertices[2] = paperVertex(D3DXVECTOR3( 1.0f,  1.0f, -1.0f), color1, D3DXVECTOR2(0,1));	// top right corner of tex
	vertices[3] = paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,1));	// bottom right corner of tex
	vertices[4] = paperVertex(D3DXVECTOR3(-1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,0));
	vertices[5] = paperVertex(D3DXVECTOR3(-1.0f,  1.0f,  1.0f), color2, D3DXVECTOR2(0,0));
	vertices[6] = paperVertex(D3DXVECTOR3( 1.0f,  1.0f,  1.0f), color2, D3DXVECTOR2(0,0));
	vertices[7] = paperVertex(D3DXVECTOR3( 1.0f, -1.0f,  1.0f), color1, D3DXVECTOR2(0,0));

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
	*/
	initGeom();
}

PaperSheet::~PaperSheet() {
	mVtxBuf->Release();
	paperDecl->Release();
	//mIndBuf->Release();
}

void PaperSheet::setupVertices()
{
	D3DCOLOR color1 = D3DCOLOR_XRGB(255, 255, 255);
	D3DCOLOR color2	= D3DCOLOR_XRGB(200, 200, 200);

	// TODO: Remove extra texture coordinates
	// Front Face (1-2-3-4), flip the vertex coordinates to show properly for some reason
	vertices[0]		= paperVertex(D3DXVECTOR3( -1.0f, 1.0f, -1.0f), color1, D3DXVECTOR2(1,1), D3DXVECTOR2(1,1));
	vertices[1]		= paperVertex(D3DXVECTOR3( 1.0f, 1.0f, -1.0f), color1, D3DXVECTOR2(0,1), D3DXVECTOR2(0,1));
	vertices[2]		= paperVertex(D3DXVECTOR3( -1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(1,0), D3DXVECTOR2(1,0));
	vertices[3]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));

	// Right Face (2-6-4-8)
	vertices[4]		= paperVertex(D3DXVECTOR3( 1.0f, 1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[5]		= paperVertex(D3DXVECTOR3( 1.0f, 1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[6]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[7]		= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));

	// Top Face (5-6-1-2)
	vertices[8]		= paperVertex(D3DXVECTOR3( -1.0f, 1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[9]		= paperVertex(D3DXVECTOR3( 1.0f, 1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[10]	= paperVertex(D3DXVECTOR3( -1.0f, 1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[11]	= paperVertex(D3DXVECTOR3( 1.0f, 1.0f, -1.0f), color1, D3DXVECTOR2(1,1), D3DXVECTOR2(0,0));

	// Back Face (6-5-8-7), flip the vertex coordinates to show properly for some reason
	vertices[12]	= paperVertex(D3DXVECTOR3( 1.0f, 1.0f, 1.0f), color1, D3DXVECTOR2(1,1), D3DXVECTOR2(0,1));
	vertices[13]	= paperVertex(D3DXVECTOR3( -1.0f, 1.0f, 1.0f), color1, D3DXVECTOR2(0,1), D3DXVECTOR2(1,1));
	vertices[14]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, 1.0f), color1, D3DXVECTOR2(1,0), D3DXVECTOR2(0,0));
	vertices[15]	= paperVertex(D3DXVECTOR3( -1.0f, -1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(1,0));

	// Left Face (5-1-7-3)
	vertices[16]	= paperVertex(D3DXVECTOR3( -1.0f, 1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[17]	= paperVertex(D3DXVECTOR3( -1.0f, 1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[18]	= paperVertex(D3DXVECTOR3( -1.0f, -1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[19]	= paperVertex(D3DXVECTOR3( -1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));

	// Bottom Face (3-4-7-8)
	vertices[21]	= paperVertex(D3DXVECTOR3( -1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[22]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, -1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[22]	= paperVertex(D3DXVECTOR3( -1.0f, -1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
	vertices[23]	= paperVertex(D3DXVECTOR3( 1.0f, -1.0f, 1.0f), color1, D3DXVECTOR2(0,0), D3DXVECTOR2(0,0));
}

void PaperSheet::releaseVertices()
{
	delete[] vertices;
}

void PaperSheet::loadTextures()
{
	D3DXCreateTextureFromFile(md3dDev, "sheet.png", &textures[0]);
	textures[1] = textures[2] = textures[3] = textures[4] = NULL;	// TODO: Remove!
	D3DXCreateTextureFromFile(md3dDev, "hole.png", &holeTexture);										// TODO: Randomize
	md3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	md3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void PaperSheet::releaseTextures()
{
	for (int i = 0; i < NUM_TEXTURES; ++i) {
		// TODO: remove this check
		if (textures[i] != NULL) { textures[i]->Release(); }
	}
	delete[] textures;
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
		{0,0,D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		D3DDECL_END()
	};
	decl[0].Offset = (char *) &v.pos - (char *) &v;
	decl[1].Offset = (char *) &v.color - (char *) &v;
	decl[2].Offset = (char *) &v.texCoor - (char *) &v;
	decl[3].Offset = (char *) &v.texCoor2 - (char *) &v;
	
	md3dDev->CreateVertexDeclaration(decl, &paperDecl);

	// Randomize hole location
	randomizeHole();

	// Make sheet transparent
	createAlphaMask();

	// lock the buffer and copy the vertices (locking the entire array)		
	mVtxBuf->Lock(0,0,(void**)&vtx, 0);
	memcpy(vtx, vertices,VTX_NUM*sizeof(paperVertex));
	mVtxBuf->Unlock();

	///// Index Buffer
	/*
	md3dDev->CreateIndexBuffer(NUM_TRIANGLES*3*sizeof(long),
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX32,D3DPOOL_MANAGED,
							   &mIndBuf,
							   NULL);

	// lock the buffer and copy the indices (locking the entire array)
	mIndBuf->Lock(0, 0, (void**)&ind, 0);
	memcpy(ind, indices, (NUM_TRIANGLES*3)*sizeof(long));
	mIndBuf->Unlock();
	*/


	return 0;
}

void PaperSheet::randomizeHole()
{
	// TODO: Randomize!
	vertices[0].texCoor2 = vertices[13].texCoor2 = D3DXVECTOR2(1,1);
	vertices[1].texCoor2 = vertices[12].texCoor2 = D3DXVECTOR2(0,1);
	vertices[2].texCoor2 = vertices[15].texCoor2 = D3DXVECTOR2(1,0);
	vertices[3].texCoor2 = vertices[14].texCoor2 = D3DXVECTOR2(0,0);	
}

void PaperSheet::createAlphaMask()
{
	D3DLOCKED_RECT sheetRec, holeRec;
	D3DSURFACE_DESC sheetDesc, holeDesc;
	IDirect3DSurface9 *sheetSurf, *holeSurf;
	gTexture->GetSurfaceLevel(0, &sheetSurf);
	holeTexture->GetSurfaceLevel(0, &holeSurf);
	sheetSurf->GetDesc(&sheetDesc);
	holeSurf->GetDesc(&holeDesc);

	sheetSurf->LockRect(&sheetRec, NULL, 0);
	holeSurf->LockRect(&holeRec, NULL, 0);

	UINT sheetWidth = sheetDesc.Width;
	UINT sheetHeight = sheetDesc.Height;
	UINT holeWidth = holeDesc.Width;
	UINT holeHeight = holeDesc.Height;
	BYTE* sbuffer = (BYTE*)(sheetRec.pBits);
	BYTE* hbuffer = (BYTE*)(holeRec.pBits);

	// For some reason the y value needs to be multiplied by 4
	const int margin = 25;	// distance from edge
	UINT startingY = 0;//rand() % (sheetHeight - holeHeight - margin) + margin;
	UINT startingX = 0;//rand() % (sheetWidth - holeWidth - margin) + margin;

	int rowNum = 0;
	
	for (UINT y = startingY; y < (startingY + holeHeight); ++y) {		// Multiply by 4 for some reason
		for (UINT x = startingX; x < startingX + holeWidth; ++x) {
			DWORD sIndex = (x * 4) + (y * sheetRec.Pitch/4) ;//+ (rowNum * sheetWidth * 3);
			DWORD hIndex = ((x - startingX) * 4) + ((y - startingY) * holeRec.Pitch /4);
			//buffer[index] = (BYTE) 0;		// blue
			//buffer[index+1] = (BYTE) 0;	// green
			//buffer[index+2] = (BYTE) 0;	// red
			
			// since the hole is grayscale, RGB values should all be the same, so 255 = black, 0 = white
			sbuffer[sIndex+3] = (BYTE)hbuffer[hIndex];		// Alpha
		}
		rowNum++;
	}
	
	/*
	for (UINT y = 0; y < sheetHeight ; ++y) {		// Multiply by 4 for some reason
		for (UINT x = 0; x < sheetWidth; ++x) {
			DWORD index = (x * 4) + (y * sheetRec.Pitch/4) + (rowNum * sheetWidth * 3);
			sbuffer[index] = (BYTE) 255;		// blue
			sbuffer[index+1] = (BYTE) 0;	// green
			sbuffer[index+2] = (BYTE) 0;	// red
			sbuffer[index+3] = (BYTE)255;		// Alpha
		}
		rowNum++;
	}*/
	
	holeSurf->UnlockRect();
	sheetSurf->UnlockRect();

	/*
	FYI To loop through every pixel:
	for (UINT y = 0; y < sheetHeight * 4; ++y) {		// Multiply by 4 for some reason
		for (UINT x = 0; x < sheetWidth; ++x) {
			DWORD index = (x * 4) + (y * sheetRec.Pitch/4);
			buffer[index] = (BYTE) 0;		// blue
			buffer[index+1] = (BYTE) 0;	// green
			buffer[index+2] = (BYTE) 0;	// red
			buffer[index+3] = (BYTE)0;		// Alpha
		}
	}
	
	*/
	/*
	// TODO: Fix this as it is is broken!
	D3DSURFACE_DESC desc;
	D3DLOCKED_RECT rec;
	gTexture->GetLevelDesc(0, &desc);

	gTexture->LockRect(0, &rec, NULL, 0);

	UINT width = desc.Width;
	UINT height = desc.Height;
	DWORD* buffer = (DWORD*)(rec.pBits);
	UINT pitchDiff = (rec.Pitch / 2) - width;

	for (UINT y = 0; y < height; ++y) {
		for (UINT x = 0; x < width; ++x) {
			//buffer->a = 30;
			//buffer++;
			//int index = rec.Pitch * y + 4 * x;
			int index= (y*rec.Pitch/4)+x;
			buffer[index] = 0xffff0000;
		}
		buffer += pitchDiff;
	}
	*/

	gTexture->UnlockRect(0);
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

/*	//arg 1 is texture
	md3dDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//select arg 1 
	md3dDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);

	//arg 1 is texture
	md3dDev->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//arg 2 is last stage
	md3dDev->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_CURRENT);
	//multiply arguments
	md3dDev->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_MODULATE);
*/

	md3dDev->SetTexture(0, gTexture);
	//alpha blending enabled
	md3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//source blend factor
	md3dDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//destination blend factor
	md3dDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//alpha from texture
	md3dDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

	// If this isn't the floor (which most aren't) then deal with the potential hole
/*	if (!floor) {
		md3dDev->SetTexture(1, holeTexture);
		
		md3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		md3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	

		md3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		md3dDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		md3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE );
		md3dDev->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		md3dDev->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
		md3dDev->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
		md3dDev->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		md3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, true);
		md3dDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);//alpha
		md3dDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);//alpha
		
	}
	else {
		md3dDev->SetTexture(1, NULL);
	}
	*/

	// TODO: Find out if these lines are necessary
	//md3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//md3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//md3dDev->SetIndices(mIndBuf);
	for (int i = 0; i < 6; i++)
		md3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	//md3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
	//md3dDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, NUM_TRIANGLES);
	//DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VTX_NUM, 0, NUM_TRIANGLES);

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

void PaperSheet::setPitch(float newValue) {
	mPitch = newValue;
}

void PaperSheet::setFloor() {
	floor = true;
}