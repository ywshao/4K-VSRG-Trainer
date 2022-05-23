#include "game.h"

void Game::init() {
	// Config init
	mINI::INIFile iniFileGlobal("config.ini");
	mINI::INIStructure iniGlobal;
	iniFileGlobal.read(iniGlobal);
	std::string textPath(std::string("texture/") + iniGlobal["Texture"]["directory"] + std::string("/"));
	mINI::INIFile iniFileTexture(textPath + std::string("config.ini"));
	mINI::INIStructure iniTexture;
	iniFileTexture.read(iniTexture);
	// Graphic init
	graphic.init(textPath, iniTexture);
	// Audio init
	audio.portAudioInit(std::stoi(iniGlobal["Audio"]["device"]));
	/*
	audio.loadSound(0, "sound_test/A_003.wav");	// Test
	audio.loadSound(1, "sound_test/C_003.wav");	// Test
	audio.loadSound(2, "sound_test/F.wav");	// Test

	audio.loadSound(0x4f, "sound_test/pi2c.wav");	// Test
	audio.loadSound(0x51, "sound_test/pi2e.wav");	// Test
	audio.loadSound(0x52, "sound_test/pi2f.wav");	// Test
	audio.loadSound(0x53, "sound_test/pi2g.wav");	// Test
	audio.loadSound(0x54, "sound_test/pi2a.wav");	// Test
	audio.loadSound(0x55, "sound_test/pi3c.wav");	// Test
	audio.loadSound(0x56, "sound_test/pi3#c.wav");	// Test
	audio.loadSound(0x57, "sound_test/pi3d.wav");	// Test
	audio.loadSound(0x58, "sound_test/pi3e.wav");	// Test
	audio.loadSound(0x59, "sound_test/pi3f.wav");	// Test
	audio.loadSound(0x5a, "sound_test/pi3g.wav");	// Test
	audio.loadSound(0x5b, "sound_test/pi3a.wav");	// Test
	audio.loadSound(0x5d, "sound_test/pi4c.wav");	// Test
	audio.loadSound(0x5e, "sound_test/pi4#c.wav");	// Test
	audio.loadSound(0x5f, "sound_test/pi4d.wav");	// Test
	audio.loadSound(0x60, "sound_test/pi4e.wav");	// Test
	audio.loadSound(0x61, "sound_test/pi4f.wav");	// Test
	audio.loadSound(0x62, "sound_test/pi4g.wav");	// Test
	audio.loadSound(0x63, "sound_test/pi4a.wav");	// Test
	audio.loadSound(0x64, "sound_test/pi4#a.wav");	// Test
	audio.loadSound(0x65, "sound_test/pi4b.wav");	// Test
	audio.loadSound(0x66, "sound_test/pi5c.wav");	// Test
	audio.loadSound(0x67, "sound_test/pi5#c.wav");	// Test
	audio.loadSound(0x69, "sound_test/pi5e.wav");	// Test
	audio.loadSound(0x6a, "sound_test/pi5f.wav");	// Test
	audio.loadSound(0x6b, "sound_test/pi5g.wav");	// Test
	audio.loadSound(0x6c, "sound_test/pi5a.wav");	// Test
	audio.loadSound(0x9e, "sound_test/bpi3.wav");	// Test*/
	// Key map init
	keyMap[0] = SDL_SCANCODE_X;
	keyMap[1] = SDL_SCANCODE_C;
	keyMap[2] = SDL_SCANCODE_COMMA;
	keyMap[3] = SDL_SCANCODE_PERIOD;
	// Fps init
	fps.init();
	// Error meter
	errorMeter.setLifeTime(stoi(iniGlobal["ErrorMeter"]["liveTime"]));
	errorMeter.setScale(stof(iniGlobal["ErrorMeter"]["scale"]));
	scrollSpeed = stof(iniGlobal["ScrollSpeed"]["speed"]);
	// Test
	//bmsParser.parseFile("sound_test/[—L‰êàY]Defeatawakenbattleship/SPI.bms");
	bmsParser.parseFile("sound_test/Altale/output.bm4");
	double offset = 0;
	double currentBpm = bmsParser.bpm;
	for (int bar = 0; bar <= bmsParser.barMax; bar++) {
		double barLength;
		if (!bmsParser.bpmInt[bar].empty()) { // Test
			currentBpm = bmsParser.bpmInt[bar][0];
		}
		barLength = (double)4 / currentBpm * 60000;
		barLength = bmsParser.timeSignature[bar] ? barLength * bmsParser.timeSignature[bar] : barLength;
		for (int key = 0; key < 8; key++) {
			int sizeNote = bmsParser.note[bar][key].size();
			double keyLength = barLength / sizeNote;
			double keyOffset = offset;
			for (int note = 0; note < sizeNote; note++) {
				if (bmsParser.note[bar][key][note]) {
					chart.add(key < 4 ? key : 4, { (Uint64)keyOffset, bmsParser.note[bar][key][note] });
				}
				keyOffset += keyLength;
			}
		}
		int sizeIdx = bmsParser.bgm[bar].size();
		for (int idx = 0; idx < sizeIdx; idx++) {
			int sizeBgm = bmsParser.bgm[bar][idx].size();
			double keyLength = barLength / sizeBgm;
			double keyOffset = offset;
			for (int bgm = 0; bgm < sizeBgm; bgm++) {
				if (bmsParser.bgm[bar][idx][bgm]) {
					chart.add(4, { (Uint64)keyOffset, bmsParser.bgm[bar][idx][bgm] });
				}
				keyOffset += keyLength;
			}
		}
		offset += barLength;
	}
	for (int wav = 0; wav < 1536; wav++) {
		if (!bmsParser.wav[wav].empty()) {
			//char fileName[100] = "sound_test/[—L‰êàY]Defeatawakenbattleship/";
			char fileName[100] = "sound_test/Altale/";
			strcat(fileName, bmsParser.wav[wav].c_str());
			audio.loadSound(wav, fileName);
			//printf("%s\n", fileName);
		}
		else {
			audio.loadSound(wav, "0");
		}
	}
	//chartOffset = 16000;
	chartOffset = 25000;
	chartVisible.init(&chart);
}

