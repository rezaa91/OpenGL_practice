#include "File.h"
#include <fstream>
#include <string>

const std::string File::readFile(const std::string& filename)
{
	std::ifstream myFile;
	myFile.open(filename);

	if (!myFile.is_open())
	{
		std::cerr << "Unable to open file for reading: " << filename << "...\n";
		return "";
	}

	std::string contents = "";
	std::string line;
	while (!myFile.eof())
	{
		std::getline(myFile, line);
		contents += line + '\n';
	}
	myFile.close();
	return contents;
}
