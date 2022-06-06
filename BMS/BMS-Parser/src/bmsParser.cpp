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

std::vector<int> BmsParser::messageToNotes(std::string message, wavType wavType) {
	std::vector<int> notes;
	for (int index = 0; index < message.length(); index += 2) {
		int wav = convert36baseToInt(message.substr(index, 2));
		notes.push_back(wav);
		if (wavType == isBgm) {
			wavInBgm[wav]++;
		}
		else if (wavType == isNote) {
			wavInNote[wav]++;
		}
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
				wavInNote[wav]++;
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
	int LCM = lcm((int)notes1.size(), (int)notes2.size());
	int notes1Dist = notes1.size() ? LCM / (int)notes1.size() : 0;
	int notes2Dist = notes2.size() ? LCM / (int)notes2.size() : 0;
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

std::vector<int> BmsParser::notesRanking(int bar, int idx, int noteDist[maxKeyNum]) {
	std::vector<int> noteRank;
	for (int key = 0; key < maxKeyNum; key++) {
		if (!note[bar][key].empty() && !(idx % noteDist[key]) && note[bar][key][idx / noteDist[key]]) {
			noteRank.push_back(note[bar][key][idx / noteDist[key]]);
		}
	}
	std::sort(noteRank.begin(), noteRank.end(), [&](int a, int b)-> bool {return wavInNote[a] * wavInBgm[b] > wavInNote[b] * wavInBgm[a]; } );
	return noteRank;
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
				bgm[barNum].push_back(messageToNotes(match[3], isBgm));
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
				note[barNum][0] = messageToNotes(match[3], isNote);
				break;
			case 12:
				note[barNum][1] = messageToNotes(match[3], isNote);
				break;
			case 13:
				note[barNum][2] = messageToNotes(match[3], isNote);
				break;
			case 14:
				note[barNum][3] = messageToNotes(match[3], isNote);
				break;
			case 15:
				note[barNum][4] = messageToNotes(match[3], isNote);
				break;
			case 18:
				note[barNum][5] = messageToNotes(match[3], isNote);
				break;
			case 19:
				note[barNum][6] = messageToNotes(match[3], isNote);
				break;
			case 16:
				note[barNum][7] = messageToNotes(match[3], isNote);
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

void BmsParser::bms7to4(PatternParameter patternParameter) {
	// So it is RNG game this time. No need to prepare your excuses!
	std::random_device rd;
	std::mt19937 mt(rd());
	// Analyze
	double density[maxBarNum] = {}; // Propotional to KPS
	std::vector<double> densityLCM[maxBarNum]; // Propotional to KPS but finer
	std::vector<double> densityPreLCM[maxBarNum]; // Propotional to KPS but finer
	std::vector<double> densitySorted;
	double densityMedian;
	double chordness[maxBarNum] = {};
	std::vector<double> chordnessLCM[maxBarNum];	// Speedly ~ lower chordness
													// Stream < Jumpstream < Handstream < Quadstream;
													// Stream < Chordjack
													// Light Chordstream/Chordjack < Dense Chordstream/Chordjack
	double currentBpm = bpm;
	double nextDensityPreLCM = 0;
	for (int bar = 0; bar <= barMax; bar++) {
		if (!bpmInt[bar].empty()) { // Test
			currentBpm = bpmInt[bar][0];
		}
		double barLength = (double)4 / currentBpm * 60000;
		barLength = timeSignature[bar] ? barLength * timeSignature[bar] : barLength;
		int sizeNote[maxKeyNum];
		std::vector<int> note4[4];
		int sizeLCM = 1;
		//int noteCount = 0;
		for (int key = 0; key < maxKeyNum; key++) {
			sizeNote[key] = (int)note[bar][key].size();
			sizeLCM = lcm(sizeLCM, sizeNote[key]);
		}
		std::vector<int> noteCount(sizeLCM);
		int noteDist[8];
		for (int key = 0; key < maxKeyNum; key++) {
			if (note[bar][key].size()) {
				noteDist[key] = sizeLCM / (int)note[bar][key].size();
				for (int idx = 0; idx < sizeLCM; idx++) {
					if (!(idx % noteDist[key]) && note[bar][key][idx / noteDist[key]]) {
						noteCount[idx]++;
					}
				}
			}
		}
		int noteCountCount[maxKeyNum + 1] = {};
		int noteCountMax = 0;
		int hasNoteCount = 0;
		int totalNoteCount = 0;
		densityLCM[bar] = std::vector<double>(sizeLCM, 0);
		densityPreLCM[bar] = std::vector<double>(sizeLCM, 0);
		chordnessLCM[bar] = std::vector<double>(sizeLCM, 0);//printf("Bar %d: ", bar);
		bool firstNote = true;
		for (int idx = 0; idx < sizeLCM; idx++) {
			noteCountCount[noteCount[idx]]++;
			if (noteCount[idx]) {
				hasNoteCount++;
				if (firstNote) {
					densityPreLCM[bar][idx] = nextDensityPreLCM;
					nextDensityPreLCM = 0;
					firstNote = false;
				}
			}
			int leftBound = (int)(idx - 100 / (barLength / sizeLCM)); // -100ms
			int rightBound = idx + idx - leftBound; // +100ms
			int size = rightBound - leftBound + 1;// printf("%d %d %d\n", leftBound, rightBound, size);
			double densityIdx = (double)noteCount[idx] / barLength * sizeLCM;
			if (size - 1) {
				densityIdx /= size - 1;
			}
			int sizeNonEmpty = 0;
			for (int iidx = 0; iidx < size; iidx++) {
				int index = leftBound + iidx;
				if (index >= 0 && index < sizeLCM && noteCount[index]) {
					sizeNonEmpty++;
				}
			}
			double chordnessIdx = sizeNonEmpty ? (double)noteCount[idx] / sizeNonEmpty : 0;
			for (int iidx = 0; iidx < size; iidx++) {
				int index = leftBound + iidx;
				if (index == sizeLCM) {
					nextDensityPreLCM += densityIdx;
				}
				else if (index >= 0 && index < sizeLCM) {
					if (index > idx) {
						densityPreLCM[bar][index] += chordnessIdx;
					}
					densityLCM[bar][index] += densityIdx;
					chordnessLCM[bar][index] += chordnessIdx;
				}
			}
		}/*
		for (int idx = 0; idx < sizeLCM; idx++) {
			printf("%f ", densityPreLCM[bar][idx]);
		}printf("\n");*/
		for (int idx = 1; idx <= maxKeyNum; idx++) {
			totalNoteCount += noteCountCount[idx] * idx;
		}
		density[bar] = totalNoteCount / barLength;
		densitySorted.push_back(density[bar]);
		chordness[bar] = hasNoteCount ? totalNoteCount / hasNoteCount : 1;
	}
	std::sort(densitySorted.begin(), densitySorted.end());
	densityMedian = densitySorted[densitySorted.size() / 2];

	// Convert
	int preprepreNote4[4] = {};
	int preprepreNoteCount4 = 0;
	int prepreNote4[4] = {};
	int prepreNoteCount4 = 0;
	int preNote4[4] = {};
	int preNoteCount4 = 0;

	int quadAccumulate = patternParameter.quad ? 100 : 0;
	int handAccumulate = patternParameter.hand ? 100 : 0;
	int jumpAccumulate = patternParameter.jump ? 100 : 0;
	for (int bar = 0; bar <= barMax; bar++) {
		int sizeNote[maxKeyNum];
		std::vector<int> note4[4];
		std::vector<int> bgm8[maxKeyNum];
		int note4Count;
		int sizeLCM = 1;
		//int noteCount = 0;
		for (int key = 0; key < maxKeyNum; key++) {
			sizeNote[key] = (int)note[bar][key].size();
			sizeLCM = lcm(sizeLCM, sizeNote[key]);
		}
		std::vector<int> noteCount(sizeLCM);
		int noteDist[8];
		for (int key = 0; key < maxKeyNum; key++) {
			if (note[bar][key].size()) {
				noteDist[key] = sizeLCM / (int)note[bar][key].size();
				for (int idx = 0; idx < sizeLCM; idx++) {
					if (!(idx % noteDist[key]) && note[bar][key][idx / noteDist[key]]) {
						noteCount[idx]++;
					}
				}
			}
		}
		int noteCountCount[maxKeyNum + 1] = {};
		int noteCountMax = 0;
		int hasNoteCount = 0;
		for (int idx = 0; idx < sizeLCM; idx++) {
			noteCountCount[noteCount[idx]]++;
			if (noteCount[idx]) {
				hasNoteCount++;
			}
		}
		/*for (int idx = 0; idx <= maxKeyNum; idx++) {
			printf("%d ", noteCountCount[idx]);
		}*/
		for (int idx = maxKeyNum; idx >= 0; idx--) {
			if (noteCountCount[idx]) {
				noteCountMax = idx;
				break;
			}
		}
		//printf("Bar %d\n", bar);
		int accumulator[maxKeyNum + 1] = {};
		for (int key = 1; key <= noteCountMax; key++) {
			accumulator[key] += accumulator[key - 1] + noteCountCount[key];
		}
		//printf("\n");
		//printf("Dense(median) Dense Chord: %f %f %f\n", densityMedian, density[bar], chordness[bar]);
		for (int idx = 0; idx < sizeLCM; idx++) {
			bool notePlaced = false;
			//printf("%d %d\n", noteCount[idx], noteCountMax);
			if (!noteCount[idx]) {
				for (int key = 0; key < 4; key++) {
					note4[key].push_back(0);
				}
				for (int bgm = 0; bgm < maxKeyNum; bgm++) {
					bgm8[bgm].push_back(0);
				}
				continue;
			}
			else {
				bool possibleJack = rand() % 100 < patternParameter.jack;
				if (accumulator[noteCount[idx]] == hasNoteCount && noteCount[idx] >= 4) {
					quadAccumulate += possibleJack ? patternParameter.quad : patternParameter.quad * 0.5;
					handAccumulate += possibleJack ? patternParameter.hand : patternParameter.hand * 0.5;
					jumpAccumulate += possibleJack ? patternParameter.jump : patternParameter.jump * 0.5;
				}
				else if ((double)accumulator[noteCount[idx]] / hasNoteCount >= 0.75) {
					if (noteCount[idx] >= 5) {
						quadAccumulate += possibleJack ? patternParameter.quad * 0.5 : patternParameter.quad * 0.25;
						handAccumulate += possibleJack ? patternParameter.hand : patternParameter.hand * 0.5;
						jumpAccumulate += possibleJack ? patternParameter.jump : patternParameter.jump * 0.5;
					}
					else if (noteCount[idx] >= 4) {
						quadAccumulate += possibleJack ? patternParameter.quad * 0.25 : patternParameter.quad * 0.125;
						handAccumulate += possibleJack ? patternParameter.hand : patternParameter.hand * 0.5;
						jumpAccumulate += possibleJack ? patternParameter.jump : patternParameter.jump * 0.5;
					}
					else {
						handAccumulate += possibleJack ? patternParameter.hand : patternParameter.hand * 0.5;
						jumpAccumulate += possibleJack ? patternParameter.jump * 0.5 : patternParameter.jump * 0.25;
					}
				}
				else if ((double)accumulator[noteCount[idx]] / hasNoteCount >= 0.5) {
					if (noteCount[idx] >= 3) {
						handAccumulate += possibleJack ? patternParameter.hand : patternParameter.hand * 0.5;
						jumpAccumulate += possibleJack ? patternParameter.jump * 0.5 : patternParameter.jump * 0.25;
					}
					else {
						jumpAccumulate += possibleJack ? patternParameter.jump : patternParameter.jump * 0.5;
					}
				}
				else if ((double)accumulator[noteCount[idx]] / hasNoteCount >= 0.25 && noteCount[idx] >= 2) {
					jumpAccumulate += possibleJack ? patternParameter.jump : patternParameter.jump * 0.5;
				}
				int maxNote = 1;
				if (!notePlaced && quadAccumulate >= 100) {
					notePlaced = true;
					quadAccumulate -= 100;
					maxNote = 4;
				}
				else if (!notePlaced && handAccumulate >= 100) {
					notePlaced = true;
					handAccumulate -= 100;
					maxNote = 3;
				}
				else if (!notePlaced && jumpAccumulate >= 100) {
					notePlaced = true;
					jumpAccumulate -= 100;
					maxNote = 2;
				}
				//printf("Acc: %d %d %d\n", quadAccumulate, handAccumulate, jumpAccumulate);
				std::vector<int> notesRanked = notesRanking(bar, idx, noteDist);
				std::vector<int> possibleKey;
				int possibleKeyCount = 0;
				for (int a = 0; a < 4; a++) {
					if (!preNote4[a] ||
						(preNoteCount4 == 4 && patternParameter.jack) ||
						possibleJack) { // Candidate or Jack
						if (!preNote4[a] ||
							(preNoteCount4 == 4 && patternParameter.jack) ||
							!prepreNote4[a] ||
							possibleJack && prepreNote4[a] && rand() % 100 < patternParameter.jackLength) { // Candidate or JackLength
							if (!preNote4[a] ||
								(preNoteCount4 == 4 && patternParameter.jack) ||
								!prepreNote4[a] ||
								!preprepreNote4[a] ||
								possibleJack && prepreNote4[a] && preprepreNote4[a] && rand() % 100 < patternParameter.jackLength) { // Candidate or JackLength
								if (densityPreLCM[bar][idx - 1 >= 0 ? idx - 1 : idx] * 4 >= densityMedian && densityPreLCM[bar][idx] * 2 >= densityMedian && densityLCM[bar][idx] >= densityMedian && (chordnessLCM[bar][idx] < 1.2 || noteCount[idx] <= 1) ||
									patternParameter.jump <= 10 && patternParameter.hand <= 10 && patternParameter.quad <= 0) { // Speed
									//printf("Speed bar: %d %d\n", bar, idx);
									//printf("Dense Chord: %f %f %f\n", densityPreLCM[bar][idx], densityLCM[bar][idx], chordnessLCM[bar][idx]);
									if ((!prepreNote4[a] && preprepreNote4[a]) && rand() % 50 < patternParameter.speedTech) { // SpeedTech triplet
										possibleKey.push_back(a);
										possibleKeyCount++;
									}
									else if ((prepreNote4[a] && rand() % 75 + 25 < patternParameter.speedTech)) { // SpeedTech trill
										possibleKey.push_back(a);
										possibleKeyCount++;
									}
									else if (preNoteCount4 >= 3) {
										possibleKey.push_back(a);
										possibleKeyCount++;
									}
									else if (prepreNoteCount4 >= 2 && !prepreNote4[a]) {
										possibleKey.push_back(a);
										possibleKeyCount++;
									}
									else if (!preNote4[a] && !prepreNote4[a] && !preprepreNote4[a]) {
										possibleKey.push_back(a);
										possibleKeyCount++;
									}
								}
								else {
									//printf("Normal bar: %d %d\n", bar, idx);
									//printf("Dense Chord: %f %f %f\n", densityPreLCM[bar][idx], densityLCM[bar][idx], chordnessLCM[bar][idx]);
									possibleKey.push_back(a);
									possibleKeyCount++;
								}
							}
						}
					}
					else if (densityLCM[bar][idx] && densityLCM[bar][idx] <= densityMedian * 1.5 && chordnessLCM[bar][idx] * densityMedian >= densityLCM[bar][idx] * 2 && preNoteCount4 >= 2 && maxNote >= 2 && rand() % 100 < patternParameter.tech) {
						possibleKey.push_back(a);
						possibleKeyCount++;
					}
				}
				note4Count = 0;
				std::shuffle(possibleKey.begin(), possibleKey.end(), mt);
				int bitHack = 0xf;
				for (int a = 0; a < 4; a++) {
					if (a < possibleKeyCount && a <= noteCount[idx] && a < notesRanked.size() && a < maxNote) {
						note4[possibleKey[a]].push_back(notesRanked[a]);
						note4Count++;
						bitHack -= 1 << possibleKey[a];
					}
				}
				for (int a = 0; bitHack; a++) {
					if (bitHack & 0x1) {
						note4[a].push_back(0);
					}
					bitHack >>= 1;
				}
				for (int a = 0; a < maxKeyNum; a++) {
					if (note4Count <= a && a < notesRanked.size()) {
						bgm8[a].push_back(notesRanked[a]);
					}
					else {
						bgm8[a].push_back(0);
					}
				}
			}
			for (int key = 0; key < 4; key++) {
				preprepreNote4[key] = prepreNote4[key];
				prepreNote4[key] = preNote4[key];
				preNote4[key] = note4[key].back();
			}
			preprepreNoteCount4 = prepreNoteCount4;
			prepreNoteCount4 = preNoteCount4;
			preNoteCount4 = note4Count;
		}
		for (int key = 0; key < 4; key++) {
			note[bar][key] = note4[key];
		}
		for (int key = 4; key < maxKeyNum; key++) {
			note[bar][key] = std::vector<int>();
		}
		for (int a = 0; a < maxKeyNum; a++) {
			bgm[bar].push_back(bgm8[a]);
		}
	}
}

void BmsParser::clear() {
	player = 0;
	bpm = 0;
	barMax = 0;
	bpmInt->clear();
	for (int bar = 0; bar < maxBarNum; bar++) {
		std::vector<std::vector<int>>().swap(bgm[bar]);
		for (int key = 0; key < maxKeyNum; key++) {
			note[bar][key].clear();
		}
		timeSignature[bar] = 0;
	}
	for (int idx = 0; idx < maxIndex; idx++) {
		bpmInt[idx].clear();
		wavInBgm[idx] = 0;
		wavInNote[idx] = 0;
	}
}
