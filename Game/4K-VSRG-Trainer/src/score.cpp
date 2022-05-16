#include "score.h"

void ErrorMeter::setLifeTime(int time) {
	lifeTime = time;
}

int ErrorMeter::getLifeTime() {
	return lifeTime;
}

void ErrorMeter::setScale(int factor) {
	scale = factor;
}
float ErrorMeter::getScale() {
	return scale;
}

void ErrorMeter::add(Uint64 time, int judge, int error) {
	errorList.push_back({ time, judge, error });
}

void ErrorMeter::update() {
	for (std::list<JudgeErrorTime>::iterator iter = errorList.begin(); iter != errorList.end();) {
		if (SDL_GetTicks64() >= iter->time + lifeTime) {
			iter = errorList.erase(iter);
		}
		else {
			iter++;
		}
	}
}

std::list<JudgeErrorTime>::iterator ErrorMeter::begin() {
	return errorList.begin();
}

std::list<JudgeErrorTime>::iterator ErrorMeter::end() {
	return errorList.end();
}

double Score::scoreV1(double difficulty, Uint64 errorMs) {
	double errorAbs = difficulty * errorMs;
	if (errorAbs <= 22.5) {
		return 1;
	}
	else if (errorAbs <= 45) {
		return 0.8;
	}
	else if (errorAbs <= 90) {
		return 0.6;
	}
	else if (errorAbs <= 135) {
		return 0.4;
	}
	else if (errorAbs <= 180) {
		return 0.2;
	}
	else {
		return 0;
	}
}

double Score::scoreV2(double difficulty, Uint64 errorMs) {
	double errorAbs = difficulty * errorMs;
	if (errorAbs <= 2) { // (0, 1)
		return 1;
	}
	else if (errorAbs <= 60) { // (60, 0.784)
		double x = errorAbs / 100;
		return 1 - x * x * x;
	}
	else if (errorAbs <= 90) { // (90, 0.541)
		double x = errorAbs - 120;
		return 0.00009 * x * x + 0.46;
	}
	else if (errorAbs <= 180) { // (0, 0.055)
		return 1.027 - 0.0054 * errorAbs;
	}
	else {
		return 0;
	}
}

int Score::judge(double difficulty, Uint64 errorMs) {
	double errorAbs = difficulty * errorMs;
	if (errorAbs <= 22.5) {
		return 0;
	}
	else if (errorAbs <= 45) {
		return 1;
	}
	else if (errorAbs <= 90) {
		return 2;
	}
	else if (errorAbs <= 135) {
		return 3;
	}
	else if (errorAbs <= 180) {
		return 4;
	}
	else if (errorAbs <= earlyMissMs) {
		return 5;
	}
	else {
		return 6;
	}
}

JudgeKeySound Score::judger(double difficulty, int key, ChartVisible& chartVisible, JudgeVisible& judgeNoteVisible, ErrorMeter& errorMeter) {
	Uint64 errorMs;
	bool early;
	bool checkNextJudge = false;
	int judgeResult = 6; // Default judge
	static JudgeKeySound judgeKeySound[4]; // Preserve the last note sound
	std::list<KeySound>::iterator judgeNote;
	static KeySound lastNote[4]; // Preserve the last note sound
	for (int k = 0; k < 4; k++) {
		judgeKeySound[k].judge = 6; // Default judge
	}
	for (std::list<KeySound>::iterator iter = judgeNote = chartVisible.begin(key); iter != chartVisible.end(key); iter++) {
		early = SDL_GetTicks64() < iter->time;
		errorMs = early ? iter->time - SDL_GetTicks64() : SDL_GetTicks64() - iter->time;
		judgeResult = judge(difficulty, errorMs);
		if (judgeResult <= 3) { // Judge window overlap
			judgeKeySound[key] = {*iter,  judgeResult};
			judgeNoteVisible.add(key, judgeKeySound[key]);
			chartVisible.remove(key, iter);
			judgeKeySound[key].time = SDL_GetTicks64();
			errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
			return judgeKeySound[key];
		}
		else if (judgeResult > 3 && judgeResult <= 5) {
			if (!early) {
				judgeNote = iter;
				checkNextJudge = true;
			}
			else if (judgeResult == 4) {
				judgeKeySound[key] = { *iter,  judgeResult };
				judgeNoteVisible.add(key, judgeKeySound[key]);
				chartVisible.remove(key, iter);
				judgeKeySound[key].time = SDL_GetTicks64();
				errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
				return judgeKeySound[key];
			}
			else if (judgeResult == 5) {
				judgeKeySound[key] = { *iter,  judgeResult };
				judgeNoteVisible.add(key, judgeKeySound[key]);
				judgeKeySound[key].time = SDL_GetTicks64();
				errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
				return judgeKeySound[key];
			}
		}
		lastNote[key] = *iter;
	}
	// Does not have judge window overlap
	if (checkNextJudge) { // Return judge 3~5
		judgeKeySound[key] = { *judgeNote,  judgeResult };
		judgeNoteVisible.add(key, { *judgeNote,  judgeResult });
		chartVisible.remove(key, judgeNote);
		judgeKeySound[key].time = SDL_GetTicks64();
		errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
		return judgeKeySound[key];
	}
	else { // Return judge 0~2
		if (judgeResult != 6) {
			judgeKeySound[key] = { lastNote[key], judgeResult };
			judgeKeySound[key].time = SDL_GetTicks64();
			errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
			return judgeKeySound[key];
		}
		else { // Not in any judge window
			//judgeKeySound = { lastNote[key], 6 };
			judgeKeySound[key].time = SDL_GetTicks64();
			return judgeKeySound[key];
		}
	}
}

bool Score::missJudger(double difficulty, ChartVisible& chartVisible, JudgeVisible& judgeNoteVisible) {
	bool flag = false;
	for (int key = 0; key < 4; key++) {
		for (std::list<KeySound>::iterator iter = chartVisible.begin(key); iter != chartVisible.end(key);) {
			if (SDL_GetTicks64() >= iter->time + lateMissMs) {
				judgeNoteVisible.add(key, { *iter,  5 });
				iter = chartVisible.remove(key, iter);
				flag = true;
			}
			else {
				iter++;
			}
		}
	}
	return flag;
}
