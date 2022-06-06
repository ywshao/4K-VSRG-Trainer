#include <cstdio>
#include <iterator>
#include <cstring>
#include "audioTest.h"

int Audio::callback(const void* inputBuffer,
	void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* userData) {
	std::list<PlayingSound>* playingSounds = (std::list<PlayingSound>*)userData;
	float output_left = 0;
	float output_right = 0;
	memset(outputBuffer, 0, 2 * framesPerBuffer * sizeof(float));
	if (playingSounds->empty()) {
		return 0;
	}
	for (std::list<PlayingSound>::iterator it = playingSounds->begin(); it != playingSounds->end(); it++) {
		float* output = (float*)outputBuffer;
		for (int times = 0; times < framesPerBuffer; times++) {
			*output++ += it->sound->left[it->playing_pos] * soundVolume;
			*output++ += it->sound->right[it->playing_pos] * soundVolume;
			if (++(it->playing_pos) >= it->sound->left.size()) {
				std::list<PlayingSound>::iterator eraseIt = it++;
				playingSounds->erase(eraseIt);
				if (it == playingSounds->end()) {
					return 0;
				}
				break;
			}
		}
	}
	return 0;
}

void Audio::portAudioInit(PaDeviceIndex deviceIndex) {
	PaError err = Pa_Initialize();
	//printf("Super duper test\n");
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
	std::vector<const PaDeviceInfo*> device;
	for (int a = 0; a < Pa_GetDeviceCount(); a++) {
		device.push_back(Pa_GetDeviceInfo(a));
	}
	static PaStreamParameters outputParameters = {
		deviceIndex, // device
		2, // channelCount
		paFloat32, // sampleFormat
		(double)framesPerBuffer / 44100, // suggestedLatency
		NULL // hostApiSpecificStreamInfo
	};
	Pa_OpenStream(&stream, nullptr, &outputParameters, 44100, framesPerBuffer, paNoFlag, callback, &playingSounds);
	Pa_StartStream(stream);
	return;
}

