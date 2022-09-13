#include "ConsoleLogger.h"



void ConsoleLogger::Log(vector<string> stuffToLog) 
{
	for (size_t i = 0; i < stuffToLog.size(); i++)
	{
		std::cout << stuffToLog[i] << std::endl;
	}
}