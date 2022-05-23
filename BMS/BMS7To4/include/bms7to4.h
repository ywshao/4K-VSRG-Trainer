#pragma once
#include <iomanip>
#include "bmsParser.h"

class Bms7To4 {
private:
	BmsParser bmsParser;
	int gcd(int a, int b);
	int lcm(int a, int b);
	std::string convertIntTo36base(int wavNum);
	std::string convertIntToHex(int wavNum);
public:
	void convert(const char* file);
};