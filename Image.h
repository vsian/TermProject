//图片处理 用WIC组件从文件名读取位图并转化为Direc2d位图
#pragma once
#include "framework.h"
//最后位图的信息存入ppBitmap
HRESULT LoadBitmapFromFile(
    ID2D1RenderTarget* pRenderTarget,
    IWICImagingFactory* pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap** ppBitmap
);
class ImageR {
private:
    ID2D1Bitmap* _pBitMap = NULL;
    ID2D1RenderTarget* _pRenderTarget = NULL;
public:
    ImageR(
        ID2D1RenderTarget* pRenderTarget,
        IWICImagingFactory* pIWICFactory,
        PCWSTR uri,
        UINT destinationWidth,
        UINT destinationHeight) :  _pRenderTarget(pRenderTarget){
        LoadBitmapFromFile(
            pRenderTarget,
            pIWICFactory,
            uri,
            destinationWidth,
            destinationHeight,
            &_pBitMap
        );
    }
    void Draw(int dx, int dy, int x, int y, int width, int height);
    void Draw(int dx, int dy);
    ~ImageR() {
        SAFE_RELEASE(_pBitMap);
    }
};
