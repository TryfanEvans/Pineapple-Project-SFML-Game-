#include "State.h"
#include <experimental/filesystem>

EditorState::EditorState(StateData& stateData, sf::RenderWindow& win) :
	State(stateData, win)
{
	player.setGridPosition(2, 2);
}

void EditorState::click(int x, int y, int button)
{
	auto [world_x, world_y] = camera.screenToWorldPos(x, y);
	int map_x = std::floor(world_x / map.tileSize);
	int map_y = std::floor(world_y / map.tileSize);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (button == 1)
		{
			Item item(&map, world_x, world_y);
			items.push_back(item);
		}
		else
		{
			for (uint key = 0; key < items.size(); key++)
			{
				Item value = items[key];
				if (value.contact(map_x * map.tileSize, map_y * map.tileSize))
				{
					items.erase(items.begin() + key);
				}
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (button == 1)
		{
			Enemy* enemy = new Melee(&map, map_x, map_y);
			enemies.push_back(enemy);
		}
		else if (button == 3)
		{
			Enemy* enemy = new Ranged(&map, map_x, map_y);
			enemies.push_back(enemy);
		}
		else if (button == 2)
		{
			for (uint key = 0; key < enemies.size(); key++)
			{
				Enemy* value = enemies[key];
				if (value->contact(map_x * map.tileSize, map_y * map.tileSize))
				{
					delete value;
					enemies.erase(enemies.begin() + key);
				}
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		if (button == 1)
		{
			map.setTile(map_x, map_y, 2);
		}
		else if (button == 2)
		{
			map.setTile(map_x, map_y, 0);
		}
		else if (button == 3)
		{
			map.setTile(map_x, map_y, 3);
		}
	}
	else
	{
		if (button == 1)
		{
			map.setTile(map_x, map_y, 1);
		}
		else
		{
			map.setTile(map_x, map_y, 0);
		}
	}
}

void EditorState::draw()
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
}

void EditorState::update(float dt)
{

	player.update(dt, enemies, items);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;

		std::experimental::filesystem::create_directory("./levels/" + stateData.level_name);

		//Remember to finish the filesystem refactoring
		//Editing other levels breaks this sometimes? WTF
		map.save(stateData.level_name);
		player.save(stateData.level_name);
		//Deletes old enemies
		{
			std::string file_name = "./levels/" + stateData.level_name + "/enemy.txt";

			remove(file_name.c_str());

			//Saves current enemies
			std::ofstream enemyfile;
			enemyfile.open(file_name, std::ios_base::app);

			for (uint key = 0; key < enemies.size(); key++)
			{
				auto [gx, gy] = enemies[key]->getGridPosition();
				enemyfile << gx << " " << gy << "," << enemies[key]->getType() << "\n";
			}
			enemyfile.close();
		}
		{
			//This broke a few times then fixed itself. Keep an eye on this because it's terrifying
			//Deletes old items
			std::string file_name = "./levels/" + stateData.level_name + "/item.txt";

			remove(file_name.c_str());

			//Saves current items
			std::ofstream itemfile;
			itemfile.open(file_name, std::ios_base::app);
			for (uint key = 0; key < items.size(); key++)
			{
				Item value = items[key];
				itemfile << value.x << " " << value.y << "\n";
			}
			itemfile.close();
		}
	}
}