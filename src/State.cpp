#include "State.h"

GameState::GameState() :
	map("level"),
	player(),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f))
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
			float x = std::stoi(line.substr(0, space + 1));
			float y = std::stoi(line.substr(space));
			Melee* pineapple = new Melee();
			pineapple->setPosition(x, y, map.tileSize);
			enemies.push_back(pineapple);
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
}

void GameState::update(float dt)
{
	player.update(dt, map, enemies, items);
	auto [gx, gy] = player.getGridPosition();
	map.generatePathfinding(gx, gy);
	for (uint key = 0; key < enemies.size(); key++)
	{

		enemies[key]->update(dt, player.getX(), player.getY(), &map);
	}

	//Stops the player from having to kill already cleared enemies after respawing, runs once after the first frame
	static bool cleared = false;
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
}

void GameState::click(int x, int y, int button, sf::RenderWindow* win)
{

	auto [window_width, window_height] = win->getSize();
	auto [view_x, view_y] = ViewPosition(player.x, player.y, window_width, window_height, map.grid_width, map.grid_height, map.tileSize);
	float origin_x = player.x - view_x + (window_width / 2);
	float origin_y = player.y - view_y + (window_height / 2);
	player.action(x - origin_x, y - origin_y, button, enemies);
}

EditorState::EditorState() :
	map("empty"),
	player(),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f))
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
			enemies.push_back(enemy);
		}
		else
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
		else
		{
			map.setTile(map_x, map_y, 0);
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

void EditorState::update(float dt)
{

	player.update(dt, map, enemies, items);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;
		map.save();
		std::ofstream levelfile;
		levelfile.open("level.txt", std::ios_base::app);
		levelfile << "\n";
		for (uint key = 0; key < enemies.size(); key++)
		{
			Enemy* value = enemies[key];
			auto [gx, gy] = value->getGridPosition();
			levelfile << gx << " " << gy << "\n";
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