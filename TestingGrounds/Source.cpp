#include <cstdint>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <map>
#include "Utilities.h"

using std::map;
using std::vector;
using std::string;




int main() 
{
	map<string, uint16_t> labelInstructionIndexPairs;
	labelInstructionIndexPairs.insert({ "LABEL", 5 });
	labelInstructionIndexPairs.insert({ "LABEL2", 6 });
	
	std::cout << labelInstructionIndexPairs["LABEL"] << std::endl;
	
	return 0;
}