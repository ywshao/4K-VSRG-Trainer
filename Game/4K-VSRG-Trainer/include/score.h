#pragma once

#include "note.h"

// Early than this will not count
#define earlyMissMs 500
// Early than this will be counted
// Later than this will clear the note and be counted as miss
#define lateMissMs 200

struct JudgeErrorTime {
	Uint64 time;
	int judge;
	int error;
};

class ErrorMeter {
private:
	std::list<JudgeErrorTime> errorList;
	int lifeTime;
	float scale;
public:
	void setLifeTime(int time);
	int getLifeTime();
	void setScale(int factor);
	float getScale();
	void add(Uint64 time, int judge, int error);
	void update();
	std::list<JudgeErrorTime>::iterator begin();
	std::list<JudgeErrorTime>::iterator end();
};

class Score {
private:
	// Stepmania & Etterna interval
	double scoreV1(double difficulty, Uint64 errorMs);
	// Etterna Wife3 style interval
	double scoreV2(double difficulty, Uint64 errorMs);
	//
	int judge(double difficulty, Uint64 errorMs);
	//
	double score;
public:
	JudgeKeySound judger(double difficulty, int key, ChartVisible &chartVisible, JudgeVisible& judgeNoteVisible, ErrorMeter& errorMeter);
	bool missJudger(double difficulty, ChartVisible& chartVisible, JudgeVisible& judgeNoteVisible);
};
