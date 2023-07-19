#include "State.h"
#include <experimental/filesystem>

GameState::GameState(StateData& stateData, sf::RenderWindow& win) :
	State(stateData, win),
	death_screen("death_screen"),
	win_screen("win_screen")
{
	std::cout << "load\n";
	map.load();
	player.load();
	enemies.load();
	items.load();
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
		projectiles.update(dt);
		enemies.update(dt);

		auto [gx, gy] = player.getGridPosition();
		map.generatePathfinding(gx, gy);
	}
	if (File::level_name == "Arena")
	{
		if (false)
		{
			std::cout << "Beat the arena!\n";
			File::level_name = "Dungeon";
			map.load();
			player.load();
			enemies.load();
			items.load();
		}
	}
}

void GameState::draw()
{
	camera.set(player.x, player.y);

	map.render(&win);
	player.render(&win);
	enemies.render(&win);
	items.render(&win);
	projectiles.render(&win);

	//Don't want either of these during development
	if (stateData.dead)
	{
		death_screen.render(&win, camera.view);
	}
	if (stateData.gameover)
	{
		win_screen.render(&win, camera.view);
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
		auto [relative_x, relative_y] = camera.screenToWorldPos(x, y);
		player.action(relative_x, relative_y, button, enemies);
	}
}