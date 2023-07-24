#include "State.h"
#include <experimental/filesystem>

GameState::GameState(Scripts& scripts, sf::RenderWindow& win) :
	State(scripts, win)
{
	std::cout << "load\n";
	scripts.loadLevel(scripts.levels[2]);
}

void GameState::update(float dt)
{
	if (!scripts.paused && !scripts.dead && !scripts.gameover)
	{
		auto [gx, gy] = player.getGridPosition();
		player.update(dt, enemies, items);
		projectiles.update(dt);
		enemies.update(dt);

		map.generatePathfinding(gx, gy);
	}

	scripts.update();
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
	if (scripts.show_screen)
	{
		scripts.screen->render(&win, camera.view);
	}
}

void GameState::click(int x, int y, int button)
{
	auto [relative_x, relative_y] = camera.screenToWorldPos(x, y);
	player.action(relative_x, relative_y, button, enemies);
}