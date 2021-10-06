#include "UUID.h"
#include <time.h>
#include <ctime>
#include <iostream>
#include <memory>
#include <sstream>

/*
UUIDs are written in 5 groups of hexadecimal digits separated by hyphens.
The length of each group is: 8-4-4-4-12. UUIDs are fixed length.
For example: 123e4567-e89b-12d3-a456-426655440000
*/

char characters[16] = {
	'0', '1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
};

void UUID::Init()
{
	srand(time(NULL));
}

std::string UUID::GenerateUUID()
{
	std::string uuid;

	std::string castedResult = std::to_string(std::time(nullptr));

	std::string lastEight = castedResult.substr(2, 8);

	std::stringstream ss;
	ss << std::hex << std::stoi(lastEight);

	uuid += ss.str();

	uuid += GenerateSequence(8 % ss.str().length());
	uuid += '-';

	uuid += GenerateSequence(4);
	uuid += '-';

	uuid += GenerateSequence(4);
	uuid += '-';

	uuid += GenerateSequence(4);
	uuid += '-';

	uuid += GenerateSequence(12);

	return uuid;
}

std::string UUID::GenerateSequence(int length)
{
	std::string sequence;

	for (int i = 0; i < length; i++) {
		int index = rand() % 16;
		sequence += characters[index];
	}

	return sequence;
}
