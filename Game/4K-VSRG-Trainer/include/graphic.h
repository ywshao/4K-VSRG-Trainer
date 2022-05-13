#pragma once

#include <mini/ini.h>
#include <SDL_gpu.h>
#include <SDL_ttf.h>
#include "..\lib\nfont-master\NFont\NFont_gpu.h"
#include <string>
#include "note.h"

enum {
	FONT_REGULAR = 0,
	FONT_JUDGE
};

class Graphic {
private:
	GPU_Target* window;
	TTF_Font* font[2];
	NFont nfont[2];
	GPU_Image* note[4];
	GPU_Image* receipter[4];
	GPU_Image* pressed[4];
	GPU_Image* judgeNote[7];
	GPU_Image* judgeKey[7];
public:
	void init(std::string texturePath, mINI::INIStructure& iniTexture);
	void exit();
	void clear();
	void draw();

	void drawNote();
	void drawReceipter();
	void drawKeyPressed(bool keyPressed[4]);
	void drawJudgeNote();
	void drawJudgeKey(std::pair<std::list<NoteTime>::iterator, std::list<NoteTime>::iterator> replayInterval);
	void drawText();
	void drawDebug(char* debugText[]);
};
