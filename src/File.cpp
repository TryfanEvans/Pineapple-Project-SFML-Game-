#include "File.h"

File::File() :
	file_name("Dungeon")
{}

File::File(std::string file_name) :
	file_name(file_name)
{
}

std::ifstream File::getFile()
{
	std::ifstream loadfile("levels/" + level_name + "/" + file_name + ".txt");
	return (loadfile);
}

void File::setFile(std::string data)
{
	//Deletes old map
	//std::string file_name = "./levels/" + level_name + "/map.txt";
	//remove(file_name.c_str());
	std::ofstream playerfile;
	playerfile.open("./levels/" + level_name + "/" + file_name + ".txt");
	playerfile << data;
	playerfile.close();
}