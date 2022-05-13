#pragma once

#include <SDL.h>
#include <list>

struct NoteTime {
	int key;
	Uint64 time;
};

class KeyLogger {
private:
	std::list<NoteTime> keyLog;
	std::list<NoteTime>::iterator keyVisible;
	bool emptyInterval;
public:
	void clear();
	void push(int key, Uint64 time);
	int count();
	void update();
	bool canShow();
	std::pair<std::list<NoteTime>::iterator, std::list<NoteTime>::iterator> replayInterval();
};
