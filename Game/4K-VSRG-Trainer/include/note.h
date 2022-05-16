#pragma once

#include <SDL.h>
#include <list>
#include "audioTest.h"

struct KeySound {
	Uint64 time;
	int sound;
};

struct JudgeKeySound : KeySound {
	JudgeKeySound() {}
	JudgeKeySound(KeySound keySound, int judge) : judge(judge) {
		time = keySound.time;
		sound = keySound.sound;
	}
	int judge;
};

template <typename T, int size>
class KeyLogger {
protected:
	typename std::list<T> keyLog[size];
	typename std::list<T>::iterator keyBegin[size];
	typename std::list<T>::iterator keyEnd[size];
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

class JudgeKey : public KeyLogger<JudgeKeySound, 4> {
private:
	Uint64 offset = 0;
public:
};

class JudgeVisible : public KeyLogger<JudgeKeySound, 4> {
public:
	void update();
};

class Chart : public KeyLogger<KeySound, 4> {
private:
public:
};

class ChartVisible : public KeyLogger<KeySound, 5> {
private:
public:
	void update(Audio* audio);
};

class NoteLogger : public KeyLogger<Uint64, 4> {
private:
public:
};