void Game::exit() {
	graphic.exit();
	audio.portAudioExit();
}

void Game::update() {
	graphic.clear();

	// Key
	static JudgeKeySound judgeKeySound({ 0, 0 }, 6);
	if (score.missJudger(1/*Test*/, chartVisible, judgeNoteVisible, chartOffset)) {
		judgeKeySound.judge = 5;
	}
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	for (int key = 0; key < 4; key++) {
		if (keyboardState[keyMap[key]]) {
			if (!keyPressed[key]) {
				judgeKeySound = score.judger(1/*Test*/, key, chartVisible, judgeNoteVisible, errorMeter, chartOffset);
				if (judgeKeySound.sound) {
					audio.playSound(judgeKeySound.sound);
				}
				judgeKeyVisible.add(key, judgeKeySound);
				judgeKey.add(key, judgeKeySound);
				keyPressed[key] = true;
			}
		}
		else {
			keyPressed[key] = false;
		}
	}
	judgeNoteVisible.update(chartOffset);
	judgeKeyVisible.update(chartOffset);
	chartVisible.update(&audio, &chart, chartOffset);
	errorMeter.update();

	graphic.drawReceipter();
	graphic.drawKeyPressed(keyPressed);
	graphic.drawNote(chartVisible, scrollSpeed, chartOffset);
	graphic.drawJudgeNote(judgeNoteVisible, chartOffset);
	graphic.drawJudgeKey(judgeKeyVisible, chartOffset);
	graphic.drawJudge(judgeKeySound.judge);
	graphic.drawErrorMeter(errorMeter.begin(), errorMeter.end(), errorMeter.getLifeTime(), errorMeter.getScale());
	char combo[32];
	sprintf(combo, "%d", score.getCombo());
	graphic.drawCombo(combo);
	graphic.drawText();
	// Debug info
	char fpsBuffer[32] = {};
	sprintf(fpsBuffer, "FPS: %d", fps.get());
	char sizeBuffer[32] = {};
	sprintf(sizeBuffer, "Sound num: %d", audio.getCurrentSoundNum());
	char keyCountBufffer[32] = {};
	sprintf(keyCountBufffer, "Key pressed: %d", judgeKey.count());
	char chartVisibleBufffer[32] = {};
	sprintf(chartVisibleBufffer, "Visible note: %d", chartVisible.count());
	char chartBufffer[32] = {};
	sprintf(chartBufffer, "Chart note: %d", chart.count());
	char scoreV1Bufffer[32] = {};
	sprintf(scoreV1Bufffer, "ScoreV1: %.4f", score.getScoreV1());
	char scoreV2Bufffer[32] = {};
	sprintf(scoreV2Bufffer, "ScoreV2: %.4f", score.getScoreV2());
	char avgErrorBufffer[32] = {};
	sprintf(avgErrorBufffer, "AvgError: %.4f", score.getAvgError());
	char varianceBufffer[32] = {};
	sprintf(varianceBufffer, "Variance: %.4f", score.getVariance());
	char SDBufffer[32] = {};
	sprintf(SDBufffer, "SD: %.4f", score.getSD());
	char* debugText[12] = { fpsBuffer,
		sizeBuffer,
		keyCountBufffer,
		chartVisibleBufffer,
		chartBufffer,
		scoreV1Bufffer,
		scoreV2Bufffer,
		avgErrorBufffer,
		varianceBufffer,
		SDBufffer
	};

	graphic.drawDebug(debugText);
	graphic.draw();
	if (fps.check()) {
		fps.update();
	}
}

void Fps::init() {
	tick = SDL_GetTicks64();
	frameCounter = 0;
	fps = 0;
}

bool Fps::check() {
	frameCounter++;
	return SDL_GetTicks64() - tick >= 1000;
}

void Fps::update() {
	fps = frameCounter;
	frameCounter = 0;
	tick = SDL_GetTicks64();
}

int Fps::get() {
	return fps;
}
