#include "State.h"
#include <experimental/filesystem>

GameState::GameState()
{
	std::cout << "load\n";
	loadLevel(SaveManager::levels[0]);
}

void GameState::update(float dt)
{
	Solid::map = &this->map;
	auto [gx, gy] = player.getGridPosition();
	player.update(dt, enemies, items);
	projectiles.update(dt);
	enemies.update(dt);

	map.generatePathfinding(gx, gy);
}

void GameState::draw()
{
	camera.set(player.x, player.y);

	map.render(win);
	player.render(win);
	enemies.render(win);
	items.render(win);
	projectiles.render(win);

	//Don't want either of these during development
}

void GameState::click(int x, int y, int button)
{
	auto [relative_x, relative_y] = camera.screenToWorldPos(x, y);
	player.action(relative_x, relative_y, button, enemies);
}