#include "State.h"
#include <experimental/filesystem>

EditorState::EditorState(float& music_volume) :
	map("empty"),
	player(&map),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f)),
	menu(&music_volume)
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
			Item item;
			item.tileSize = map.tileSize;
			item.setGridPosition(map_x, map_y);
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
			Enemy* enemy = new Melee(&map, x, y);
			enemies.push_back(enemy);
		}
		else if (button == 3)
		{
			Enemy* enemy = new Ranged(&map, x, y);
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

	player.update(dt, enemies, items, gameover);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;
		std::experimental::filesystem::create_directory("./levels/" + level_name);

		map.save();
		//Remember to finish the filesystem refactoring
		std::ofstream levelfile;
		levelfile.open("level.txt", std::ios_base::app);
		levelfile << "\n";
		for (uint key = 0; key < enemies.size(); key++)
		{
			std::cout << enemies[key]->getType();

			auto [gx, gy] = enemies[key]->getGridPosition();
			levelfile << gx << " " << gy
					  << ","
					  << enemies[key]->getType()
					  << "\n";
		}
		levelfile << "\n";
		for (uint key = 0; key < items.size(); key++)
		{
			Item value = items[key];
			auto [gx, gy] = value.getGridPosition();
			levelfile << gx << " " << gy << "\n";
		}
		levelfile.close();
		auto [gx, gy] = player.getGridPosition();
		std::ofstream playerfile;
		playerfile.open("player.txt");
		playerfile << gx << " " << gy << "\n";
		playerfile.close();
	}
}