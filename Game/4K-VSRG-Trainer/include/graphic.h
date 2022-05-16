#pragma once

#include <mini/ini.h>
#include <SDL_gpu.h>
#include <SDL_ttf.h>
#include "..\..\lib\nfont-master\NFont\NFont_gpu.h"
#include <string>
#include "note.h"
#include "score.h"

#define receipterY 600
#define fontNum 3

enum {
	FONT_REGULAR = 0,
	FONT_DEBUG,
	FONT_JUDGE
};

class Graphic {
private:
	GPU_Target* window;
	TTF_Font* font[fontNum];
	NFont nfont[fontNum];
	GPU_Image* note[4];
	GPU_Image* receipter[4];
	GPU_Image* pressed[4];
	GPU_Image* judgeNote[7];
	GPU_Image* judgeKey[7];
	GPU_Image* errorMeterMiddle;
	GPU_Image* errorMeter[7];
public:
	void init(std::string texturePath, mINI::INIStructure& iniTexture);
	void exit();
	void clear();
	void draw();

	void drawNote(std::list<KeySound>::iterator begin[4], std::list<KeySound>::iterator end[4], float scrollSpeed);
	void drawReceipter();
	void drawKeyPressed(bool keyPressed[4]);
	void drawJudgeNote(std::list<JudgeKeySound>::iterator begin[4], std::list<JudgeKeySound>::iterator end[4]);
	void drawJudgeKey(std::list<JudgeKeySound>::iterator begin[4], std::list<JudgeKeySound>::iterator end[4]);
	void drawJudge(int judge);
	void drawErrorMeter(std::list<JudgeErrorTime>::iterator begin, std::list<JudgeErrorTime>::iterator end, int lifeTime, float scale);
	void drawText();
	void drawDebug(char* debugText[]);
};
