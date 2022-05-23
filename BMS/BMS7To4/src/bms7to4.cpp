#include "bms7to4.h"

int Bms7To4::gcd(int a, int b) {
	return b ? gcd(b, a % b) : a;
}

int Bms7To4::lcm(int a, int b) {
	return a * b ? a * b / gcd(a, b) : a;
}

std::string Bms7To4::convertIntTo36base(int wavNum) {
	std::string returnNum;
	int num = wavNum / 36;
	if (0 <= num && num <= 9) {
		returnNum.push_back(num + '0');
	}
	else {
		returnNum.push_back(num - 10 + 'A');
	}
	num = wavNum % 36;
	if (0 <= num && num <= 9) {
		returnNum.push_back(num + '0');
	}
	else {
		returnNum.push_back(num - 10 + 'A');
	}
	return returnNum;
}

std::string Bms7To4::convertIntToHex(int wavNum) {
	std::string returnNum;
	int num = wavNum / 16;
	if (0 <= num && num <= 9) {
		returnNum.push_back(num + '0');
	}
	else {
		returnNum.push_back(num - 10 + 'A');
	}
	num = wavNum % 16;
	if (0 <= num && num <= 9) {
		returnNum.push_back(num + '0');
	}
	else {
		returnNum.push_back(num - 10 + 'A');
	}
	return returnNum;
}

void Bms7To4::convert(const char* file) {
	std::ofstream fstream("output.bm4");
	bmsParser.parseFile(file);
	fstream << "#BPM" << ' ' << bmsParser.bpm << std::endl;
	for (int wav = 0; wav < 1536; wav++) {
		if (!bmsParser.wav[wav].empty()) {
			fstream << "#WAV" << convertIntTo36base(wav) << ' ' << bmsParser.wav[wav] << std::endl;
		}
	}
	for (int bar = 0; bar <= bmsParser.barMax; bar++) {
		for (int idx = 0; idx < bmsParser.bgm[bar].size(); idx++) {
			fstream << "#" << std::setfill('0') << std::setw(3) << bar << "01:";
			for (int bgm = 0; bgm < bmsParser.bgm[bar][idx].size(); bgm++) {
				fstream << convertIntTo36base(bmsParser.bgm[bar][idx][bgm]);
			}
			fstream << std::endl;
		}
	}
	for (int bar = 0; bar <= bmsParser.barMax; bar++) {
		if (bmsParser.timeSignature[bar]) {
			fstream << "#" << std::setfill('0') << std::setw(3) << bar << "02:" << bmsParser.timeSignature[bar] << std::endl;
		}
	}
	for (int bar = 0; bar <= bmsParser.barMax; bar++) {
		if (!bmsParser.bpmInt[bar].empty()) {
			fstream << "#" << std::setfill('0') << std::setw(3) << bar << "03:";
			for (int idx = 0; idx < bmsParser.bgm[bar].size(); idx++) {
				fstream << convertIntToHex(bmsParser.bpmInt[bar][idx]);
			}
			fstream << std::endl;
		}
	}
	int prepreviousNote[8] = {};
	int previousNote[8] = {};
	int previousNoteCount = 0;
	for (int bar = 0; bar <= bmsParser.barMax; bar++) {
		std::vector<int> note4[8];
		int sizeLCM = 1;
		int sizeNote[8];
		bool flag = false;
		for (int key = 0; key < 8; key++) {
			sizeNote[key] = bmsParser.note[bar][key].size();
			sizeLCM = lcm(sizeLCM, sizeNote[key]);
			if (bmsParser.note[bar][key].size()) {
				flag = true;
			}
		}
		if (flag) {
			std::vector<int> note8[8];
			for (int key = 0; key < 8; key++) {
				if (bmsParser.note[bar][key].size()) {
					int noteDist = sizeLCM / bmsParser.note[bar][key].size();
					for (int index = 0; index < sizeLCM; index++) {
						if (index % noteDist) {
							note8[key].push_back(0);
						}
						else {
							note8[key].push_back(bmsParser.note[bar][key][index / noteDist]);
						}
					}
				}
				else {
					for (int index = 0; index < sizeLCM; index++) {
						note8[key].push_back(0);
					}
				}
			}
			for (int index = 0; index < sizeLCM; index++) {
				int currentNote[8] = {};
				int currentNoteCount = 0;
				std::vector<int> currentNote8;
				for (int key = 0; key < 8; key++) {
					if (note8[key][index]) {
						currentNote8.push_back(note8[key][index]);
					}
				}
				if (!currentNote8.size()) {
					for (int key = 0; key < 8; key++) {
						note4[key].push_back(0);
					}
					continue;
				}
				int newNoteNum = 0;
				for (; newNoteNum < 4 - previousNoteCount && newNoteNum < currentNote8.size(); newNoteNum++) {
					int key;
					while (previousNote[key = rand() % 4] || currentNote[key]);
					if (prepreviousNote[key]) {
						while (previousNote[key = rand() % 4] || currentNote[key]);
					}
					currentNote[key] = currentNote8[newNoteNum];
					currentNoteCount++;
				}
				for (int key = 4; newNoteNum < currentNote8.size(); newNoteNum++) {
					currentNote[key++] = currentNote8[newNoteNum];
				}
				for (int key = 0; key < 8; key++) {
					note4[key].push_back(currentNote[key]);
					prepreviousNote[key] = previousNote[key];
					previousNote[key] = currentNote[key];
				}
				previousNoteCount = currentNoteCount;
			}
			for (int key = 0; key < 4; key++) {
				fstream << '#' << std::setfill('0') << std::setw(3) << bar << '1' << key + 1 << ':';
				for (auto i : note4[key]) {
					fstream << convertIntTo36base(i);
				}
				fstream << std::endl;
			}
			for (int key = 4; key < 8; key++) {
				fstream << '#' << std::setfill('0') << std::setw(3) << bar << "01:";
				for (auto i : note4[key]) {
					fstream << convertIntTo36base(i);
				}
				fstream << std::endl;
			}
		}
	}
	fstream.close();
}