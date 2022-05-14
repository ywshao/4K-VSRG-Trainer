#include "bmsParser.h"

int BmsParser::convert36baseToInt(std::string wavNum) {
	int returnNum = 0;
	if ('0' <= wavNum[0] && wavNum[0] <= '9') {
		returnNum += (wavNum[0] - '0') * 36;
	}
	else if ('A' <= wavNum[0] && wavNum[0] <= 'Z') {
		returnNum += (wavNum[0] - 'A' + 10) * 36;
	}
	if ('0' <= wavNum[1] && wavNum[1] <= '9') {
		return returnNum + (wavNum[0] - '0');
	}
	else if ('A' <= wavNum[1] && wavNum[1] <= 'Z') {
		return returnNum + (wavNum[0] - 'A' + 10);
	}
}

std::vector<int> BmsParser::messageToNotes(std::string message) {
	std::vector<int> notes;
	for (int index = 0; index < message.length(); index += 2) {
		notes.push_back(convert36baseToInt(message.substr(index, 2)));
	}
}

void BmsParser::parseFile(const char* file) {
	std::regex regHeader("#\\b[:upper:]+ \\b\\S+");
	std::regex regWav("#WAV\\b([:upper:]|[:digit:]){2} \\b\\S+");
	std::regex regBPM("#BPM\\b([:upper:]|[:digit:]){2} \\b\\S+");
	std::regex regData("#\\b[:upper:]{2}\\b([:upper:]|[:digit:]){2}:\\b([:upper:]|[:digit:])+");
	std::ifstream fstream(file);
	std::string buffer;
	std::smatch match;
	while (std::getline(fstream, buffer)) {
		if (std::regex_search(buffer, match, regHeader)) {
			if (match[0] == std::string("PLAYER")) {
				player = std::stoi(match[1]);
			}
			else if (match[0] == std::string("BPM")) {
				bpm = std::stof(match[1]);
			}
		}
		else if (std::regex_search(buffer, match, regWav)) {
			wav[convert36baseToInt(match[0])] = match[1];
		}
		else if (std::regex_search(buffer, match, regBPM)) {
			bpmFloat[convert36baseToInt(match[0])] = std::stof(match[1]);
		}
		else if (std::regex_search(buffer, match, regData)) {
			switch (std::stoi(match[1])) {
			case 1: // BGM
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
				note[std::stoi(match[0])][0] = messageToNotes(match[2]);
				break;
			case 12:
				note[std::stoi(match[0])][1] = messageToNotes(match[2]);
				break;
			case 13:
				note[std::stoi(match[0])][2] = messageToNotes(match[2]);
				break;
			case 14:
				note[std::stoi(match[0])][3] = messageToNotes(match[2]);
				break;
			case 15:
				note[std::stoi(match[0])][4] = messageToNotes(match[2]);
				break;
			case 18:
				note[std::stoi(match[0])][5] = messageToNotes(match[2]);
				break;
			case 19:
				note[std::stoi(match[0])][6] = messageToNotes(match[2]);
				break;
			case 16:
				note[std::stoi(match[0])][7] = messageToNotes(match[2]);
				break;
			}
		}
	}
}
