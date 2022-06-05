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
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
}

void Audio::playSound(int index) {
	playingSounds.push_back(PlayingSound(&sound[index]));
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
	if (sfInfo.format & SF_FORMAT_FLOAT || sfInfo.format & SF_FORMAT_DOUBLE) {
		//printf("It's float!\n");
		sf_count_t count = 0;
		const int MAX_LEN = 256;
		float readData[MAX_LEN];
		while (count = sf_read_float(sndFile, readData, MAX_LEN)) {
			for (int idx = 0; idx < count;) {
				if (sfInfo.samplerate == 11025) {
					sound[index].left.push_back(readData[idx]);
					sound[index].left.push_back(readData[idx]);
					sound[index].left.push_back(readData[idx]);
					sound[index].left.push_back(readData[idx++]);
					sound[index].right.push_back(readData[idx]);
					sound[index].right.push_back(readData[idx]);
					sound[index].right.push_back(readData[idx]);
					sound[index].right.push_back(readData[idx++]);
				}
				else if (sfInfo.samplerate == 22050) {
					sound[index].left.push_back(readData[idx]);
					sound[index].left.push_back(readData[idx++]);
					sound[index].right.push_back(readData[idx]);
					sound[index].right.push_back(readData[idx++]);
				}
				else if (sfInfo.samplerate == 44100 && sfInfo.channels == 2) {
					sound[index].left.push_back(readData[idx++]);
					sound[index].right.push_back(readData[idx++]);
				}
				else if (sfInfo.samplerate == 44100 && sfInfo.channels == 1) {
					sound[index].left.push_back(readData[idx]);
					sound[index].right.push_back(readData[idx++]);
				}
			}
		}
	}
	else if (sfInfo.format & SF_FORMAT_OGG && (sfInfo.format % 0x10000) & SF_FORMAT_VORBIS) {
		//printf("It's OGG!\n");
		sf_count_t count = 0;
		const int MAX_LEN = 256;
		double readData[MAX_LEN];
		while (count = sf_read_double(sndFile, readData, MAX_LEN)) {
			for (int idx = 0; idx < count;) {
				sound[index].left.push_back(readData[idx++]);
				sound[index].right.push_back(readData[idx++]);
			}
		}
	}
	sf_close(sndFile);
}

void Audio::offloadSound(int index) {
	sound[index].left.clear();
	sound[index].right.clear();
}

void Audio::stopSound() {
	playingSounds.clear();
}

int Audio::getCurrentSoundNum() {
	return playingSounds.size();
}
