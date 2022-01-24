#pragma once

#include "main.h"
#include <MMSystem.h>
#include <thread>
#include <chrono>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32")

void StartAudioSpeakerThread(HWAVEOUT&, WAVEHDR&, int);

class Audio
{
public:
	Audio();
	~Audio();

	bool Init();
	void Destroy();

	void Start();
	void End();
private:
	std::thread AudioCapture;
	std::thread AudioSpeaker;

	HWAVEIN hWaveIn;
	WAVEHDR WaveInHdr;
	HWAVEOUT hWaveOut;
	WAVEFORMATEX Format;

	const int NUMPTS = 44100 * 10;
	short int WaveIn[5000000];

	int Time = 10;
};