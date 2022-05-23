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
	bool longNoteCancel[maxKeyNum] = {};
	int gcd(int a, int b);
	int lcm(int a, int b);
	int convertHexToInt(std::string wavNum);
	int convert36baseToInt(std::string wavNum);
	std::vector<int> messageToNotes(std::string message);
	std::vector<int> messageToLongNotes(std::string message, int key);
	std::vector<int> messageToBpms(std::string message);
	std::vector<int> notesMerge(std::vector<int> notes1, std::vector<int> notes2);
public:
	int player;
	float bpm;
	std::vector<int> bpmInt[maxIndex];
	float bpmFloat[maxIndex];
	int barMax;
	std::string wav[maxIndex] = {};
	float timeSignature[maxBarNum] = {};
	std::vector<std::vector<int>> bgm[maxBarNum];
	std::vector<int> note[maxBarNum][maxKeyNum];
	void parseFile(const char* file);
};
