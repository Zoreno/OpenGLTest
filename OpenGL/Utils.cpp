#include "Utils.h"

#include <fstream>
#include <streambuf>

std::string getStringFromFile(const std::string& path)
{
	std::ifstream fileStream(path);
	return std::string{ std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>() };
}