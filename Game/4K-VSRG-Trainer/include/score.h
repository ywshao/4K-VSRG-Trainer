#pragma once

#include "note.h"

#define missMs 500

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
	void missJudger(double difficulty, ChartVisible& chartVisible, JudgeVisible& judgeNoteVisible);
};
