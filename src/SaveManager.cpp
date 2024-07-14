#include "SaveManager.h"

namespace SaveManager
{

std::vector<std::string> levels = { "Arena", "Ballerina", "Dungeon" };
uint level_unlock = 0;
uint level_index = 0;

void newGame()
{
	level_index = 0;
}

//Load current level_unlock from file
void continueGame()
{
	std::ifstream loadfile("saves/Original.txt");

	std::string line;
	std::getline(loadfile, line);
	level_unlock = std::stoi(line);
	level_index = 0;
	loadfile.close();
}

void loadGame(uint level)
{
	if (level <= level_unlock)
	{
		level_index = level;
	}
}

void completeLevel()
{
	level_index++;
	level_unlock = std::max(level_unlock, level_index);
	if (level_unlock == levels.size())
	{
		std::cout << "game complete!";
	}

	std::ofstream playerfile;
	playerfile.open("saves/Original.txt");
	playerfile << std::to_string(level_unlock);
	playerfile.close();
}

}