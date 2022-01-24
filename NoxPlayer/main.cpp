#include "main.h"
#include "System.h"

// https://glorypapa.tistory.com/18
// https://nowonbun.tistory.com/744
// https://kldp.org/node/135620

//Debug

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System* mainWindow = new System;

	if (!mainWindow->GetNoxPlayer())
	{
		std::cout << "녹스 앱 플레이어 실행을 확인하지 못했습니다." << std::endl;
		return 0;
	}

	mainWindow->Init(hInstance);
	mainWindow->Run();
	mainWindow->Destroy();

	delete mainWindow;
	mainWindow = nullptr;

	return 0;
}

// Release
/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System* mainWindow = new System;

	if (!mainWindow->GetNoxPlayer())
	{
		std::cout << "녹스 앱 플레이어 실행을 확인하지 못했습니다." << std::endl;
		return 0;
	}

	mainWindow->Init(hInstance);
	mainWindow->Run();
	mainWindow->Destroy();

	delete mainWindow;
	mainWindow = nullptr;

	return 0;
}
*/