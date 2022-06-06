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

int Score::judgeScoreV1(double difficulty, Uint64 errorMs) {
	double errorAbs = difficulty * errorMs;
	if (errorAbs <= 22.5) {
		return 5;
	}
	else if (errorAbs <= 45) {
		return 4;
	}
	else if (errorAbs <= 90) {
		return 3;
	}
	else if (errorAbs <= 135) {
		return 2;
	}
	else if (errorAbs <= 180) {
		return 1;
	}
	else {
		return 0;
	}
}

double Score::judgeScoreV2(double difficulty, Uint64 errorMs) {
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

JudgeKeySound Score::judger(double difficulty, int key, ChartVisible& chartVisible, JudgeVisible& judgeNoteVisible, ErrorMeter& errorMeter, Uint64 chartOffset) {
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
		early = SDL_GetTicks64() < iter->time + chartOffset;
		errorMs = early ? iter->time + chartOffset - SDL_GetTicks64() : SDL_GetTicks64() - iter->time - chartOffset;
		judgeResult = judge(difficulty, errorMs);
		if (judgeResult <= 3) { // Better than good or judge window overlap and better than good
			updateScore(difficulty, early, errorMs, judgeResult);
			judgeKeySound[key] = {*iter,  judgeResult};
			judgeNoteVisible.add(key, judgeKeySound[key]);
			chartVisible.remove(key, iter);
			judgeKeySound[key].time = SDL_GetTicks64() - chartOffset;
			errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
			return judgeKeySound[key];
		}
		else if (judgeResult > 3 && judgeResult <= 5) { // Bad or miss
			if (!early) { // Prevent judge window overlap (Later (time) note > earlier (time) note)
				judgeNote = iter;
				checkNextJudge = true;
			}
			else if (judgeResult == 4) { // Early bad
				updateScore(difficulty, early, errorMs, judgeResult);
				judgeKeySound[key] = { *iter,  judgeResult };
				judgeNoteVisible.add(key, judgeKeySound[key]);
				chartVisible.remove(key, iter);
				judgeKeySound[key].time = SDL_GetTicks64() - chartOffset;
				errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
				return judgeKeySound[key];
			}
			else if (judgeResult == 5) { // Early miss (early poor)
				earlyMiss();
				judgeKeySound[key] = { *iter,  judgeResult };
				judgeNoteVisible.add(key, judgeKeySound[key]);
				judgeKeySound[key].time = SDL_GetTicks64() - chartOffset;
				errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
				return judgeKeySound[key];
			}
		}
		lastNote[key] = *iter;
	}
	// Does not have judge window overlap
	if (checkNextJudge) { // Return judge 3~5
		updateScore(difficulty, early, errorMs, judgeResult);
		judgeKeySound[key] = { *judgeNote,  judgeResult };
		judgeNoteVisible.add(key, { *judgeNote,  judgeResult });
		chartVisible.remove(key, judgeNote);
		judgeKeySound[key].time = SDL_GetTicks64() - chartOffset;
		errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
		return judgeKeySound[key];
	}
	else { // Return judge 4~5
		if (judgeResult != 6) {
			updateScore(difficulty, early, errorMs, judgeResult);
			judgeKeySound[key] = { lastNote[key], judgeResult };
			judgeNoteVisible.add(key, { *judgeNote,  judgeResult });
			chartVisible.remove(key, judgeNote);
			judgeKeySound[key].time = SDL_GetTicks64() - chartOffset;
			errorMeter.add(SDL_GetTicks64(), judgeResult, early ? -(int)errorMs : (int)errorMs);
			return judgeKeySound[key];
		}
		else { // Not in any judge window
			judgeKeySound[key] = { lastNote[key], 6} ;
			judgeKeySound[key].time = SDL_GetTicks64() - chartOffset;
			return judgeKeySound[key];
		}
	}
}

