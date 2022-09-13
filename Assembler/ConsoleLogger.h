#pragma once
#include <iostream>
#include "Logger.h"


class ConsoleLogger : public Logger
{
	void Log(vector<string> stuffToLog);
};