#include "Image.h"
#pragma once
class Animation {
private:
	ID2D1RenderTarget* _pRenderTarget;
	std::vector<ImageR*> _list;
	int _frames;
public:
	Animation(ID2D1RenderTarget* pRenderTarget) : _pRenderTarget(pRenderTarget), _frames(0){}
	void append(
		IWICImagingFactory* pIWICFactory,
		PCWSTR uri,
		UINT destinationWidth,
		UINT destinationHeight
	);
	bool show(int x, int y, int& num);
	~Animation() {
		for (auto i : _list) {
			delete i;
		}
	}
};