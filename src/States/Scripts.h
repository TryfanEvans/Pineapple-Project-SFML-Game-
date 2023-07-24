//Had this idea that level could be a namespace/class all of it's own. Would contain level_name, load and save functions
//Potentially everything that is loaded from the files, player, enemies, items and the map
//The gamestate could access the level name just fine, just go Level::name

#ifndef Scripts_H
#define Scripts_H
#include "Collision.h"
#include "Entity.h"
#include "Menu.h"
#include "Player.h"

//Scripts to manage events happening within the level and giving the appropriate response.
class Scripts
{
public:
	//RED AMD WHITE
	static EntityVec* projectiles;
	static EntityVec* enemies;
	static EntityVec* items;
	static Player* player;
	static Map* map;

	//Init
	static std::vector<std::string> levels;
	uint level_index = 0;
	std::string action_pending = "";

	//Win and loss states
	static bool gameover;
	bool dead = false;

	//Screens
	Screen* screen;
	Screen death_screen;
	Screen win_screen;
	bool show_screen = false;

	//Settings
	float music_volume = 0.0f;
	float sfx_volume = 0.5f; //Not currently implemented
	bool paused = false;
	//Probably something to do with keybindings, also not implemented
	std::string graphics_quality = "low"; //Will probably never be implemented

	Scripts() :
		death_screen("death_screen"),
		win_screen("win_screen")
	{}

	//TODO: boolean return for success/failure
	void
	loadLevel(std::string level_name)
	{
		File::level_name = level_name;
		map->load();
		player->load();
		enemies->load();
		items->load();
	}

	//This needs to be specific to each level
	bool levelCleared()
	{
		if (File::level_name == "Dungeon")
			return Scripts::gameover;
		return enemies->getSize() == 0;
	}

	void nextLevel()
	{
		//Later gameover will require the grail make gameover true
		level_index++;
		if (level_index == levels.size())
			std::cout << "Find the grail!";
		else
			loadLevel(levels[level_index]);
	}

	void death()
	{
		dead = true;
		screen = &death_screen;
		show_screen = true;
	}

	void win()
	{
		gameover = true;
		screen = &win_screen;
		show_screen = true;
	}

	void update()
	{
		if (levelCleared())
		{
			nextLevel();
		}

		if (dead == true)
		{
			death();
		}

		if (gameover == true)
		{
			win();
		}

		if (paused and show_screen and sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			show_screen = false;
		}

		action_pending = "";
	}
};

#endif