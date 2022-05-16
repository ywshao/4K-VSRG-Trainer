#include "bmsParser.h"

#include <iostream>

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

std::vector<int> BmsParser::messageToNotes(std::string message) {
	std::vector<int> notes;
	for (int index = 0; index < message.length(); index += 2) {
		notes.push_back(convert36baseToInt(message.substr(index, 2)));
	}
	return notes;
}

void BmsParser::parseFile(const char* file) {
	std::regex regWav("#WAV([[:upper:]|[:digit:]]{2}) (.+)");
	std::regex regBPM("#BPM([[:upper:]|[:digit:]]{2}) (.+)");
	std::regex regHeader("#([[:upper:]]+) (.+)");
	std::regex regData("#([[:digit:]]{3})([[:digit:]]{2}):([[:upper:]|[:digit:]]+)");
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
				break;
			case 3: // BPM
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
			}
		}
	}
}
