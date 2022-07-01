#include "State.h"
#include <experimental/filesystem>

//Cool idea - Enemies and Item vectors are so similar they could be templates
//The difference in each load/save file could be compartmentalised into the Templated class?
//while (std::getline(loadfile, line))
//{
//	T* spawn = T->spawnFromText() //Static function, returns T on the heap
//	vector.push_back(T);
//}
void GameState::loadEnemies(std::string level_name)
{
	std::ifstream loadfile("./levels/" + level_name + "/enemy.txt");
	std::string line;
	while (std::getline(loadfile, line))
	{
		int space = line.find(" ");
		int comma = line.find(",");
		float x = std::stoi(line.substr(0, space + 1));
		float y = std::stoi(line.substr(space, comma));
		std::string type = line.substr(comma + 1);

		//This could be a method of the enemies class
		if (type == "Melee")
		{
			Enemy* enemy = new Melee(&map, x, y);
			enemies.push_back(enemy);
		}
		else if (type == "Ranged")
		{
			Enemy* enemy = new Ranged(&map, x, y);
			enemies.push_back(enemy);
		}
		else
		{
			std::cout << type << " is not a valid enemy type!\n";
		}
	}
}

void GameState::loadItems(std::string level_name)
{
	std::ifstream loadfile("./levels/" + level_name + "/item.txt");
	std::string line;
	while (std::getline(loadfile, line))
	{
		int space = line.find(" ");
		float x = std::stoi(line.substr(0, space + 1));
		float y = std::stoi(line.substr(space));
		Item item(&map, x, y);
		items.push_back(item);
	}
}

static bool cleared = false;
GameState::GameState(StateData& stateData, sf::RenderWindow& win) :
	State(stateData, win),
	death_screen("death_screen"),
	win_screen("win_screen")
{
	std::cout << "load\n";
	map.load(stateData.level_name);
	player.load(stateData.level_name);
	loadEnemies(stateData.level_name);
	loadItems(stateData.level_name);

	cleared = false;
}

static bool mouse_enabled = true;
void GameState::update(float dt)
{
	menu.toggle();

	if (stateData.paused)
	{
		menu.update(win);
		mouse_enabled = false;
	}
	else if (!stateData.dead && !stateData.gameover)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			mouse_enabled = true;
		}
		player.update(dt, enemies, items);
		auto [gx, gy] = player.getGridPosition();
		map.generatePathfinding(gx, gy);
		for (uint key = 0; key < enemies.size(); key++)
		{
			enemies[key]->update(dt, player.getX(), player.getY(), stateData.dead);
		}

		//Stops the player from having to kill already cleared enemies after respawing, runs once after the first frame

		cleared = true; //This is temporary, for development purposes
		while (!cleared)
		{
			cleared = true;
			for (uint key = 0; key < enemies.size(); key++)
			{
				if (!(enemies[key]->getObstructed(player.getX(), player.getY())))
				{
					enemies.erase(enemies.begin() + key);
					cleared = false;
				}
			}
		}
	}
}

void GameState::draw()
{
	camera.set(player.x, player.y);

	map.render(&win);

	player.render(&win);
	for (uint key = 0; key < enemies.size(); key++)
	{
		Enemy* value = enemies[key];
		value->render(&win);
	}
	for (uint key = 0; key < items.size(); key++)
	{
		Item& value = items[key];
		value.render(&win);
	}
	//Don't want either of these during development
	if (stateData.dead)
	{
		//	death_screen.render(&win);
	}
	if (stateData.gameover)
	{
		//	win_screen.render(&win);
	}
	if (stateData.paused)
	{
		menu.render(&win);
	}
}

void GameState::click(int x, int y, int button)
{
	if (mouse_enabled)
	{
		auto [origin_x, origin_y] = camera.worldToScreenPos(player.x, player.y);
		player.action(x - origin_x, y - origin_y, button, enemies);
	}
}