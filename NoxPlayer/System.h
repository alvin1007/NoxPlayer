#pragma once

#include "main.h"

#include "Graphic.h"
#include "Audio.h"
#include <fstream>

class System
{
public:
	System();
	~System();

	bool Init(HINSTANCE&);
	bool GetNoxPlayer();
	void Run();
	void Destroy();
private:
	bool InitWindow();
private:
	Graphic* m_Graphic;
	Audio* m_Audio;

	HWND m_hwnd, m_noxHwnd;
	HINSTANCE m_hinstance;
	const wchar_t* m_applicationName;

	int width = 1290;
	int height = 750;

	int posX = 0;
	int posY = 0;
};