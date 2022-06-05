#pragma once

#include <SDL.h>
#include <list>
#include <queue>
#include "audioTest.h"

#define keyNum 4
#define addNoteMs 1000
#define removeNoteMs 500
#define judgeVisibleMs 2000

struct KeySound {
	Uint64 time;
	int sound;
};

struct KeySoundCmp {
	bool operator()(KeySound a, KeySound b) {
		return a.time > b.time;
	}
};

struct JudgeKeySound : KeySound {
	JudgeKeySound() {}
	JudgeKeySound(KeySound keySound, int judge) : judge(judge) {
		time = keySound.time;
		sound = keySound.sound;
	}
	int judge;
};

template <typename T>
class KeyLogger {
protected:
	typename std::list<T> keyLog[keyNum];
	typename std::list<T>::iterator keyBegin[keyNum];
	typename std::list<T>::iterator keyEnd[keyNum];
public:
	void clear();
	void add(int key, T data);
	typename std::list<T>::iterator remove(int key, typename std::list<T>::iterator iter);
	int count();
	int count(int key);
	typename std::list<T>::iterator* begin();
	typename std::list<T>::iterator* end();
	typename std::list<T>::iterator begin(int key);
	typename std::list<T>::iterator end(int key);
};

class KeyLoggerBgm : public KeyLogger<KeySound> {
public:
	std::priority_queue<KeySound, std::vector<KeySound>, KeySoundCmp> bgm;
	void clear();
};

class JudgeKey : public KeyLogger<JudgeKeySound> {
private:
public:
};

class JudgeVisible : public KeyLogger<JudgeKeySound> {
private:
public:
	void update(Uint64 chartOffset);
};

class Chart : public KeyLoggerBgm {
private:
public:
	void add(int key, KeySound data);
};

class ChartVisible : public KeyLoggerBgm {
private:
	std::list<KeySound>::iterator keyToLoad[keyNum];
public:
	void init(Chart* chart);
	void update(Audio* audio, Chart* chart, Uint64 chartOffset);
};

class NoteLogger : public KeyLogger<Uint64> {
private:
public:
};
