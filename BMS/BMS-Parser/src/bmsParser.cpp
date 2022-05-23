#include "bmsParser.h"

#include <iostream>

int BmsParser::gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}

int BmsParser::lcm(int a, int b) {
	return a * b ? a * b / gcd(a, b) : a;
}

int BmsParser::convert36baseToInt(std::string wavNum) {
	int returnNum = 0;
	if ('0' <= wavNum[0] && wavNum[0] <= '9') {
		returnNum += (wavNum[0] - '0') * 36;
	}
	else if ('A' <= wavNum[0] && wavNum[0] <= 'Z') {
		returnNum += (wavNum[0] - 'A' + 10) * 36;
	}
	if ('0' <= wavNum[1] && wavNum[1] <= '9') {
		return returnNum + (wavNum[1] - '0');
	}
	else if ('A' <= wavNum[1] && wavNum[1] <= 'Z') {
		return returnNum + (wavNum[1] - 'A' + 10);
	}
	else {
		return 0;
	}
}

int BmsParser::convertHexToInt(std::string wavNum) {
	int returnNum = 0;
	if ('0' <= wavNum[0] && wavNum[0] <= '9') {
		returnNum += (wavNum[0] - '0') * 16;
	}
	else if ('A' <= wavNum[0] && wavNum[0] <= 'F') {
		returnNum += (wavNum[0] - 'A' + 10) * 16;
	}
	if ('0' <= wavNum[1] && wavNum[1] <= '9') {
		return returnNum + (wavNum[1] - '0');
	}
	else if ('A' <= wavNum[1] && wavNum[1] <= 'F') {
		return returnNum + (wavNum[1] - 'A' + 10);
	}
	else {
		return 0;
	}
}

std::vector<int> BmsParser::messageToNotes(std::string message) {
	std::vector<int> notes;
	for (int index = 0; index < message.length(); index += 2) {
		notes.push_back(convert36baseToInt(message.substr(index, 2)));
	}
	return notes;
}

std::vector<int> BmsParser::messageToLongNotes(std::string message, int key) {
	std::vector<int> notes;
	for (int index = 0; index < message.length(); index += 2) {
		int wav = convert36baseToInt(message.substr(index, 2));
		if (wav) {
			if (!longNoteCancel[key]) {
				notes.push_back(wav);
				longNoteCancel[key] = true;
			}
			else {
				notes.push_back(0);
				longNoteCancel[key] = false;
			}
		}
		else {
			notes.push_back(0);
		}
	}
	return notes;
}

std::vector<int> BmsParser::messageToBpms(std::string message) {
	std::vector<int> bpms;
	for (int index = 0; index < message.length(); index += 2) {
		bpms.push_back(convertHexToInt(message.substr(index, 2)));
	}
	return bpms;
}

std::vector<int> BmsParser::notesMerge(std::vector<int> notes1, std::vector<int> notes2) {
	std::vector<int> notes;
	int LCM = lcm(notes1.size(), notes2.size());
	int notes1Dist = notes1.size() ? LCM / notes1.size() : 0;
	int notes2Dist = notes2.size() ? LCM / notes2.size() : 0;
	for (int index = 0; index < LCM; index++) {
		if (notes1Dist && !(index % notes1Dist)) {
			notes.push_back(notes1[index / notes1Dist]);
		}
		else if (notes2Dist && !(index % notes2Dist)) {
			notes.push_back(notes2[index / notes2Dist]);
			std::cout << notes2[index / notes2Dist] << std::endl;
		}
		else {
			notes.push_back(0);
		}
	}
	return notes;
}

void BmsParser::parseFile(const char* file) {
	std::regex regWav("#WAV([[:upper:]|[:digit:]]{2}) (.+)");
	std::regex regBPM("#BPM([[:upper:]|[:digit:]]{2}) (.+)");
	std::regex regHeader("#([[:upper:]]+) (.+)");
	std::regex regData("#([[:digit:]]{3})([[:digit:]]{2}):([[:upper:]|[:digit:]|\.]+)");
	std::ifstream fstream(file);
	std::string buffer;
	std::smatch match;
	while (std::getline(fstream, buffer)) {
		if (std::regex_search(buffer, match, regWav)) {
			wav[convert36baseToInt(match[1])] = match[2];
		}
		else if (std::regex_search(buffer, match, regBPM)) {
			bpmFloat[convert36baseToInt(match[1])] = std::stof(match[2]);
		}
		else if (std::regex_search(buffer, match, regHeader)) {
			if (match[1] == std::string("PLAYER")) {
				player = std::stoi(match[2]);
			}
			else if (match[1] == std::string("BPM")) {
				bpm = std::stof(match[2]);
			}
		}
		else if (std::regex_search(buffer, match, regData)) {
			int barNum = std::stoi(match[1]);
			barMax = barNum > barMax ? barNum : barMax;
			switch (std::stoi(match[2])) {
			case 1: // BGM
				bgm[barNum].push_back(messageToNotes(match[3]));
				break;
			case 2: // Time signature
				timeSignature[barNum] = std::stof(match[3]);
				break;
			case 3: // BPM
				bpmInt[barNum] = messageToBpms(match[3]);
				break;
			case 8: // BPM float
				break;
			case 9: // Pause
				fprintf(stderr, "Stop Sequence detected!\n");
				break;
			case 11:
				note[barNum][0] = messageToNotes(match[3]);
				break;
			case 12:
				note[barNum][1] = messageToNotes(match[3]);
				break;
			case 13:
				note[barNum][2] = messageToNotes(match[3]);
				break;
			case 14:
				note[barNum][3] = messageToNotes(match[3]);
				break;
			case 15:
				note[barNum][4] = messageToNotes(match[3]);
				break;
			case 18:
				note[barNum][5] = messageToNotes(match[3]);
				break;
			case 19:
				note[barNum][6] = messageToNotes(match[3]);
				break;
			case 16:
				note[barNum][7] = messageToNotes(match[3]);
				break;
			case 51:
				note[barNum][0] = notesMerge(messageToLongNotes(match[3], 0), note[barNum][0]);
				break;
			case 52:
				note[barNum][1] = notesMerge(messageToLongNotes(match[3], 1), note[barNum][1]);
				break;
			case 53:
				note[barNum][2] = notesMerge(messageToLongNotes(match[3], 2), note[barNum][2]);
				break;
			case 54:
				note[barNum][3] = notesMerge(messageToLongNotes(match[3], 3), note[barNum][3]);
				break;
			case 55:
				note[barNum][4] = notesMerge(messageToLongNotes(match[3], 4), note[barNum][4]);
				break;
			case 58:
				note[barNum][5] = notesMerge(messageToLongNotes(match[3], 5), note[barNum][5]);
				break;
			case 59:
				note[barNum][6] = notesMerge(messageToLongNotes(match[3], 6), note[barNum][6]);
				break;
			case 56:
				note[barNum][7] = notesMerge(messageToLongNotes(match[3], 7), note[barNum][7]);
				break;
			}
		}
	}
}
