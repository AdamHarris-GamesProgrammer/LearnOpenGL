#pragma once
#include <string>

class UUID
{
public:
	static void Init();
	static std::string GenerateUUID();

private:
	static std::string GenerateSequence(int length);
};

