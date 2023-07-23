#include "State.h"
#include <experimental/filesystem>

GameState::GameState(Scripts& scripts, sf::RenderWindow& win) :
	State(scripts, win),
	death_screen("death_screen"),
	win_screen("win_screen")
{
	std::cout << "load\n";
	scripts.loadLevel(scripts.levels[0]);
}

static bool mouse_enabled = true;
void GameState::update(float dt)
{
	menu.toggle();

	if (scripts.paused)
	{
		menu.update(win);
		mouse_enabled = false;
	}
	else if (!scripts.dead && !scripts.gameover)
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			mouse_enabled = true;
		}
		auto [gx, gy] = player.getGridPosition();
		player.update(dt, enemies, items);
		projectiles.update(dt);
		enemies.update(dt);
		scripts.update();

		map.generatePathfinding(gx, gy);
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
	if (scripts.dead)
	{
		death_screen.render(&win, camera.view);
	}
	if (scripts.gameover)
	{
		win_screen.render(&win, camera.view);
	}
	if (scripts.paused)
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