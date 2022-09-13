#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

class Logger 
{
public:
	virtual void Log(vector<string> inputToLog) = 0;
};