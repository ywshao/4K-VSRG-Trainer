#pragma once

#define bufferSize 1000
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <list>
#include <regex>

class BmsParser {
private:
	FILE* fp;
	char buffer[bufferSize];
	int player;
	float bpm;
	float bpmFloat[1536];
	std::string wav[1536] = {};
	std::list<float> timeSignature;
	std::vector<int> note[1000][8];
	int convert36baseToInt(std::string wavNum);
	std::vector<int> messageToNotes(std::string message);
public:
	void parseFile(const char* file);
};