bool Score::missJudger(double difficulty, ChartVisible& chartVisible, JudgeVisible& judgeNoteVisible, Uint64 chartOffset) {
	bool flag = false;
	for (int key = 0; key < 4; key++) {
		for (std::list<KeySound>::iterator iter = chartVisible.begin(key); iter != chartVisible.end(key);) {
			if (SDL_GetTicks64() >= iter->time + chartOffset + lateMissMs) {
				miss();
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

void Score::clearSeg() {
	for (int judge = 0; judge <= 6; judge++) {
		judgeCounterSeg[judge] = 0;
	}
	scoreV1Seg = 0;
	scoreV2Seg = 0;
	judgedNoteCountSeg = 0;
	notMissCountSeg = 0;
	comboSeg = 0;
	meanSeg = 0;
	M2Seg = 0;
}

void Score::clearDan() {
	for (int judge = 0; judge <= 6; judge++) {
		judgeCounter[judge] = 0;
	}
	scoreV1 = 0;
	scoreV2 = 0;
	judgedNoteCount = 0;
	notMissCount = 0;
	combo = 0;
	mean = 0;
	M2 = 0;
}

void Score::updateScore(double difficulty, bool early, Uint64 errorMs, int judgeResult) {
	scoreV1 += judgeScoreV1(difficulty, errorMs);
	scoreV1Seg += judgeScoreV1(difficulty, errorMs);
	scoreV2 += judgeScoreV2(difficulty, errorMs);
	scoreV2Seg += judgeScoreV2(difficulty, errorMs);
	judgedNoteCount++;
	judgedNoteCountSeg++;
	judgeCounter[judgeResult]++;
	judgeCounterSeg[judgeResult]++;
	switch (judgeResult) {
	case 0:
		hp += 5;
		break;
	case 1:
		hp += 4;
		break;
	case 2:
		hp += 3;
		break;
	case 3:
		hp -= 10;
		break;
	case 4:
		hp -= 25;
		break;
	case 5:
		hp -= 50;
		break;
	}
	hp = hp > 1000 ? 1000 : hp;
	notMissCount++;
	notMissCountSeg++;
	judgeResult <= 2 ? combo++ : combo = 0;
	judgeResult <= 2 ? comboSeg++ : comboSeg = 0;

	// Welford's online algorithm
	int errorMsSigned = early ? -(int)errorMs : errorMs;
	double delta = errorMsSigned - mean;
	mean += delta / notMissCount;
	double delta2 = errorMsSigned - mean;
	M2 += delta * delta2;

	double deltaSeg = errorMsSigned - meanSeg;
	meanSeg += deltaSeg / notMissCountSeg;
	double delta2Seg = errorMsSigned - meanSeg;
	M2Seg += deltaSeg * delta2Seg;
}

void Score::earlyMiss() {
	judgeCounter[6]++;
	judgeCounterSeg[6]++;
	hp -= 10;
}

void Score::miss() {
	judgedNoteCount++;
	judgedNoteCountSeg++;
	judgeCounter[5]++;
	judgeCounterSeg[5]++;
	combo = 0;
	comboSeg = 0;
	hp -= 50;
}

double Score::getScoreV1() {
	return judgedNoteCount ? (double)20 * scoreV1 / judgedNoteCount : 0;
}

double Score::getScoreV2() {
	return judgedNoteCount ? (double)100 * scoreV2 / judgedNoteCount : 0;
}

double Score::getScoreV1Seg() {
	return judgedNoteCountSeg ? (double)20 * scoreV1Seg / judgedNoteCountSeg : 0;
}

double Score::getScoreV2Seg() {
	return judgedNoteCountSeg ? (double)100 * scoreV2Seg / judgedNoteCountSeg : 0;
}

double Score::getAvgError() {
	return mean;
}

double Score::getAvgErrorSeg() {
	return meanSeg;
}

double Score::getVariance() {
	return notMissCount ? M2 / notMissCount : 0;
}

double Score::getVarianceSeg() {
	return notMissCountSeg ? M2Seg / notMissCountSeg : 0;
}

double Score::getSD() {
	return sqrt(getVariance());
}

double Score::getSDSeg() {
	return sqrt(getVarianceSeg());
}

int Score::getCombo() {
	return combo;
}