void Audio::portAudioExit() {
	PaError err = Pa_Terminate();
	//printf("Super duper test\n");
	//if (err != paNoError)
		//printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

void Audio::playSound(bool changeRate, int index) {
	if (changeRate) {
		playingSounds.push_back(PlayingSound(&newSound[index]));
	}
	else {
		playingSounds.push_back(PlayingSound(&sound[index]));
	}
}

void Audio::loadSound(int index, const char* path) {
	if (!strcmp(path, "0")) {
		sound[index].left.push_back(0);
		sound[index].right.push_back(0);
		return;
	}
	SF_INFO sfInfo;
	SNDFILE* sndFile = sf_open(path, SFM_READ, &sfInfo);
	//printf("Channels: %d\n", sfInfo.channels);
	//printf("Smaple rate: %d\n", sfInfo.samplerate);
	if (!sndFile) {
		char* newPath = _strdup(path);
		newPath[strlen(newPath) - 3] = 'o';
		newPath[strlen(newPath) - 2] = 'g';
		newPath[strlen(newPath) - 1] = 'g';
		sndFile = sf_open(newPath, SFM_READ, &sfInfo);
	}
	//printf("Format: %x\n", sfInfo.format);
	//printf("%x\n", SF_FORMAT_FLOAT);
	//printf("%x\n", SF_FORMAT_DOUBLE);
	//if ((sfInfo.format & 0xff) == SF_FORMAT_FLOAT || (sfInfo.format & 0xff) == SF_FORMAT_DOUBLE) {
		//printf("It's float!\n");
		sf_count_t count = 0;
		const int MAX_LEN = 256;
		float readData[MAX_LEN];
		while (count = sf_read_float(sndFile, readData, MAX_LEN)) {
			for (int idx = 0; idx < count;) {
				if (sfInfo.channels == 2) {
					sound[index].left.push_back(readData[idx++]);
					sound[index].right.push_back(readData[idx++]);
				}
				else if (sfInfo.channels == 1) {
					sound[index].left.push_back(readData[idx]);
					sound[index].right.push_back(readData[idx++]);
				}
			}
		}
	/* }
	else if ((sfInfo.format & 0xff) == SF_FORMAT_PCM_U8) {
		printf("It's uint!\n");
		sf_count_t count = 0;
		const int MAX_LEN = 256;
		float readData[MAX_LEN];
		while (count = sf_read_float(sndFile, readData, MAX_LEN)) {
			for (int idx = 0; idx < count;) {
				if (sfInfo.channels == 2) {
					sound[index].left.push_back((double)(readData[idx++] - 128) / 128);
					sound[index].right.push_back((double)(readData[idx++] - 128) / 128);
				}
				else if (sfInfo.channels == 1) {
					sound[index].left.push_back((double)(readData[idx] - 128) / 128);
					sound[index].right.push_back((double)(readData[idx++] - 128) / 128);
				}
			}
		}
	}
	else if ((sfInfo.format & 0xff) == SF_FORMAT_OGG && (sfInfo.format % 0x10000) == SF_FORMAT_VORBIS) {
		printf("It's OGG!\n");
		sf_count_t count = 0;
		const int MAX_LEN = 256;
		float readData[MAX_LEN];
		while (count = sf_read_float(sndFile, readData, MAX_LEN)) {
			for (int idx = 0; idx < count;) {
				if (sfInfo.channels == 2) {
					sound[index].left.push_back((double)(readData[idx++] - 128) / 128);
					sound[index].right.push_back((double)(readData[idx++] - 128) / 128);
				}
				else if (sfInfo.channels == 1) {
					sound[index].left.push_back((double)(readData[idx] - 128) / 128);
					sound[index].right.push_back((double)(readData[idx++] - 128) / 128);
				}
			}
		}
	}*/
	sf_close(sndFile);
	if (sfInfo.samplerate != 44100) {
		resample(index, sfInfo.samplerate);
	}
}

void Audio::resample(int index, int sampleRate) {
	if (sound[index].left.size() > 1) {
		HANDLE hLeft = soundtouch_createInstance();
		soundtouch_setChannels(hLeft, 1);
		soundtouch_setSampleRate(hLeft, 44100);
		soundtouch_setRate(hLeft, (float)sampleRate / 44100);
		float* soundLeft = new float[sound[index].left.size()];
		std::copy(sound[index].left.begin(), sound[index].left.end(), soundLeft);
		soundtouch_putSamples(hLeft, soundLeft, sound[index].left.size());
		int newSize = (int)((float)sound[index].left.size() * 44100 / sampleRate);
		float* newSoundLeft = new float[newSize];
		std::fill(newSoundLeft, newSoundLeft + newSize, 0);
		soundtouch_receiveSamples(hLeft, newSoundLeft, newSize);
		soundtouch_destroyInstance(hLeft);
		sound[index].left = std::vector<float>(newSoundLeft, newSoundLeft + newSize);
		delete[] soundLeft;
		delete[] newSoundLeft;
	}
	else {
		sound[index].left.push_back(0);
	}
	if (sound[index].right.size() > 1) {
		HANDLE hRight = soundtouch_createInstance();
		soundtouch_setChannels(hRight, 1);
		soundtouch_setSampleRate(hRight, 44100);
		soundtouch_setRate(hRight, (float)sampleRate / 44100);
		float* soundRight = new float[sound[index].right.size()];
		std::copy(sound[index].right.begin(), sound[index].right.end(), soundRight);
		soundtouch_putSamples(hRight, soundRight, sound[index].right.size());
		int newSize = (int)((float)sound[index].right.size() * 44100 / sampleRate);
		float* newSoundRight = new float[newSize];
		std::fill(newSoundRight, newSoundRight + newSize, 0);
		soundtouch_receiveSamples(hRight, newSoundRight, newSize);
		soundtouch_destroyInstance(hRight);
		sound[index].right = std::vector<float>(newSoundRight, newSoundRight + newSize);
		delete[] soundRight;
		delete[] newSoundRight;
	}
	else {
		sound[index].right.push_back(0);
	}
}

void Audio::changeRate(bool wsola, int rate, int index) {
	if (sound[index].left.size() > 1) {
		HANDLE hLeft = soundtouch_createInstance();
		soundtouch_setChannels(hLeft, 1);
		soundtouch_setSampleRate(hLeft, 44100);
		if (wsola) {
			soundtouch_setTempo(hLeft, (float)rate / 100);
			//soundtouch_setTempoChange(hLeft, ((float)rate - 100) / 100);
		}
		else {
			soundtouch_setRate(hLeft, (float)rate / 100);
			//soundtouch_setRateChange(hLeft, ((float)rate - 100) / 100);
		}
		float* soundLeft = new float[sound[index].left.size()];
		std::copy(sound[index].left.begin(), sound[index].left.end(), soundLeft);
		soundtouch_putSamples(hLeft, soundLeft, sound[index].left.size());
		int newSize = (int)((float)sound[index].left.size() * 100 / rate);
		float* newSoundLeft = new float[newSize];
		std::fill(newSoundLeft, newSoundLeft + newSize, 0);
		soundtouch_receiveSamples(hLeft, newSoundLeft, newSize);
		soundtouch_destroyInstance(hLeft);
		newSound[index].left = std::vector<float>(newSoundLeft, newSoundLeft + newSize);
		delete [] soundLeft;
		delete [] newSoundLeft;
	}
	else {
		newSound[index].left.push_back(0);
	}
	if (sound[index].right.size() > 1) {
		HANDLE hRight = soundtouch_createInstance();
		soundtouch_setChannels(hRight, 1);
		soundtouch_setSampleRate(hRight, 44100);
		if (wsola) {
			soundtouch_setTempo(hRight, (float)rate / 100);
			//soundtouch_setTempoChange(hRight, ((float)rate - 100) / 100);
		}
		else {
			soundtouch_setRate(hRight, (float)rate / 100);
			//soundtouch_setRateChange(hRight, ((float)rate - 100) / 100);
		}
		float* soundRight = new float[sound[index].right.size()];
		std::copy(sound[index].right.begin(), sound[index].right.end(), soundRight);
		soundtouch_putSamples(hRight, soundRight, sound[index].right.size());
		int newSize = (int)((float)sound[index].right.size() * 100 / rate);
		float* newSoundRight = new float[newSize];
		std::fill(newSoundRight, newSoundRight + newSize, 0);
		soundtouch_receiveSamples(hRight, newSoundRight, newSize);
		soundtouch_destroyInstance(hRight);
		newSound[index].right = std::vector<float>(newSoundRight, newSoundRight + newSize);
		delete [] soundRight;
		delete [] newSoundRight;
	}
	else {
		newSound[index].right.push_back(0);
	}
}

void Audio::offloadSound(int index) {
	sound[index].left = std::vector<float>();
	sound[index].right = std::vector<float>();
	offloadNewSound(index);
}

void Audio::offloadNewSound(int index) {
	newSound[index].left = std::vector<float>();
	newSound[index].right = std::vector<float>();
}

void Audio::stopSound() {
	playingSounds.clear();
}

int Audio::getCurrentSoundNum() {
	return playingSounds.size();
}
