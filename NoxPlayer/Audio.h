#pragma once

#include "main.h"
#include <MMSystem.h>
#include <thread>
#include <chrono>
#include <mutex>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32")


void CALLBACK waveInProc(HWAVEIN, UINT, DWORD, DWORD, DWORD);
void StartAudioCaptureThread(HWAVEIN&, WAVEFORMATEX&);
void StartAudioSpeakerThread(HWAVEOUT&, HWAVEIN&, WAVEFORMATEX&);

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
	HWAVEOUT hWaveOut;

	WAVEFORMATEX Format;

	const int NUMPTS = 44100 * 10;
	short int WaveIn[5000000];
};

class AudioBuffer
{
public:
	AudioBuffer();
	~AudioBuffer();

	MMRESULT Init();

	MMRESULT CaptureStart(HWAVEIN&);
	MMRESULT CaptureReSet(HWAVEIN&);
	MMRESULT CaptureEnd(HWAVEIN&);

	MMRESULT SpeakerStart(HWAVEOUT&);
	MMRESULT SpeakerEnd(HWAVEOUT&);

	MMRESULT BufferReset();
private:
	WAVEHDR WaveHdr;
	std::mutex mutex;

	short int* WaveBuffer;
	int BufferSize = 200000;
};