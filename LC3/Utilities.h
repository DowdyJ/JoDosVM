#include <cstdint>
#include <string>

using std::string;

class Utilities 
{
public:
	static void LoadFileInto(string filename, uint16_t* test, uint16_t numberToRead);
};