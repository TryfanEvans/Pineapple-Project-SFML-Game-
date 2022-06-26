#include "State.h"
#include <experimental/filesystem>

static bool cleared = false;
GameState::GameState(float& music_volume) :
	map("level"),
	player(&map),
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
				std::cout << "Not a valid enemy type!\n";
			}
		}
		if (section == 2 && !line.empty())
		{
			float x = line.at(0);
			float y = line.at(2);
			Item item;
			item.setGridPosition(x, y);
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
		player.update(dt, enemies, items, gameover);
		auto [gx, gy] = player.getGridPosition();
		map.generatePathfinding(gx, gy);
		for (uint key = 0; key < enemies.size(); key++)
		{
			enemies[key]->update(dt, player.getX(), player.getY(), dead);
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