#include "Graphic.h"

Graphic::Graphic()
{
}

Graphic::~Graphic()
{
}

bool Graphic::Init(HWND& m_hwnd)
{
    hdc = GetDC(m_hwnd);
    bitmap = NULL;

    return true;
}

void Graphic::End(HWND& m_hwnd)
{
    DestroyWindow(m_hwnd);
}

void Graphic::DrawBitmap()
{
    BITMAP bit;
    HDC mhdc = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(mhdc, bitmap);

    GetObject(bitmap, sizeof(BITMAP), &bit);

    BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, mhdc, 0, 0, SRCCOPY);

    SelectObject(mhdc, oldBitmap);
    DeleteObject(oldBitmap);
    DeleteDC(mhdc);
}

bool Graphic::NoxPlayerCapture(HWND& m_noxHwnd)
{
    HBITMAP bit, oldBit;

    HDC hdc = GetDC(m_noxHwnd);
    HDC hMemDC = CreateCompatibleDC(hdc);

    bit = CreateCompatibleBitmap(hdc, 1280, 720);
    oldBit = (HBITMAP)SelectObject(hMemDC, bit);
    if (!bit || !oldBit)
        return false;

    BitBlt(hMemDC, 0, 0, 1280, 720, hdc, 0, 0, SRCCOPY);

    SelectObject(hMemDC, oldBit);

    DeleteDC(hdc);
    DeleteDC(hMemDC);
    DeleteObject(oldBit);

    bitmap = bit;

    return true;
}

void Graphic::Reset()
{
    DeleteObject(bitmap);
}

void Graphic::Destroy()
{
    DeleteObject(bitmap);
    DeleteDC(hdc);
}