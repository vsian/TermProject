#pragma once
#include"framework.h"
#include"Image.h"
#include"Data.h"
class Map {
private:
	ID2D1Bitmap* pBitMap = NULL;
	UINT _cntY = MAPLENGTH;
	UINT _speed = 0;
public:
	Map(PCWSTR filename, UINT speed,
		ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory) : _speed(speed){
		LoadBitmapFromFile(pRenderTarget, pIWICFactory, filename, WINDOWWIDTH, MAPLENGTH, &pBitMap);
	}
	void mapShow(ID2D1RenderTarget* pRenderTarget);
	~Map() {}
};