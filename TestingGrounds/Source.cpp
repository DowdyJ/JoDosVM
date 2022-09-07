#include <cstdint>
#include <iostream>
#include <bitset>



uint16_t ExtendSign(uint16_t value, uint16_t bits) 
{
	if ((value >> (bits - 1)) & 1)
	{
		value = ~value;
		value += 1;
		
	}

	std::cout << std::bitset<16>(value) << std::endl;
	return value;
}


int main() 
{
	while (true)
	{
		std::bitset<16> buffer;
		std::cin >> buffer;
		uint16_t egg = static_cast<uint16_t>(buffer.to_ulong());
		auto value = ExtendSign(egg, 5);
		//auto otherVAl = sign_extend(egg, 5);
		//std::cout << std::bitset<16>(otherVAl) << std::endl;
		//std::cout << "The two functions are equal: " << (value == otherVAl) << std::endl;
	}


	return 0;
}