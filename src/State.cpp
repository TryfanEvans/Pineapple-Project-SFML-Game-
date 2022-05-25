#include "State.h"

static bool cleared = false;

GameState::GameState(float& music_volume) :
	map("level"),
	player(),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f)),
	menu(&music_volume),
	death_screen("death_screen"),
	win_screen("win_screen")
{
	std::cout << "load"
			  << "\n";
	std::ifstream loadfile("level.txt");
	std::string line;
	int section = 0;
	while (std::getline(loadfile, line))
	{
		if (section == 1 && !line.empty())
		{
			int space = line.find(" ");
			int comma = line.find(",");
			float x = std::stoi(line.substr(0, space + 1));
			float y = std::stoi(line.substr(space, comma));
			std::string type = line.substr(comma + 1);
			std::cout << line << "\n"
					  //	  << type << "\n "
					  << y << "\n ";

			Enemy* enemy = new Ranged();
			if (type == "Melee")
			{
				enemy = new Melee();
			}
			enemy->setPosition(x, y, map.tileSize);
			enemies.push_back(enemy);
		}
		if (section == 2 && !line.empty())
		{
			float x = line.at(0);
			float y = line.at(2);
			Item item;
			item.setPosition(x, y, map.tileSize);
			items.push_back(item);
		}
		if (line == "")
		{
			section++;
		}
	}
	cleared = false;
}

static bool mouse_enabled = true;
void GameState::update(float dt, sf::Window& win)
{
	menu.checkPaused();

	if (menu.paused)
	{
		menu.update(win);
		mouse_enabled = false;
	}
	else if (!dead && !gameover)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			mouse_enabled = true;
		}
		player.update(dt, map, enemies, items, gameover);
		auto [gx, gy] = player.getGridPosition();
		map.generatePathfinding(gx, gy);
		for (uint key = 0; key < enemies.size(); key++)
		{
			enemies[key]->update(dt, player.getX(), player.getY(), &map, dead);
		}

		//Stops the player from having to kill already cleared enemies after respawing, runs once after the first frame
		while (!cleared)
		{
			cleared = true;
			for (uint key = 0; key < enemies.size(); key++)
			{
				if (!(enemies[key]->getObstructed(player.getX(), player.getY(), &map)))
				{
					enemies.erase(enemies.begin() + key);
					cleared = false;
				}
			}
		}
	}
}

void GameState::draw(sf::RenderWindow* win)
{
	auto [window_width, window_height] = win->getSize();

	auto [view_x, view_y] = ViewPosition(player.x, player.y, window_width, window_height, map.grid_width, map.grid_height, map.tileSize);
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

	if (dead)
	{
		death_screen.render(win, view);
	}
	if (gameover)
	{
		win_screen.render(win, view);
	}
	if (menu.paused)
	{
		menu.render(win);
	}
}

void GameState::click(int x, int y, int button, sf::RenderWindow* win)
{
	if (mouse_enabled)
	{
		auto [window_width, window_height] = win->getSize();
		auto [view_x, view_y] = ViewPosition(player.x, player.y, window_width, window_height, map.grid_width, map.grid_height, map.tileSize);
		float origin_x = player.x - view_x + (window_width / 2);
		float origin_y = player.y - view_y + (window_height / 2);
		player.action(x - origin_x, y - origin_y, button, enemies);
	}
}

EditorState::EditorState(float& music_volume) :
	map("empty"),
	player(),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f)),
	menu(&music_volume)
{
	player.setPosition(2, 2, map.tileSize);
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
			item.setPosition(map_x, map_y, map.tileSize);
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
			Enemy* enemy = new Melee;
			enemy->tileSize = map.tileSize;
			enemy->setPosition(map_x, map_y, map.tileSize);
			enemy->type = "Melee";

			enemies.push_back(enemy);
		}
		else if (button == 3)
		{
			Enemy* enemy = new Ranged;
			enemy->tileSize = map.tileSize;
			enemy->type = "Ranged";
			enemy->setPosition(map_x, map_y, map.tileSize);
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
		std::cout << button;
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

	player.update(dt, map, enemies, items, gameover);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;
		map.save();
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