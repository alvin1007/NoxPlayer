#pragma once

#include "main.h"

class Graphic
{
public:
	Graphic();
	~Graphic();

	bool Init(HWND&);
	void End(HWND&);

	void DrawBitmap();
	bool NoxPlayerCapture(HWND&);
	void Reset();
	void Destroy();
private:
	HDC hdc;
	HBITMAP bitmap;
};