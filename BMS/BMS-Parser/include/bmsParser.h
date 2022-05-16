#pragma once

#define bufferSize 1000
#define maxIndex 1536
#define maxBarNum 1000
#define maxKeyNum 8

#include <cstdlib>
#include <fstream>
#include <vector>
#include <list>
#include <regex>

class BmsParser {
private:
	FILE* fp;
	char buffer[bufferSize];
	int convert36baseToInt(std::string wavNum);
	std::vector<int> messageToNotes(std::string message);
public:
	int player;
	float bpm;
	float bpmFloat[maxIndex];
	int barMax;
	std::string wav[maxIndex] = {};
	std::list<float> timeSignature;
	std::vector<std::vector<int>> bgm[maxBarNum];
	std::vector<int> note[maxBarNum][maxKeyNum];
	void parseFile(const char* file);
};
