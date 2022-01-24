#include "Audio.h"

//https://graphics.stanford.edu/~mdfisher/Code/Engine/AudioCapture.cpp.html

AudioBuffer* WaveBuffer = new AudioBuffer;
std::mutex mutex;

void CALLBACK waveInProc(HWAVEIN hWaveIn, UINT uMsg, DWORD dwIntance, DWORD dwParam1, DWORD dwParam2)
{
	switch (uMsg)
	{
	case WIM_DATA:
	{
		MMRESULT result;
		WAVEHDR* WaveHdr = (WAVEHDR*)dwParam1;
		std::cout << "callback" << std::endl;

		// mutex.lock();

		/*
		result = waveInReset(hWaveIn);
		if (result)
		{
			mutex.unlock();
			return;
		}
		*/

		/*
		result = waveInPrepareHeader(hWaveIn, WaveHdr, sizeof WAVEHDR);
		if (result)
		{
			std::cout << "waveInPrepareHeader Error" << std::endl;
			waveInClose(hWaveIn);
			mutex.unlock();
			return;
		}

		*/
		result = waveInAddBuffer(hWaveIn, WaveHdr, sizeof WAVEHDR);
		if (result)
		{
			std::cout << "waveInAddBuffer Error" << std::endl;
			waveInClose(hWaveIn);
			// mutex.unlock();
			return;
		}

		// mutex.unlock();

		break;
	}
	}
}

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

	WaveBuffer->Init();

	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &Format, 0, 0, WAVE_FORMAT_DIRECT);
	if (result)
		std::cout << "waveInOpen Error" << std::endl;

	//result = waveInOpen(&hWaveIn, WAVE_MAPPER, &Format, (DWORD_PTR)waveInProc, 0, CALLBACK_FUNCTION);
	//if (result)
	//std::cout << "waveInOpen Error" << std::endl;

	result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &Format, 0, 0, WAVE_FORMAT_DIRECT);
	if (result)
		std::cout << "Out Open Error" << std::endl;

	return true;
}

void Audio::Destroy()
{
	waveInClose(hWaveIn);
	waveOutClose(hWaveOut);
}


void Audio::Start()
{
	AudioCapture = std::thread(StartAudioCaptureThread, std::ref(hWaveIn), std::ref(Format));
	AudioSpeaker = std::thread(StartAudioSpeakerThread, std::ref(hWaveOut), std::ref(hWaveIn), std::ref(Format));
}

void Audio::End()
{
	Sleep(1000 * 20);
}

void StartAudioCaptureThread(HWAVEIN& hWaveIn, WAVEFORMATEX& Format)
{
	MMRESULT result;

	result = WaveBuffer->CaptureStart(hWaveIn);
	if (!result)
		std::cout << "CaptureStartError" << std::endl;
E1:
	Sleep(1000);

	mutex.lock();

	WaveBuffer->CaptureReSet(hWaveIn);

	mutex.unlock();

	goto E1;

	result = WaveBuffer->CaptureEnd(hWaveIn);
	if (!result)
		std::cout << "CaptureEndError" << std::endl;
}

void StartAudioSpeakerThread(HWAVEOUT& hWaveOut, HWAVEIN& hWaveIn, WAVEFORMATEX& Format)
{
	MMRESULT result;

	Sleep(2000); // ÃÊ¹Ý µô·¹ÀÌ

	while (true)
	{
		std::cout << "hello" << std::endl;
		mutex.lock();

		result = WaveBuffer->SpeakerStart(hWaveOut);
		if (!result)
		{
			std::cout << "SpeakerStartError" << std::endl;
			break;
		}

		Sleep(1000);
		waveInReset(hWaveIn);

		if (!result)
			break;

		mutex.unlock();

		Sleep(100);
	}

	mutex.unlock();
	std::cout << " Speaker Error" << std::endl;
}

AudioBuffer::AudioBuffer()
{
}

AudioBuffer::~AudioBuffer()
{
}

MMRESULT AudioBuffer::Init()
{
	WaveBuffer = (short int *)malloc(sizeof(short int *) * BufferSize);

	WaveHdr.lpData = (LPSTR)WaveBuffer;
	WaveHdr.dwBufferLength = 176400;
	WaveHdr.dwBytesRecorded = 0;
	WaveHdr.dwUser = 0;
	WaveHdr.dwFlags = 0;
	WaveHdr.dwLoops = 0;

	return true;
}

MMRESULT AudioBuffer::CaptureStart(HWAVEIN& hWaveIn)
{
	MMRESULT result;

	mutex.lock();

	result = waveInPrepareHeader(hWaveIn, &WaveHdr, sizeof WAVEHDR);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	result = waveInAddBuffer(hWaveIn, &WaveHdr, sizeof WAVEHDR);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	result = waveInStart(hWaveIn);
	if (result)
	{
		mutex.unlock();
		return !result;
	}
	
	mutex.unlock();

	return true;
}

MMRESULT AudioBuffer::CaptureReSet(HWAVEIN& hWaveIn)
{
	MMRESULT result;

	mutex.lock();

	// BufferReset();

	result = waveInPrepareHeader(hWaveIn, &WaveHdr, sizeof WAVEHDR);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	result = waveInAddBuffer(hWaveIn, &WaveHdr, sizeof WAVEHDR);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	result = waveInReset(hWaveIn);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	mutex.unlock();

	return true;
}

MMRESULT AudioBuffer::CaptureEnd(HWAVEIN& hWaveIn)
{
	MMRESULT result;

	mutex.lock();

	result = waveInStop(hWaveIn);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	result = waveInReset(hWaveIn);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	mutex.unlock();

	return true;
}

MMRESULT AudioBuffer::SpeakerStart(HWAVEOUT& hWaveOut)
{
	MMRESULT result;

	mutex.lock();

	result = waveOutPrepareHeader(hWaveOut, &WaveHdr, sizeof WAVEHDR);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	result = waveOutWrite(hWaveOut, &WaveHdr, sizeof WAVEHDR);
	if (result)
	{
		mutex.unlock();
		return !result;
	}

	mutex.unlock();

	return true;
}

MMRESULT AudioBuffer::SpeakerEnd(HWAVEOUT& hWaveOut)
{
	mutex.lock();
	waveOutReset(hWaveOut);
	mutex.unlock();

	return true;
}

MMRESULT AudioBuffer::BufferReset()
{
	mutex.lock();

	delete WaveBuffer;
	WaveBuffer = (short int*)malloc(sizeof(short int*) * BufferSize);

	WaveHdr.lpData = (LPSTR)WaveBuffer;

	mutex.unlock();

	return true;
}