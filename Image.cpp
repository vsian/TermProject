#include "Image.h"
//图片处理 用WIC组件从文件名读取位图并转化为Direc2d位图
#pragma once
#include "framework.h"
//最后位图的信息存入ppBitmap指向的地址
HRESULT LoadBitmapFromFile(
    ID2D1RenderTarget* pRenderTarget,
    IWICImagingFactory* pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap** ppBitmap
)
{
    HRESULT hr = S_OK;

    IWICBitmapDecoder* pDecoder = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICStream* pStream = NULL;
    IWICFormatConverter* pConverter = NULL;
    IWICBitmapScaler* pScaler = NULL;

    hr = pIWICFactory->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
    if (SUCCEEDED(hr))
    {
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }

    if (destinationWidth != 0 || destinationHeight != 0)
    {
        UINT originalWidth, originalHeight;
        hr = pSource->GetSize(&originalWidth, &originalHeight);
        if (SUCCEEDED(hr))
        {
            if (destinationWidth == 0)
            {
                FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
            }
            else if (destinationHeight == 0)
            {
                FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
            }

            hr = pIWICFactory->CreateBitmapScaler(&pScaler);
            if (SUCCEEDED(hr))
            {
                hr = pScaler->Initialize(
                    pSource,
                    destinationWidth,
                    destinationHeight,
                    WICBitmapInterpolationModeCubic
                );
            }
            if (SUCCEEDED(hr))
            {
                hr = pConverter->Initialize(
                    pScaler,
                    GUID_WICPixelFormat32bppPBGRA,
                    WICBitmapDitherTypeNone,
                    NULL,
                    0.f,
                    WICBitmapPaletteTypeMedianCut
                );
            }
        }
    }
    if (SUCCEEDED(hr))
    {
        hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, ppBitmap);
    }
    SAFE_RELEASE(pDecoder);
    SAFE_RELEASE(pSource);
    SAFE_RELEASE(pStream);
    SAFE_RELEASE(pConverter);
    SAFE_RELEASE(pScaler);
    return hr;
}

void ImageR::Draw(int dx, int dy, int x, int y, int width, int height)
{
    _pRenderTarget->DrawBitmap(
        _pBitMap,
        D2D1::RectF((FLOAT)dx, (FLOAT)dy, (FLOAT)(dx + width), (FLOAT)(dy + height)),
        1.0F,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)(x + width), (FLOAT)(y + height))
    );
}

void ImageR::Draw(int dx, int dy)
{
    INT width = _pBitMap->GetPixelSize().width;
    INT height = _pBitMap->GetPixelSize().height;
    _pRenderTarget->DrawBitmap(
        _pBitMap,
        D2D1::RectF((FLOAT)dx, (FLOAT)dy, (FLOAT)(dx + width), (FLOAT)(dy + height)),
        1.0F,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        D2D1::RectF((FLOAT)0, (FLOAT)0, (FLOAT)(width), (FLOAT)(height))
    );
}
