
#ifndef FILE_H
#define FILE_H

//What if this was a nested class in every savable item, each having a file_name and a shared static level_name? Holy shit I'm so pleased with this idea
class File
{
public:
	std::string static level_name;
	std::string file_name;

	File(std::string file_name);
	File();

	std::ifstream getFile();

	void setFile(std::string data);
};

#endif