#include"Map.h"

void Map::mapShow(ID2D1RenderTarget* pRenderTarget)
{
	pRenderTarget->Clear();
	D2D1_POINT_2F upperLeft = D2D1::Point2F((FLOAT)(0), (FLOAT)(_cntY));
	if (_cntY == MAPLENGTH - WINDOWHEIGHT) _cntY = MAPLENGTH;	//背景滚动完毕，将标记移回初始位置，重新开始滚动
	_cntY += _speed;	//移动标记
	pRenderTarget->DrawBitmap(
		pBitMap,
		D2D1::RectF(0.0f, 0.0f, WINDOWWIDTH, WINDOWHEIGHT),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(upperLeft.x, upperLeft.y,
		(FLOAT)(upperLeft.x + WINDOWWIDTH), (FLOAT)(upperLeft.y - WINDOWHEIGHT))
	);
}
