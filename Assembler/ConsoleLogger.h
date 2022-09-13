#pragma once
#include <iostream>
#include "Logger.h"


class ConsoleLogger : Logger
{
	void Log(vector<string> stuffToLog);
};