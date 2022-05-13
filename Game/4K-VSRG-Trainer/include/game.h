#pragma once

// Case sensitive for ini reader
// #define MINI_CASE_SENSITIVE

#include <SDL.h>
#include <mini/ini.h>
#include <string>
#include <cstdio>
#include "graphic.h"
#include "audioTest.h"
#include "note.h"

class Fps {
private:
	Uint64 tick;
	int frameCounter;
	int fps;
public:
	void init();
	bool check();
	void update();
	int get();
};

class Game {
private:
	Graphic graphic;
	Audio audio;
	KeyLogger keyLogger;
	Fps fps;
	SDL_Scancode keyMap[4] = {};
	bool keyPressed[4] = {};
	
public:
	void init();
	void exit();
	void update();
};