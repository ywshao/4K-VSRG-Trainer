#pragma once

// Case sensitive for ini reader
// #define MINI_CASE_SENSITIVE

#include <SDL.h>
#include <mini/ini.h>
#include <string>
#include <cstdio>
#include <filesystem>
#include <thread>
#include "graphic.h"
#include "audioTest.h"
#include "bmsParser.h"
#include "bms7to4.h"
#include "note.h"
#include "score.h"

enum GameType {
	Train = 0,
	Dan,
	Exit
};

enum GameState {
	SelectGame = 0,
	SelectBms,
	SelectDan,
	SelectFile,
	SelectDifficulty,
	Play
};

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
	GameState gameState;
	Graphic graphic;
	Audio audio;
	PaDeviceIndex audioDeviceIndex;
	BmsParser bmsParser;
	Uint64 chartOffset;
	JudgeKey judgeKey;
	JudgeVisible judgeKeyVisible;
	JudgeVisible judgeNoteVisible;
	Chart chart;
	ChartVisible chartVisible;
	Score score;
	ErrorMeter errorMeter;
	Fps fps;
	SDL_Scancode keyMap[11] = {};
	bool keyPressed[11] = {};
	Uint64 keyDelay[11] = {};
	Uint64 key;

	std::vector<std::filesystem::path> bmsDir;
	std::vector<std::vector<std::filesystem::path>> bmsFileDir;
	int gameSelect = 0;
	int bmsSelect = 0;
	int bmsFileSelect = 0;
	int difficultySelect = 0;
	PatternParameter patternParameter;

	double scrollSpeed;

	std::filesystem::path bmsPath;
	void patternParameterWrite(std::string iniPath);
	void bmsInit();
	
public:
	void init();
	void exit();
	void update();
};
