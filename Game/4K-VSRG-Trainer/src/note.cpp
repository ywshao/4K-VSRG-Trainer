#include "note.h"

template <typename T>
void KeyLogger<T>::clear() {
	for (int key = 0; key < 4; key++) {
		keyLog[key].clear();
	}
}

void KeyLogger<KeySound>::add(int key, KeySound data) {
	keyLog[key].push_back(data);
}

void KeyLogger<JudgeKeySound>::add(int key, JudgeKeySound data) {
	keyLog[key].push_back(data);
}

std::list<KeySound>::iterator KeyLogger<KeySound>::remove(int key, std::list<KeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound>::remove(int key, std::list<JudgeKeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

int KeyLogger<KeySound>::count() {
	int size = 0;
	for (int key = 0; key < 4; key++) {
		size += keyLog[key].size();
	}
	return size;
}

int KeyLogger<JudgeKeySound>::count() {
	int size = 0;
	for (int key = 0; key < 4; key++) {
		size += keyLog[key].size();
	}
	return size;
}

// Test
template <typename T>
int KeyLogger<T>::count(int key) {
	return keyLog[key].size();
}

std::list<KeySound>::iterator* KeyLogger<KeySound>::begin() {
	for (int key = 0; key < 4; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<JudgeKeySound>::iterator* KeyLogger<JudgeKeySound>::begin() {
	for (int key = 0; key < 4; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<KeySound>::iterator* KeyLogger<KeySound>::end() {
	for (int key = 0; key < 4; key++) {
		keyEnd[key] = keyLog[key].end();
	}
	return keyEnd;
}

std::list<JudgeKeySound>::iterator* KeyLogger<JudgeKeySound>::end() {
	for (int key = 0; key < 4; key++) {
		keyEnd[key] = keyLog[key].end();
	}
	return keyEnd;
}

std::list<KeySound>::iterator KeyLogger<KeySound>::begin(int key) {
	return keyLog[key].begin();
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound>::begin(int key) {
	return keyLog[key].begin();
}

std::list<KeySound>::iterator KeyLogger<KeySound>::end(int key) {
	return keyLog[key].end();
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound>::end(int key) {
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

void ChartVisible::update() {
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
}
