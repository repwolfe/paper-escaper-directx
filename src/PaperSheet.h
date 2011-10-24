#ifndef PAPER_SHEET_H
#define PAPER_SHEET_H

#include "GameObject.h"

#define VTX_NUM 8
#define NUM_TRIANGLES 12	// 6 * 2

struct paperVertex {
	D3DXVECTOR3 pos;
	DWORD color;        // The vertex color
	paperVertex(D3DXVECTOR3 p, DWORD c) {pos = p; color = c;}
	paperVertex() {pos = D3DXVECTOR3(0,0,0); color = 0;}
};

class PaperSheet : GameObject
{
public:
	PaperSheet();

	virtual int initGeom();
	virtual int render(int time);

private:
	paperVertex vertices[VTX_NUM];
	long indices[NUM_TRIANGLES * 3];		// 3 vertices per triangle

	static IDirect3DVertexDeclaration9* cubeDecl;
};

#endif