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

UUIDs have 32 digits plus 4 hyphens for a total of 36 characters.
UUIDs are fixed length. UUIDs are 128-bits in binary.
(32 hex digits x 4 bits per hex digit = 128-bits).

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

	std::time_t result = std::time(nullptr);

	std::string castedResult = std::to_string(result);

	std::string lastEight = castedResult.substr(2, 8);

	std::stringstream ss;
	ss << std::hex << lastEight;

	uuid += ss.str();

	for (int i = 0; i < 6; i++) {
		int index = rand() % 16;
		uuid += characters[index];
	}

	uuid += '-';

	for (int i = 0; i < 4; i++) {
		int index = rand() % 16;
		uuid += characters[index];
	}

	uuid += '-';

	for (int i = 0; i < 4; i++) {
		int index = rand() % 16;
		uuid += characters[index];
	}

	uuid += '-';

	for (int i = 0; i < 4; i++) {
		int index = rand() % 16;
		uuid += characters[index];
	}

	uuid += '-';

	for (int i = 0; i < 12; i++) {
		int index = rand() % 16;
		uuid += characters[index];
	}

	return uuid;
}
