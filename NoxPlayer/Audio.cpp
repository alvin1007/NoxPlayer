#include "Audio.h"

//https://graphics.stanford.edu/~mdfisher/Code/Engine/AudioCapture.cpp.html

Audio::Audio()
{
}

Audio::~Audio()
{
}

bool Audio::Init()
{
	MMRESULT result;

	Format.wFormatTag = WAVE_FORMAT_PCM;
	Format.nChannels = 2;
	Format.nSamplesPerSec = 44100;
	Format.nBlockAlign = 4;
	Format.nAvgBytesPerSec = 44100 * 4;
	Format.wBitsPerSample = 16;
	Format.cbSize = 0;

	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &Format, 0, 0, WAVE_FORMAT_DIRECT);
	if (result)
		return false;


	WaveInHdr.lpData = (LPSTR)WaveIn;
	WaveInHdr.dwBufferLength = 1000000;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0;
	WaveInHdr.dwFlags = 0;
	WaveInHdr.dwLoops = 0;
	waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
	if (result)
		return false;


	// result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &Format, 0, 0, WAVE_FORMAT_DIRECT);
	// if (result)
	//  return false;

	return true;
}

void Audio::Destroy()
{
	waveOutUnprepareHeader(hWaveOut, &WaveInHdr, sizeof(WAVEHDR));
	waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

	waveInClose(hWaveIn);
	waveOutClose(hWaveOut);
}


void Audio::Start()
{
	MMRESULT result;

	AudioSpeaker = std::thread(StartAudioSpeakerThread, hWaveOut, WaveInHdr);

	result = waveInStart(hWaveIn);
	if (result)
		std::cout << "오디오 입력이 시작되지 못했습니다." << std::endl;

	// AudioSpeaker = std::thread(StartAudioSpeakerThread, std::ref(hWaveOut), std::ref(WaveInHdr), BufferLength / SampleRate * 1000);

}

void Audio::End()
{

	MMRESULT result;

	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &Format, 0, 0, WAVE_FORMAT_DIRECT);
	if (result)
		std::cout << "Out Open Error" << std::endl;

	result = waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr));
	if (result)
		std::cout << "Out Write Error" << std::endl;

	Sleep(1000 * 20);
}

void StartAudioSpeakerThread(HWAVEOUT& hWaveOut, WAVEHDR& WaveInHdr, int WaitSeconds)
{
	while (true)
	{
		if (waveOutReset(hWaveOut))
		{
			std::cout << "waveOutPrepareHeader Error" << std::endl;
			break;
		}

		// 장치에 출력 준비를 알리는 함수
		if (waveOutPrepareHeader(hWaveOut, &WaveInHdr, sizeof(WAVEHDR)))
		{
			std::cout << "waveOutPrepareHeader Error" << std::endl;
			break;
		}

		// 출력 시작
		if (waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WAVEHDR)))
		{
			std::cout << "waveOutPrepareHeader Error" << std::endl;
			break;
		}
	}
	/*
	MMRESULT result;

	result = waveOutWrite(hWaveOut, &WaveInHdr, sizeof(WaveInHdr));
	if (result)
		std::cout << "오디오 출력이 시작되지 못했습니다." << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(WaitSeconds));
	*/
}