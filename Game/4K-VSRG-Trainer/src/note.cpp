#include "note.h"

void KeyLogger::clear() {
	keyLog.clear();
}

void KeyLogger::push(int key, Uint64 time) {
	keyLog.push_back({ key, time });
	if (keyLog.size() == 1) {
		keyVisible = keyLog.begin();
	}
}

int KeyLogger::count() {
	return keyLog.size();
}

void KeyLogger::update() {
	if (!keyLog.empty()) {
		std::list<NoteTime>::iterator updated = keyVisible;
		while (updated->time + 2000 < SDL_GetTicks64()) {
			keyVisible = updated;
			if (++updated == keyLog.end()) {
				emptyInterval = true;
				return;
			}
		}
		emptyInterval = false;
		return;
	}
	emptyInterval = true;
}

bool KeyLogger::canShow() {
	return !emptyInterval;
}

std::pair<std::list<NoteTime>::iterator, std::list<NoteTime>::iterator> KeyLogger::replayInterval() {
	return std::make_pair(keyVisible, keyLog.end());
}
