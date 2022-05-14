#pragma once

#include <SDL.h>
#include <list>

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

template <typename T>
class KeyLogger {
protected:
	typename std::list<T> keyLog[4];
	typename std::list<T>::iterator keyBegin[4];
	typename std::list<T>::iterator keyEnd[4];
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

class JudgeKey : public KeyLogger<JudgeKeySound> {
private:
	Uint64 offset = 0;
public:
};

class JudgeVisible : public KeyLogger<JudgeKeySound> {
public:
	void update();
};

class Chart : public KeyLogger<KeySound> {
private:
public:
};

class ChartVisible : public KeyLogger<KeySound> {
private:
public:
	void update();
};

class NoteLogger : public KeyLogger<Uint64> {
private:
public:
};
