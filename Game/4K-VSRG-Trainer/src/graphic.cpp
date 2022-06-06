#include "graphic.h"

void Graphic::init(std::string texturePath, mINI::INIStructure& iniTexture) {
	// SDL_gpu init
	window = GPU_Init(1280, 720, GPU_INIT_DISABLE_VSYNC);
	SDL_GL_SetSwapInterval(0);
	// Font init
	TTF_Init();
	font[FONT_REGULAR] = TTF_OpenFont("texture/Roboto-Regular.ttf", 40);
	nfont[FONT_REGULAR].load(font[FONT_REGULAR]);
	font[FONT_DEBUG] = TTF_OpenFont("texture/Roboto-Regular.ttf", 30);
	nfont[FONT_DEBUG].load(font[FONT_DEBUG]);
	font[FONT_JUDGE] = TTF_OpenFont("texture/BebasNeue-Regular.ttf", 80);
	nfont[FONT_JUDGE].load(font[FONT_JUDGE]);
	// Load images
	note[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note1"]).c_str());
	note[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note2"]).c_str());
	note[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note3"]).c_str());
	note[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["note4"]).c_str());
	receiptor[0] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receiptor1"]).c_str());
	receiptor[1] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receiptor2"]).c_str());
	receiptor[2] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receiptor3"]).c_str());
	receiptor[3] = GPU_LoadImage((texturePath + iniTexture["Texture"]["receiptor4"]).c_str());
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

void Graphic::drawNote(ChartVisible &chartVisible, float scrollSpeed, Uint64 chartOffset) {
	//GPU_Blit(note[0], NULL, window, 0, 0);
	//GPU_Blit(note[1], NULL, window, 200, 200);
	for (int key = 0; key < 4; key++) {
		for (std::list<KeySound>::iterator iter = chartVisible.begin(key); iter != chartVisible.end(key); iter++) {
			//GPU_Blit(note[key], NULL, window, 300 + key * 100, (SDL_GetTicks64() - iter->time) * scrollSpeed + receipterY);
			GPU_Blit(note[key], NULL, window, 300 + key * 100, (int)(SDL_GetTicks64() - chartOffset - iter->time) * scrollSpeed + receipterY);
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

void Graphic::drawReceiptor() {
	GPU_Blit(receiptor[0], NULL, window, 300, receipterY);
	GPU_Blit(receiptor[1], NULL, window, 400, receipterY);
	GPU_Blit(receiptor[2], NULL, window, 500, receipterY);
	GPU_Blit(receiptor[3], NULL, window, 600, receipterY);
}

void Graphic::drawKeyPressed(bool keyPressed[4]) {
	for (int key = 0; key < 4; key++) {
		if (keyPressed[key]) {
			GPU_Blit(pressed[key], NULL, window, 300 + key * 100, receipterY);
		}
	}
}

void Graphic::drawJudgeNote(JudgeVisible& judgeNoteVisible, Uint64 chartOffset) {
	for (int key = 0; key < 4; key++) {
		std::list<JudgeKeySound>::iterator iter = judgeNoteVisible.begin(key);
		for (; iter != judgeNoteVisible.end(key); iter++) {
			GPU_Blit(judgeNote[iter->judge], NULL, window, 800 + key * 100, SDL_GetTicks64() - iter->time - chartOffset);
		}
	}
}

void Graphic::drawJudgeKey(JudgeVisible& judgeKeyVisible, Uint64 chartOffset) {
	for (int key = 0; key < 4; key++) {
		std::list<JudgeKeySound>::iterator iter = judgeKeyVisible.begin(key);
		for (; iter != judgeKeyVisible.end(key); iter++) {
			GPU_Blit(judgeKey[iter->judge], NULL, window, 800 + key * 100, SDL_GetTicks64() - iter->time - chartOffset);
		}
	}
}

void Graphic::drawJudge(int judge) {
	static int staticJudge = 6;
	staticJudge = judge != 6 ? judge : staticJudge;
	switch (staticJudge) {
	case 0:
		nfont[FONT_JUDGE].draw(window, 300, 100, SDL_Color{ 0, 255, 255, 255 }, "MARVELOUS");
		break;
	case 1:
		nfont[FONT_JUDGE].draw(window, 330, 100, SDL_Color{ 255, 255, 0, 255 }, "PERFECT");
		break;
	case 2:
		nfont[FONT_JUDGE].draw(window, 360, 100, SDL_Color{ 0, 255, 0, 255 }, "GREAT");
		break;
	case 3:
		nfont[FONT_JUDGE].draw(window, 375, 100, SDL_Color{ 0, 0, 255, 255 }, "GOOD");
		break;
	case 4:
		nfont[FONT_JUDGE].draw(window, 390, 100, SDL_Color{ 255, 0, 255, 255 }, "BAD");
		break;
	case 5:
		nfont[FONT_JUDGE].draw(window, 375, 100, SDL_Color{ 255, 0, 0, 255 }, "MISS");
		break;
	}
}

void Graphic::drawErrorMeter(std::list<JudgeErrorTime>::iterator begin, std::list<JudgeErrorTime>::iterator end, int lifeTime, float scale) {
	GPU_Blit(errorMeterMiddle, NULL, window, 450, 200);
	for (std::list<JudgeErrorTime>::iterator iter = begin; iter != end; iter++) {
		int alpha = 255 - (float)255 * (SDL_GetTicks64() - iter->time) / lifeTime;
		GPU_SetRGBA(errorMeter[iter->judge], 255, 255, 255, alpha >= 0 ? alpha : 0);
		GPU_Blit(errorMeter[iter->judge], NULL, window, 450 + iter->error * scale, 200);
	}
}

void Graphic::drawCombo(char* combo) {
	nfont[FONT_JUDGE].draw(window, 400, 220, SDL_Color{ 255, 255, 255, 255 }, combo);
}

void Graphic::drawText() {
	//nfont[FONT_REGULAR].draw(window, 700, 300, SDL_Color{ 255, 255, 255, 255 }, "Sound sample taken from:\nPEACE BREAKER\nArtist: Omnipotenx(xi)");
	nfont[FONT_REGULAR].draw(window, 700, 300, SDL_Color{ 255, 255, 255, 255 }, "Sound sample taken from:\n(In the description)");
}

void Graphic::drawGameType(int gameSelect) {
	const static char* gameType[3] = {"Train", "Dan", "Exit"};
	for (int idx = -7; idx <= 7; idx++) {
		nfont[FONT_REGULAR].draw(window, 100, (idx + 7) * 50, SDL_Color{ 255, 255, 255, 255 }, gameType[(idx + gameSelect + 3 * 3) % 3]);
	}
	nfont[FONT_REGULAR].draw(window, 60, 350, SDL_Color{ 255, 255, 255, 255 }, ">");
}

void Graphic::drawDan(std::vector<std::filesystem::path>& danDir, int danNum, int danSelect) {
	for (int idx = -7; idx <= 7; idx++) {
		nfont[FONT_REGULAR].draw(window, 100, (idx + 7) * 50, SDL_Color{ 255, 255, 255, 255 }, danDir[(idx + danSelect + danDir.size() * 7) % danDir.size()].string().c_str());
	}
	nfont[FONT_REGULAR].draw(window, 60, 350, SDL_Color{ 255, 255, 255, 255 }, ">");
}

void Graphic::drawBms(std::vector<std::filesystem::path>& bmsDir, int bmsSelect) {
	for (int idx = -7; idx <= 7; idx++) {
		nfont[FONT_REGULAR].draw(window, 100, (idx + 7) * 50, SDL_Color{ 255, 255, 255, 255 }, bmsDir[(idx + bmsSelect + bmsDir.size() * 7) % bmsDir.size()].string().c_str());
	}
	nfont[FONT_REGULAR].draw(window, 60, 350, SDL_Color{ 255, 255, 255, 255 }, ">");
}

void Graphic::drawBmsFile(std::vector<std::vector<std::filesystem::path>>& bmsFileDir, int bmsSelect, int bmsFileSelect) {
	for (int idx = -7; idx <= 7; idx++) {
		nfont[FONT_REGULAR].draw(window, 100, (idx + 7) * 50, SDL_Color{ 255, 255, 255, 255 }, bmsFileDir[bmsSelect][(idx + bmsFileSelect + bmsFileDir[bmsSelect].size() * 7) % bmsFileDir[bmsSelect].size()].string().c_str());
	}
	nfont[FONT_REGULAR].draw(window, 60, 350, SDL_Color{ 255, 255, 255, 255 }, ">");
}

void Graphic::drawDifficultySelect(PatternParameter& patternParameter, int difficultySelect) {
	char buffer[64];
	nfont[FONT_REGULAR].draw(window, 100, 350, SDL_Color{ 255, 255, 255, 255 }, "Start");
	nfont[FONT_REGULAR].draw(window, 100, 400, SDL_Color{ 255, 255, 255, 255 }, "+0.01");
	nfont[FONT_REGULAR].draw(window, 100, 450, SDL_Color{ 255, 255, 255, 255 }, "-0.01");
	sprintf(buffer, "%.2f", (float)patternParameter.rate / 100);
	nfont[FONT_REGULAR].draw(window, 200, 250, SDL_Color{ 255, 255, 255, 255 }, "Rate");
	nfont[FONT_REGULAR].draw(window, 200, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 250, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.jack / 100);
	nfont[FONT_REGULAR].draw(window, 300, 250, SDL_Color{ 255, 255, 255, 255 }, "Jack");
	nfont[FONT_REGULAR].draw(window, 300, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 350, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.jackLength / 100);
	nfont[FONT_REGULAR].draw(window, 400, 250, SDL_Color{ 255, 255, 255, 255 }, "Jack\nLength");
	nfont[FONT_REGULAR].draw(window, 400, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 450, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.speedTech / 100);
	nfont[FONT_REGULAR].draw(window, 500, 250, SDL_Color{ 255, 255, 255, 255 }, "Speed\nTech");
	nfont[FONT_REGULAR].draw(window, 500, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 550, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.jump / 100);
	nfont[FONT_REGULAR].draw(window, 600, 250, SDL_Color{ 255, 255, 255, 255 }, "Jump");
	nfont[FONT_REGULAR].draw(window, 600, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 650, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.hand / 100);
	nfont[FONT_REGULAR].draw(window, 700, 250, SDL_Color{ 255, 255, 255, 255 }, "Hand");
	nfont[FONT_REGULAR].draw(window, 700, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 750, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.quad / 100);
	nfont[FONT_REGULAR].draw(window, 800, 250, SDL_Color{ 255, 255, 255, 255 }, "Quad");
	nfont[FONT_REGULAR].draw(window, 800, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 850, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	sprintf(buffer, "%.2f", (float)patternParameter.tech / 100);
	nfont[FONT_REGULAR].draw(window, 900, 250, SDL_Color{ 255, 255, 255, 255 }, "Chord\nTech");
	nfont[FONT_REGULAR].draw(window, 900, 350, SDL_Color{ 255, 255, 255, 255 }, buffer);
	nfont[FONT_REGULAR].draw(window, 950, 400, SDL_Color{ 255, 255, 255, 255 }, "*");
	nfont[FONT_REGULAR].draw(window, 200 + difficultySelect * 100, 400, SDL_Color{ 255, 255, 255, 255 }, ">");
}

void Graphic::drawHp(int hp) {
	if (hp > 500) {
		unsigned char newColor = (unsigned char)(255 - ((float)1000 - hp) / 500 * 255);
		GPU_RectangleFilled(window, 0, 40, (float)hp / 5, 60, SDL_Color{ 255, 255, newColor, 255 });
	}
	else {
		unsigned char newColor = (unsigned char)(255 - ((float)500 - hp) / 500 * 255);
		GPU_RectangleFilled(window, 0, 40, (float)hp / 5, 60, SDL_Color{ 255, newColor, 0, 255 });
	}
}

void Graphic::drawResult(Score& score) {
	char scoreCount[32];
	sprintf(scoreCount, "ScoreV1: %.6f", score.getScoreV1Seg());
	nfont[FONT_REGULAR].draw(window, 50, 100, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "ScoreV2: %.6f", score.getScoreV2Seg());
	nfont[FONT_REGULAR].draw(window, 50, 150, SDL_Color{ 255, 255, 255, 255 }, scoreCount);

	sprintf(scoreCount, "%d", score.judgeCounterSeg[0]);
	nfont[FONT_REGULAR].draw(window, 50, 200, SDL_Color{ 0, 255, 255, 255 }, "MARVELOUS:");
	nfont[FONT_REGULAR].draw(window, 300, 200, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounterSeg[1]);
	nfont[FONT_REGULAR].draw(window, 50, 250, SDL_Color{ 255, 255, 0, 255 }, "PERFECT:");
	nfont[FONT_REGULAR].draw(window, 300, 250, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounterSeg[2]);
	nfont[FONT_REGULAR].draw(window, 50, 300, SDL_Color{ 0, 255, 0, 255 }, "GREAT:");
	nfont[FONT_REGULAR].draw(window, 300, 300, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounterSeg[3]);
	nfont[FONT_REGULAR].draw(window, 50, 350, SDL_Color{ 0, 0, 255, 255 }, "GOOD:");
	nfont[FONT_REGULAR].draw(window, 300, 350, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounterSeg[4]);
	nfont[FONT_REGULAR].draw(window, 50, 400, SDL_Color{ 255, 0, 255, 255 }, "BAD:");
	nfont[FONT_REGULAR].draw(window, 300, 400, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounterSeg[5]);
	nfont[FONT_REGULAR].draw(window, 50, 450, SDL_Color{ 255, 0, 0, 255 }, "MISS:");
	nfont[FONT_REGULAR].draw(window, 300, 450, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounterSeg[6]);
	nfont[FONT_REGULAR].draw(window, 50, 500, SDL_Color{ 255, 0, 0, 255 }, "EARLY MISS:");
	nfont[FONT_REGULAR].draw(window, 300, 500, SDL_Color{ 255, 255, 255, 255 }, scoreCount);

	sprintf(scoreCount, "%.2f ms", score.getAvgErrorSeg());
	nfont[FONT_REGULAR].draw(window, 50, 550, SDL_Color{ 255, 255, 255, 255 }, "Avg. Error:");
	nfont[FONT_REGULAR].draw(window, 300, 550, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%.2f", score.getVarianceSeg());
	nfont[FONT_REGULAR].draw(window, 50, 600, SDL_Color{ 255, 255, 255, 255 }, "Var.:");
	nfont[FONT_REGULAR].draw(window, 300, 600, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%.2f ms", score.getSDSeg());
	nfont[FONT_REGULAR].draw(window, 50, 650, SDL_Color{ 255, 255, 255, 255 }, "SD.:");
	nfont[FONT_REGULAR].draw(window, 300, 650, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
}

void Graphic::drawDanResult(Score& score, int danSeg) {
	char scoreCount[32];
	sprintf(scoreCount, "Segment: %d", danSeg + 1);
	nfont[FONT_REGULAR].draw(window, 650, 50, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "ScoreV1: %.6f", score.getScoreV1());
	nfont[FONT_REGULAR].draw(window, 650, 100, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "ScoreV2: %.6f", score.getScoreV2());
	nfont[FONT_REGULAR].draw(window, 650, 150, SDL_Color{ 255, 255, 255, 255 }, scoreCount);

	sprintf(scoreCount, "%d", score.judgeCounter[0]);
	nfont[FONT_REGULAR].draw(window, 650, 200, SDL_Color{ 0, 255, 255, 255 }, "MARVELOUS:");
	nfont[FONT_REGULAR].draw(window, 900, 200, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounter[1]);
	nfont[FONT_REGULAR].draw(window, 650, 250, SDL_Color{ 255, 255, 0, 255 }, "PERFECT:");
	nfont[FONT_REGULAR].draw(window, 900, 250, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounter[2]);
	nfont[FONT_REGULAR].draw(window, 650, 300, SDL_Color{ 0, 255, 0, 255 }, "GREAT:");
	nfont[FONT_REGULAR].draw(window, 900, 300, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounter[3]);
	nfont[FONT_REGULAR].draw(window, 650, 350, SDL_Color{ 0, 0, 255, 255 }, "GOOD:");
	nfont[FONT_REGULAR].draw(window, 900, 350, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounter[4]);
	nfont[FONT_REGULAR].draw(window, 650, 400, SDL_Color{ 255, 0, 255, 255 }, "BAD:");
	nfont[FONT_REGULAR].draw(window, 900, 400, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounter[5]);
	nfont[FONT_REGULAR].draw(window, 650, 450, SDL_Color{ 255, 0, 0, 255 }, "MISS:");
	nfont[FONT_REGULAR].draw(window, 900, 450, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%d", score.judgeCounter[6]);
	nfont[FONT_REGULAR].draw(window, 650, 500, SDL_Color{ 255, 0, 0, 255 }, "EARLY MISS:");
	nfont[FONT_REGULAR].draw(window, 900, 500, SDL_Color{ 255, 255, 255, 255 }, scoreCount);

	sprintf(scoreCount, "%.2f ms", score.getAvgError());
	nfont[FONT_REGULAR].draw(window, 650, 550, SDL_Color{ 255, 255, 255, 255 }, "Avg. Error:");
	nfont[FONT_REGULAR].draw(window, 900, 550, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%.2f", score.getVariance());
	nfont[FONT_REGULAR].draw(window, 650, 600, SDL_Color{ 255, 255, 255, 255 }, "Var.:");
	nfont[FONT_REGULAR].draw(window, 900, 600, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
	sprintf(scoreCount, "%.2f ms", score.getSD());
	nfont[FONT_REGULAR].draw(window, 650, 650, SDL_Color{ 255, 255, 255, 255 }, "SD.:");
	nfont[FONT_REGULAR].draw(window, 900, 650, SDL_Color{ 255, 255, 255, 255 }, scoreCount);
}

void Graphic::drawDebug(char* debugText[]) {
	for (int index = 0; debugText[index]; index++) {
		nfont[FONT_DEBUG].draw(window, 10, index * 40 + 100, SDL_Color{ 255, 255, 255, 255 }, debugText[index]);
	}
}
