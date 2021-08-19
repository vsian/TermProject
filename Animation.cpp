#include "Animation.h"

void Animation::append(IWICImagingFactory* pIWICFactory, PCWSTR uri, UINT destinationWidth, UINT destinationHeight)
{
	_list.push_back(new ImageR(_pRenderTarget, pIWICFactory, uri, destinationWidth, destinationHeight));
	_frames = _list.size();
}

bool Animation::show(int x, int y, int& num)
{
	if (num >= _frames) {
		return false;
	}
	_list[num++]->Draw(x, y);
	return num < _frames;
}
