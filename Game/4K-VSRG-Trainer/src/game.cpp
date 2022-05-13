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

	audio.loadSound(0, "sound_test/A_003.wav");	// Test
	audio.loadSound(1, "sound_test/C_003.wav");	// Test
	audio.loadSound(2, "sound_test/F.wav");	// Test
	// Key map init
	keyMap[0] = SDL_SCANCODE_X;
	keyMap[1] = SDL_SCANCODE_C;
	keyMap[2] = SDL_SCANCODE_COMMA;
	keyMap[3] = SDL_SCANCODE_PERIOD;
	// Fps init
	fps.init();
}

void Game::exit() {
	graphic.exit();
	audio.portAudioExit();
}

void Game::update() {
	graphic.clear();
	
	// Key
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	for (int key = 0; key < 4; key++) {
		if (keyboardState[keyMap[key]]) {
			if (!keyPressed[key]) {
				audio.playSound(key - key / 2);
				keyPressed[key] = true;
				keyLogger.push(key, SDL_GetTicks64());
			}
		}
		else {
			keyPressed[key] = false;
		}
	}
	keyLogger.update();

	graphic.drawReceipter();
	graphic.drawKeyPressed(keyPressed);
	graphic.drawNote();
	if (keyLogger.canShow()) {
		graphic.drawJudgeKey(keyLogger.replayInterval());
	}
	graphic.drawText();
	// Debug info
	char fpsBuffer[32] = {};
	sprintf(fpsBuffer, "FPS: %d", fps.get());
	char sizeBuffer[32] = {};
	sprintf(sizeBuffer, "Sound num: %d", audio.getCurrentSoundNum());
	char keyCountBufffer[32] = {};
	sprintf(keyCountBufffer, "Key pressed: %d", keyLogger.count());
	char* debugText[5] = { fpsBuffer, sizeBuffer, keyCountBufffer };

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
