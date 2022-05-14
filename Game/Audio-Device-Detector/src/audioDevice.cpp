#include <portaudio.h>
#include <cstdlib>
#include <cstdio>

int main() {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
	}
	printf("Num\tDevice\tInputChannels\tOutputChannels\n");
	for (int a = 0, index = 0; a < Pa_GetDeviceCount(); a++) {
		if (Pa_GetDeviceInfo(a)->maxOutputChannels) {
			printf("%d\t%s\t%d\t%d\n", index++, Pa_GetDeviceInfo(a)->name, Pa_GetDeviceInfo(a)->maxInputChannels, Pa_GetDeviceInfo(a)->maxOutputChannels);
		}
	}
	system("PAUSE");
	return 0;
}
