#include "State.h"

GameState::GameState() :
	map("level"),
	player(),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f))
{
	player.setPosition(3, 2, map.tileSize);
	Melee* pineapple = new Melee();
	enemies.push_back(pineapple);

	for (uint key = 0; key < enemies.size(); key++)
	{

		enemies[key]->setPosition(4, 7, map.tileSize);
	}
}

void GameState::load()
{}

void GameState::update(float dt)
{
	player.update(dt, true, map);
	auto [gx, gy] = player.getGridPosition();
	map.generatePathfinding(gx, gy);
	for (uint key = 0; key < enemies.size(); key++)
	{

		enemies[key]->update(dt, player.getX(), player.getY(), &map);
	}
}

void GameState::draw(sf::RenderWindow* win)
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
}

void GameState::click(int x, int y, int button, sf::RenderWindow* win)
{
	std::cout << x << y << button;

	auto [window_width, window_height] = win->getSize();
	float origin_x = std::min(player.x, window_width / 2.f);
	origin_x = std::max(origin_x, window_width / 2.f + player.x - view_x);
	float origin_y = std::min(player.y, window_height / 2.f);
	origin_y = std::max(origin_y, window_height / 2.f + player.y - view_x);
	player.action(x - origin_x, y - origin_y, button, enemies);
}

EditorState::EditorState() :
	map("empty"),
	player(),
	view(sf::FloatRect(0.f, 0.f, 300.f, 300.f))
{
	player.setPosition(2, 2, map.tileSize);
}

void EditorState::load()
{}

void EditorState::click(int x, int y, int button, sf::RenderWindow* win)
{
	auto [window_width, window_height] = win->getSize();
	std::cout << x << y << button;

	int map_x = std::floor((view_x - window_width / 2 + x) / map.tileSize) - 1;
	int map_y = std::floor((view_y - window_height / 2 + y) / map.tileSize) - 1;

	map.setTile(map_x, map_y, 1);
}

void EditorState::draw(sf::RenderWindow* win)
{
	auto [window_width, window_height] = win->getSize();
	auto [view_x, view_y] = this->ViewPosition(player.x, player.y, window_width, window_height, map.grid_width, map.grid_height, map.tileSize);
	view.setCenter(sf::Vector2f(view_x, view_y));
	win->setView(view);

	map.render(win);
	player.render(win);
}

void EditorState::update(float dt)
{
	player.update(dt, false, map);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;
		map.save();
	}
}