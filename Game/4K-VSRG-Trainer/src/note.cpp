#include "note.h"

void KeyLogger<KeySound>::clear() {
	for (int key = 0; key < keyNum; key++) {
		keyLog[key].clear();
	}
}

void KeyLogger<JudgeKeySound>::clear() {
	for (int key = 0; key < keyNum; key++) {
		keyLog[key].clear();
	}
}

void KeyLoggerBgm::clear() {
	for (int key = 0; key < keyNum; key++) {
		keyLog[key].clear();
	}
	bgm = std::priority_queue<KeySound, std::vector<KeySound>, KeySoundCmp>();
}

void KeyLogger<KeySound>::add(int key, KeySound data) {
	keyLog[key].push_back(data);
}

void KeyLogger<JudgeKeySound>::add(int key, JudgeKeySound data) {
	keyLog[key].push_back(data);
}

void Chart::add(int key, KeySound data) {
	if (key < keyNum) {
		keyLog[key].push_back(data);
	}
	else {
		bgm.push(data);
	}
}

std::list<KeySound>::iterator KeyLogger<KeySound>::remove(int key, std::list<KeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

std::list<JudgeKeySound>::iterator KeyLogger<JudgeKeySound>::remove(int key, std::list<JudgeKeySound>::iterator iter) {
	return keyLog[key].erase(iter);
}

int KeyLogger<KeySound>::count() {
	int size = 0;
	for (int key = 0; key < keyNum; key++) {
		size += keyLog[key].size();
	}
	return size;
}
int KeyLogger<JudgeKeySound>::count() {
	int size = 0;
	for (int key = 0; key < keyNum; key++) {
		size += keyLog[key].size();
	}
	return size;
}

int KeyLogger<KeySound>::count(int key) {
	return keyLog[key].size();
}

std::list<KeySound>::iterator* KeyLogger<KeySound>::begin() {
	for (int key = 0; key < keyNum; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<JudgeKeySound>::iterator* KeyLogger<JudgeKeySound>::begin() {
	for (int key = 0; key < keyNum; key++) {
		keyBegin[key] = keyLog[key].begin();
	}
	return keyBegin;
}

std::list<KeySound>::iterator* KeyLogger<KeySound>::end() {
	for (int key = 0; key < keyNum; key++) {
		keyEnd[key] = keyLog[key].end();
	}
	return keyEnd;
}

std::list<JudgeKeySound>::iterator* KeyLogger<JudgeKeySound>::end() {
	for (int key = 0; key < keyNum; key++) {
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

void ChartVisible::init(Chart* chart) {
	for (int key = 0; key < keyNum; key++) {
		if (chart->count(key)) {
			keyToLoad[key] = chart->begin(key);
		}
	}
	bgm = chart->bgm;
}

void JudgeVisible::update(Uint64 chartOffset) {
	for (int key = 0; key < keyNum; key++) {
		for (std::list<JudgeKeySound>::iterator iter = keyLog[key].begin(); !keyLog[key].empty() && iter != keyLog[key].end();) {
			if (iter->time + chartOffset + judgeVisibleMs < SDL_GetTicks64()) {
				iter = keyLog[key].erase(iter);
			}
			else {
				iter++;
			}
		}
	}
}

void ChartVisible::update(Audio* audio, Chart* chart, Uint64 chartOffset) {
	for (int key = 0; key < keyNum; key++) {
		for (; chart->count(key) && keyToLoad[key] != chart->end(key);) {
			if (keyToLoad[key]->time + chartOffset <= SDL_GetTicks64() + addNoteMs || !count(key)) {
				keyLog[key].push_back(*keyToLoad[key]);
				keyToLoad[key]++;
			}
			else {
				break;
			}
		}
	}
	for (int key = 0; key < keyNum; key++) {
		for (std::list<KeySound>::iterator iter = keyLog[key].begin(); !keyLog[key].empty() && iter != keyLog[key].end();) {
			if (iter->time + chartOffset + removeNoteMs < SDL_GetTicks64()) {
				iter = keyLog[key].erase(iter);
			}
			else {
				break;
			}
		}
	}
	while (!bgm.empty()) {
		if (bgm.top().time + chartOffset <= SDL_GetTicks64()) {
			audio->playSound(bgm.top().sound);
			bgm.pop();
		}
		else {
			break;
		}
	}
}
