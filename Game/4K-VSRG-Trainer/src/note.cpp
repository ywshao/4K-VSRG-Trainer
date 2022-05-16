#include "note.h"

void KeyLogger<KeySound, 4>::clear() {
	for (int key = 0; key < 4; key++) {
		keyLog[key].clear();
	}
}

void KeyLogger<KeySound, 4>::add(int key, KeySound data) {
	keyLog[key].push_back(data);
}

void KeyLogger<KeySound, 5>::add(int key, KeySound data) {
	keyLog[key].push_back(data);
}

void KeyLogger<JudgeKeySound, 4>::add(int key, JudgeKeySound data) {
	keyLog[key].push_back(data);
}

std::list<KeySound>::iterator KeyLogger<KeySound, 4>::remove(int key, std::list<KeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

std::list<KeySound>::iterator KeyLogger<KeySound, 5>::remove(int key, std::list<KeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound, 4>::remove(int key, std::list<JudgeKeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

int KeyLogger<KeySound, 4>::count() {
	int size = 0;
	for (int key = 0; key < 4; key++) {
		size += keyLog[key].size();
	}
	return size;
}
int KeyLogger<KeySound, 5>::count() {
	int size = 0;
	for (int key = 0; key < 4; key++) {
		size += keyLog[key].size();
	}
	return size;
}

int KeyLogger<JudgeKeySound, 4>::count() {
	int size = 0;
	for (int key = 0; key < 4; key++) {
		size += keyLog[key].size();
	}
	return size;
}

// Test
/*
int KeyLogger<T, 4>::count(int key) {
	return keyLog[key].size();
}
*/
std::list<KeySound>::iterator* KeyLogger<KeySound, 4>::begin() {
	for (int key = 0; key < 4; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<KeySound>::iterator* KeyLogger<KeySound, 5>::begin() {
	for (int key = 0; key < 5; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<JudgeKeySound>::iterator* KeyLogger<JudgeKeySound, 4>::begin() {
	for (int key = 0; key < 4; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<KeySound>::iterator* KeyLogger<KeySound, 4>::end() {
	for (int key = 0; key < 4; key++) {
		keyEnd[key] = keyLog[key].end();
	}
	return keyEnd;
}

std::list<KeySound>::iterator* KeyLogger<KeySound, 5>::end() {
	for (int key = 0; key < 5; key++) {
		keyEnd[key] = keyLog[key].end();
	}
	return keyEnd;
}

std::list<JudgeKeySound>::iterator* KeyLogger<JudgeKeySound, 4>::end() {
	for (int key = 0; key < 4; key++) {
		keyEnd[key] = keyLog[key].end();
	}
	return keyEnd;
}

std::list<KeySound>::iterator KeyLogger<KeySound, 4>::begin(int key) {
	return keyLog[key].begin();
}

std::list<KeySound>::iterator KeyLogger<KeySound, 5>::begin(int key) {
	return keyLog[key].begin();
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound, 4>::begin(int key) {
	return keyLog[key].begin();
}

std::list<KeySound>::iterator KeyLogger<KeySound, 4>::end(int key) {
	return keyLog[key].end();
}

std::list<KeySound>::iterator KeyLogger<KeySound, 5>::end(int key) {
	return keyLog[key].end();
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound, 4>::end(int key) {
	return keyLog[key].end();
}

void JudgeVisible::update() {
	for (int key = 0; key < 4; key++) {
		for (std::list<JudgeKeySound>::iterator iter = keyLog[key].begin(); !keyLog[key].empty() && iter != keyLog[key].end();) {
			if (iter->time + 2000 < SDL_GetTicks64()) {
				iter = keyLog[key].erase(iter);
			}
			else {
				iter++;
			}
		}
	}
}

void ChartVisible::update(Audio* audio) {
	for (int key = 0; key < 4; key++) {
		for (std::list<KeySound>::iterator iter = keyLog[key].begin(); !keyLog[key].empty() && iter != keyLog[key].end();) {
			if (iter->time + 500 < SDL_GetTicks64()) {
				iter = keyLog[key].erase(iter);
			}
			else {
				iter++;
			}
		}
	}
	for (std::list<KeySound>::iterator iter = keyLog[4].begin(); !keyLog[4].empty() && iter != keyLog[4].end();) {
		if (iter->time <= SDL_GetTicks64()) {
			audio->playSound(iter->sound);
			iter = keyLog[4].erase(iter);
		}
		else {
			iter++;
		}
	}
}
