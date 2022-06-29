#include "State.h"
#include <experimental/filesystem>

EditorState::EditorState(StateData& stateData) :
	State(stateData),
	map(),
	player(&map),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f)),
	menu(stateData)
{
	player.setGridPosition(2, 2);
}

void EditorState::click(int x, int y, int button, sf::RenderWindow* win)
{
	auto [window_width, window_height] = win->getSize();
	auto [view_x, view_y] = ViewPosition(player.x, player.y, window_width, window_height, map.grid_width, map.grid_height, map.tileSize);

	int map_x = std::floor((view_x - window_width / 2 + x) / map.tileSize);
	int map_y = std::floor((view_y - window_height / 2 + y) / map.tileSize);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (button == 1)
		{
			Item item(&map, x - map.tileSize, y - map.tileSize);
			items.push_back(item);
		}
		else
		{
			for (uint key = 0; key < items.size(); key++)
			{
				Item& value = items[key];
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

void EditorState::draw(sf::RenderWindow* win)
{
	auto [window_width, window_height] = win->getSize();
	auto [view_x, view_y] = this->ViewPosition(player.x, player.y, window_width, window_height, map.grid_width, map.grid_height, map.tileSize);
	view.setCenter(sf::Vector2f(view_x, view_y));
	win->setView(view);

	map.render(win);
	player.render(win);
	for (uint key = 0; key < enemies.size(); key++)
	{
		Enemy* value = enemies[key];
		value->render(win);
	}
	for (uint key = 0; key < items.size(); key++)
	{
		Item& value = items[key];
		value.render(win);
	}
}

void EditorState::update(float dt, sf::Window&)
{

	player.update(dt, enemies, items);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;

		std::experimental::filesystem::create_directory("./levels/" + level_name);

		//Remember to finish the filesystem refactoring
		//Editing other levels breaks this sometimes? WTF
		map.save(level_name);

		//Deletes old enemies
		{
			std::string file_name = "./levels/" + level_name + "/enemy.txt";

			remove(file_name.c_str());

			//Saves current enemies
			std::ofstream enemyfile;
			enemyfile.open(file_name, std::ios_base::app);

			for (uint key = 0; key < enemies.size(); key++)
			{
				std::cout << enemies[key]->getType() << " is the type \n";

				auto [gx, gy] = enemies[key]->getGridPosition();
				enemyfile << gx << " " << gy << "," << enemies[key]->getType() << "\n";
			}
			enemyfile.close();
		}
		{
			//This broke a few times then fixed itself. Keep an eye on this because it's terrifying
			//Deletes old items
			std::string file_name = "./levels/" + level_name + "/item.txt";

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
		auto [gx, gy] = player.getGridPosition();
		std::ofstream playerfile;
		playerfile.open("./levels/" + level_name + "/player.txt");
		playerfile << gx << " " << gy << "\n";
		playerfile.close();
	}
}