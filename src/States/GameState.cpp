#include "State.h"
#include <experimental/filesystem>

GameState::GameState(sf::RenderWindow* win) :
	State(win)
{
	std::cout << "load\n";
	loadLevel(Scripts::levels[2]);
}

void GameState::update(float dt)
{
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