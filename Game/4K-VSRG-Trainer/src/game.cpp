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
	audio.loadSound(0x9e, "sound_test/bpi3.wav");	// Test
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
	Uint64 offset = 6000;							// Test
	for (Uint64 a = 0, b = 2, c = 3; a < 32; a += 4, b += 4, c += 4) {		// Test
		chart.add(0, { a * 100 + offset, 0 });		// Test
		chart.add(3, { b * 100 + offset, 2 });		// Test
		chart.add(2, { b * 100 + offset, 1 });		// Test
		chart.add(1, { c * 100 + offset, 1 });		// Test
		chartVisible.add(0, { a * 100 + offset, 0 });	// Test
		chartVisible.add(3, { b * 100 + offset, 2 });	// Test
		chartVisible.add(2, { b * 100 + offset, 1 });	// Test
		chartVisible.add(1, { c * 100 + offset, 1 });	// Test
	}												// Test
	chart.add(0, { 32 * 100 + offset, 0 });			// Test
	chartVisible.add(0, { 32 * 100 + offset, 0 });	// Test

	for (Uint64 a = 48, b = 50, c = 51; a < 64; a += 4, b += 4, c += 4) {	// Test
		chart.add(1, { a * 100 + offset, 0 });		// Test
		chart.add(0, { b * 100 + offset, 2 });		// Test
		chart.add(3, { b * 100 + offset, 1 });		// Test
		chart.add(3, { c * 100 + offset, 1 });		// Test
		chartVisible.add(1, { a * 100 + offset, 0 });	// Test
		chartVisible.add(0, { b * 100 + offset, 2 });	// Test
		chartVisible.add(3, { b * 100 + offset, 1 });	// Test
		chartVisible.add(3, { c * 100 + offset, 1 });	// Test
	}												// Test
	for (Uint64 a = 64, b = 66, c = 67; a < 80; a += 4, b += 4, c += 4) {	// Test
		chart.add(1, { a * 100 + offset, 0 });		// Test
		chart.add(0, { b * 100 + offset, 2 });		// Test
		chart.add(2, { b * 100 + offset, 1 });		// Test
		chart.add(2, { c * 100 + offset, 1 });		// Test
		chartVisible.add(1, { a * 100 + offset, 0 });	// Test
		chartVisible.add(0, { b * 100 + offset, 2 });	// Test
		chartVisible.add(2, { b * 100 + offset, 1 });	// Test
		chartVisible.add(2, { c * 100 + offset, 1 });	// Test
	}												// Test
	chart.add(1, { 80 * 100 + offset, 0 });			// Test
	chartVisible.add(1, { 80 * 100 + offset, 0 });	// Test

	Uint64 dist = 84;								// Test
	Uint64 dist2 = 56;
	offset = 17000;									// Test
	for (int a = 0; a < 6; a += 3) {
		chartVisible.add(2, { offset += dist, 0x63 });	// Test
		chartVisible.add(1, { offset += dist, 0x61 });	// Test
		chartVisible.add(0, { offset += dist, 0x5f });	// Test
		chartVisible.add(2, { offset += dist, 0x63 });	// Test
		chartVisible.add(1, { offset += dist, 0x61 });	// Test
		chartVisible.add(0, { offset += dist, 0x5f });	// Test
		chartVisible.add(2, { offset += dist, 0x63 });	// Test
		chartVisible.add(1, { offset += dist, 0x61 });	// Test
		chartVisible.add(0, { offset += dist, 0x5f });	// Test
		chartVisible.add(2, { offset += dist, 0x63 });	// Test
		chartVisible.add(1, { offset += dist, 0x61 });	// Test
		chartVisible.add(0, { offset += dist, 0x5f });	// Test
		chartVisible.add(2, { offset += dist, 0x63 });	// Test
		chartVisible.add(0, { offset += dist, 0x5f });	// Test
		chartVisible.add(3, { offset += dist, 0x64 });	// Test
		chartVisible.add(0, { offset += dist, 0x5f });	// Test

		chartVisible.add(1, { offset += dist, 0x66 });	// Test
		chartVisible.add(2, { offset += dist, 0x61 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test
		chartVisible.add(1, { offset += dist, 0x66 });	// Test
		chartVisible.add(2, { offset += dist, 0x61 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test
		chartVisible.add(1, { offset += dist, 0x66 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test
		chartVisible.add(0, { offset += dist, 0x64 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test
		chartVisible.add(1, { offset += dist, 0x63 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test
		chartVisible.add(2, { offset += dist, 0x61 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test
		chartVisible.add(1, { offset += dist, 0x62 });	// Test
		chartVisible.add(3, { offset += dist, 0x5f });	// Test

		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(1, { offset += dist, 0x5d });	// Test
		chartVisible.add(2, { offset += dist, 0x5a });	// Test
		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(1, { offset += dist, 0x5d });	// Test
		chartVisible.add(2, { offset += dist, 0x5a });	// Test
		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(2, { offset += dist, 0x5a });	// Test
		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(2, { offset += dist, 0x5a });	// Test
		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(1, { offset += dist, 0x62 });	// Test
		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(2, { offset += dist, 0x65 });	// Test
		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(3, { offset += dist, 0x66 });	// Test

		chartVisible.add(0, { offset += dist, 0x69 });	// Test
		chartVisible.add(1, { offset += dist, 0x66 });	// Test
		chartVisible.add(2, { offset += dist, 0x62 });	// Test
		chartVisible.add(0, { offset += dist, 0x69 });	// Test
		chartVisible.add(1, { offset += dist, 0x66 });	// Test
		chartVisible.add(2, { offset += dist, 0x62 });	// Test
		chartVisible.add(0, { offset += dist, 0x69 });	// Test
		chartVisible.add(2, { offset += dist, 0x62 });	// Test
		chartVisible.add(3, { offset += dist, 0x6b });	// Test
		chartVisible.add(2, { offset += dist, 0x66 });	// Test
		chartVisible.add(1, { offset += dist, 0x62 });	// Test
		chartVisible.add(3, { offset += dist, 0x6b });	// Test
		chartVisible.add(2, { offset += dist, 0x66 });	// Test
		chartVisible.add(1, { offset += dist, 0x62 });	// Test
		chartVisible.add(3, { offset += dist, 0x6b });	// Test
		chartVisible.add(1, { offset += dist, 0x62 });	// Test

		chartVisible.add(0, { offset += dist, 0x60 });	// Test
		chartVisible.add(2, { offset, 0x62 });			// Test
		chartVisible.add(3, { offset, 0x66 });			// Test
		chartVisible.add(1, { offset += dist, 0x4f });	// Test
		chartVisible.add(2, { offset += dist, 0x51 });	// Test
		chartVisible.add(3, { offset += dist, 0x53 });	// Test
		chartVisible.add(1, { offset += dist, 0x55 });	// Test
		chartVisible.add(2, { offset += dist, 0x58 });	// Test
		chartVisible.add(0, { offset += dist, 0x5a });	// Test
		chartVisible.add(1, { offset += dist, 0x5d });	// Test

		chartVisible.add(3, { offset += dist, 0x66 });	// Test
		chartVisible.add(2, { offset += dist2, 0x62 });	// Test
		chartVisible.add(1, { offset += dist2, 0x60 });	// Test
		chartVisible.add(2, { offset += dist2, 0x5d });	// Test
		chartVisible.add(1, { offset += dist2, 0x5a });	// Test
		chartVisible.add(0, { offset += dist2, 0x58 });	// Test
		chartVisible.add(3, { offset += dist2, 0x6a });	// Test
		chartVisible.add(2, { offset += dist, 0x66 });	// Test
		chartVisible.add(1, { offset += dist, 0x63 });	// Test
		chartVisible.add(0, { offset += dist, 0x61 });	// Test

		chartVisible.add(1, { offset += dist, 0x5e });	// Test
		chartVisible.add(3, { offset, 0x67 });			// Test
		chartVisible.add(2, { offset += dist, 0x56 });	// Test
		chartVisible.add(1, { offset += dist, 0x58 });	// Test
		chartVisible.add(0, { offset += dist, 0x5a });	// Test
		chartVisible.add(1, { offset += dist, 0x5e });	// Test
		chartVisible.add(2, { offset += dist, 0x60 });	// Test
		chartVisible.add(3, { offset += dist, 0x62 });	// Test
		chartVisible.add(1, { offset += dist, 0x5e });	// Test
		chartVisible.add(0, { offset += dist, 0x62 });	// Test
		chartVisible.add(3, { offset, 0x6b });			// Test
		chartVisible.add(2, { offset += dist, 0x58 });	// Test
		chartVisible.add(1, { offset += dist, 0x5a });	// Test
		chartVisible.add(0, { offset += dist, 0x5e });	// Test
		chartVisible.add(2, { offset += dist, 0x60 });	// Test
		chartVisible.add(3, { offset, 0x69 });			// Test
		chartVisible.add(0, { offset += dist, 0x5e });	// Test
		chartVisible.add(1, { offset += dist, 0x5a });	// Test
		chartVisible.add(2, { offset += dist, 0x58 });	// Test

		chartVisible.add(0, { offset += dist, 0x61 });	// Test
		chartVisible.add(1, { offset, 0x6a });			// Test
		chartVisible.add(3, { offset += dist, 0x52 });	// Test
		chartVisible.add(2, { offset += dist, 0x54 });	// Test
		chartVisible.add(1, { offset += dist, 0x57 });	// Test
		chartVisible.add(0, { offset += dist, 0x59 });	// Test
		chartVisible.add(1, { offset += dist, 0x5b });	// Test
		chartVisible.add(2, { offset += dist, 0x5f });	// Test
		chartVisible.add(3, { offset += dist, 0x61 });	// Test
		chartVisible.add(0, { offset += dist, 0x62 });	// Test
		chartVisible.add(2, { offset, 0x6b });			// Test
		chartVisible.add(1, { offset += dist, 0x54 });	// Test
		chartVisible.add(3, { offset += dist, 0x57 });	// Test
		chartVisible.add(2, { offset += dist, 0x59 });	// Test
		chartVisible.add(0, { offset += dist, 0x5b });	// Test
		chartVisible.add(1, { offset += dist, 0x5f });	// Test
		chartVisible.add(3, { offset += dist, 0x61 });	// Test
		chartVisible.add(2, { offset += dist, 0x63 });	// Test

		chartVisible.add(0, { offset += dist, 0x63 });	// Test
		chartVisible.add(3, { offset, 0x6c });			// Test
		chartVisible.add(2, { offset += dist, 0x5f });	// Test
		chartVisible.add(1, { offset += dist, 0x5b });	// Test
		chartVisible.add(0, { offset += dist, 0x59 });	// Test
		chartVisible.add(3, { offset += dist, 0x54 });	// Test
		chartVisible.add(0, { offset += dist, 0x57 });	// Test
		chartVisible.add(1, { offset += dist, 0x59 });	// Test
		chartVisible.add(2, { offset += dist, 0x5b });	// Test
		chartVisible.add(0, { offset += dist, 0x5d });	// Test
		chartVisible.add(1, { offset, 0x66 });			// Test
		chartVisible.add(3, { offset += dist, 0x55 });	// Test
		chartVisible.add(0, { offset += dist, 0x58 });	// Test
		chartVisible.add(2, { offset += dist, 0x5a });	// Test
		chartVisible.add(3, { offset += dist, 0x58 });	// Test
		chartVisible.add(0, { offset += dist, 0x5a });	// Test
		chartVisible.add(1, { offset += dist, 0x5d });	// Test
		chartVisible.add(3, { offset += dist, 0x60 });	// Test
	}
	chartVisible.add(2, { offset += dist, 0x9e });		// Test

}

void Game::exit() {
	graphic.exit();
	audio.portAudioExit();
}

void Game::update() {
	graphic.clear();

	score.missJudger(1/*Test*/, chartVisible, judgeNoteVisible);
	// Key
	static JudgeKeySound judgeKeySound({ 0, 0 }, 6);
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	for (int key = 0; key < 4; key++) {
		if (keyboardState[keyMap[key]]) {
			if (!keyPressed[key]) {
				judgeKeySound = score.judger(1/*Test*/, key, chartVisible, judgeNoteVisible, errorMeter);
				audio.playSound(judgeKeySound.sound);
				judgeKeyVisible.add(key, judgeKeySound);
				keyPressed[key] = true;
			}
		}
		else {
			keyPressed[key] = false;
		}
	}
	judgeNoteVisible.update();
	judgeKeyVisible.update();
	chartVisible.update();
	errorMeter.update();

	graphic.drawReceipter();
	graphic.drawKeyPressed(keyPressed);
	graphic.drawNote(chartVisible.begin(), chartVisible.end(), scrollSpeed);
	graphic.drawJudgeNote(judgeNoteVisible.begin(), judgeNoteVisible.end());
	graphic.drawJudgeKey(judgeKeyVisible.begin(), judgeKeyVisible.end());
	graphic.drawJudge(judgeKeySound.judge);
	graphic.drawErrorMeter(errorMeter.begin(), errorMeter.end(), errorMeter.getLifeTime(), errorMeter.getScale());
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
	char* debugText[5] = { fpsBuffer, sizeBuffer, keyCountBufffer, chartVisibleBufffer };

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
