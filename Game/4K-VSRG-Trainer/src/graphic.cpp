#include "graphic.h"

void Graphic::init(std::string texturePath, mINI::INIStructure& iniTexture) {
	// SDL_gpu init
	window = GPU_Init(1280, 720, GPU_INIT_DISABLE_VSYNC);
	SDL_GL_SetSwapInterval(0);
	// Font init
	TTF_Init();
	font[FONT_REGULAR] = TTF_OpenFont("texture/Roboto-Regular.ttf", 40);
	nfont[FONT_REGULAR].load(font[FONT_REGULAR]);
	font[FONT_JUDGE] = TTF_OpenFont("texture/BebasNeue-Regular.ttf", 80);
	nfont[FONT_JUDGE].load(font[FONT_JUDGE]);
	// Load images
	note[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note1"]).c_str());
	note[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note2"]).c_str());
	note[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note3"]).c_str());
	note[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note4"]).c_str());
	receipter[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receipter1"]).c_str());
	receipter[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receipter2"]).c_str());
	receipter[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receipter3"]).c_str());
	receipter[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receipter4"]).c_str());
	pressed[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["pressed1"]).c_str());
	pressed[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["pressed2"]).c_str());
	pressed[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["pressed3"]).c_str());
	pressed[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["pressed4"]).c_str());
	judgeNote[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote1"]).c_str());
	judgeNote[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote2"]).c_str());
	judgeNote[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote3"]).c_str());
	judgeNote[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote4"]).c_str());
	judgeNote[4] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote5"]).c_str());
	judgeNote[5] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote6"]).c_str());
	judgeNote[6] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeNote7"]).c_str());
	judgeKey[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey1"]).c_str());
	judgeKey[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey2"]).c_str());
	judgeKey[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey3"]).c_str());
	judgeKey[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey4"]).c_str());
	judgeKey[4] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey5"]).c_str());
	judgeKey[5] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey6"]).c_str());
	judgeKey[6] = GPU_LoadImage((texturePath + iniTexture["Texture"]["judgeKey7"]).c_str());
	errorMeter[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeter1"]).c_str());
	errorMeter[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeter2"]).c_str());
	errorMeter[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeter3"]).c_str());
	errorMeter[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeter4"]).c_str());
	errorMeter[4] = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeter5"]).c_str());
	errorMeter[5] = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeter6"]).c_str());
	errorMeterMiddle = GPU_LoadImage((texturePath + iniTexture["Texture"]["errorMeterMiddle"]).c_str());
}

void Graphic::exit() {
	TTF_Quit();
	GPU_Quit();
}

void Graphic::clear() {
	GPU_Clear(window);
}

void Graphic::draw() {
	GPU_Flip(window);
}

void Graphic::drawNote(std::list<KeySound>::iterator begin[4], std::list<KeySound>::iterator end[4], float scrollSpeed) {
	//GPU_Blit(note[0], NULL, window, 0, 0);
	//GPU_Blit(note[1], NULL, window, 200, 200);
	for (int key = 0; key < 4; key++) {
		for (std::list<KeySound>::iterator iter = begin[key]; iter != end[key]; iter++) {
			//GPU_Blit(note[key], NULL, window, 200 + key * 100, (SDL_GetTicks64() - iter->time) * scrollSpeed + receipterY);
			GPU_Blit(note[key], NULL, window, 200 + key * 100, (int)(SDL_GetTicks64() - iter->time) * scrollSpeed + receipterY);
		}
	}
	/*
	GPU_Blit(note[0], NULL, window, 200, SDL_GetTicks64() % 800);
	GPU_Blit(note[1], NULL, window, 300, (SDL_GetTicks64() + 100) % 800);
	GPU_Blit(note[2], NULL, window, 400, (SDL_GetTicks64() + 200) % 800);
	GPU_Blit(note[3], NULL, window, 500, (SDL_GetTicks64() + 200) % 800);
	GPU_Blit(note[0], NULL, window, 200, (SDL_GetTicks64() + 400) % 800);
	GPU_Blit(note[1], NULL, window, 300, (SDL_GetTicks64() + 500) % 800);
	GPU_Blit(note[2], NULL, window, 400, (SDL_GetTicks64() + 600) % 800);
	GPU_Blit(note[3], NULL, window, 500, (SDL_GetTicks64() + 600) % 800);*/
}

void Graphic::drawReceipter() {
	GPU_Blit(receipter[0], NULL, window, 200, receipterY);
	GPU_Blit(receipter[1], NULL, window, 300, receipterY);
	GPU_Blit(receipter[2], NULL, window, 400, receipterY);
	GPU_Blit(receipter[3], NULL, window, 500, receipterY);
}

void Graphic::drawKeyPressed(bool keyPressed[4]) {
	for (int key = 0; key < 4; key++) {
		if (keyPressed[key]) {
			GPU_Blit(pressed[key], NULL, window, 200 + key * 100, receipterY);
		}
	}
}

void Graphic::drawJudgeNote(std::list<JudgeKeySound>::iterator begin[4], std::list<JudgeKeySound>::iterator end[4]) {
	for (int key = 0; key < 4; key++) {
		std::list<JudgeKeySound>::iterator iter = begin[key];
		for (; iter != end[key]; iter++) {
			GPU_Blit(judgeNote[iter->judge], NULL, window, 700 + key * 100, SDL_GetTicks64() - iter->time);
		}
	}
}

void Graphic::drawJudgeKey(std::list<JudgeKeySound>::iterator begin[4], std::list<JudgeKeySound>::iterator end[4]) {
	for (int key = 0; key < 4; key++) {
		std::list<JudgeKeySound>::iterator iter = begin[key];
		for (; iter != end[key]; iter++) {
			GPU_Blit(judgeKey[iter->judge], NULL, window, 700 + key * 100, SDL_GetTicks64() - iter->time);
		}
	}
}

void Graphic::drawJudge(int judge) {
	static int staticJudge = 6;
	staticJudge = judge != 6 ? judge : staticJudge;
	switch (staticJudge) {
	case 0:
		nfont[FONT_JUDGE].draw(window, 200, 200, SDL_Color{ 0, 255, 255, 255 }, "MARVELOUS");
		break;
	case 1:
		nfont[FONT_JUDGE].draw(window, 200, 200, SDL_Color{ 255, 255, 0, 255 }, "PERFECT");
		break;
	case 2:
		nfont[FONT_JUDGE].draw(window, 200, 200, SDL_Color{ 0, 255, 0, 255 }, "GREAT");
		break;
	case 3:
		nfont[FONT_JUDGE].draw(window, 200, 200, SDL_Color{ 0, 0, 255, 255 }, "GOOD");
		break;
	case 4:
		nfont[FONT_JUDGE].draw(window, 200, 200, SDL_Color{ 255, 0, 255, 255 }, "BAD");
		break;
	case 5:
		nfont[FONT_JUDGE].draw(window, 200, 200, SDL_Color{ 255, 0, 0, 255 }, "MISS");
		break;
	}
}

void Graphic::drawErrorMeter(std::list<JudgeErrorTime>::iterator begin, std::list<JudgeErrorTime>::iterator end, int lifeTime, float scale) {
	GPU_Blit(errorMeterMiddle, NULL, window, 350, 300);
	for (std::list<JudgeErrorTime>::iterator iter = begin; iter != end; iter++) {
		int alpha = 255 - (float)255 * (SDL_GetTicks64() - iter->time) / lifeTime;
		GPU_SetRGBA(errorMeter[iter->judge], 255, 255, 255, alpha >= 0 ? alpha : 0);
		GPU_Blit(errorMeter[iter->judge], NULL, window, 350 + iter->error * scale, 300);
	}
}

void Graphic::drawText() {
	nfont[FONT_REGULAR].draw(window, 600, 300, SDL_Color{ 255, 255, 255, 255 }, "Sound sample taken from:\nDefeat awaken battle ship\nArtist: Sakamiya\n\nPure Ruby\nArtist: SHIKI\nRIP");
}

void Graphic::drawDebug(char* debugText[]) {
	for (int index = 0; debugText[index]; index++) {
		nfont[FONT_REGULAR].draw(window, 10, index * 50, SDL_Color{ 255, 255, 255, 255 }, debugText[index]);
	}
}
