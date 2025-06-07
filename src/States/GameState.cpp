#include "State.h"
#include <experimental/filesystem>

GameState::GameState()
{
	std::cout << "load\n";
	loadLevel(SaveManager::levels[0]);

	pause_menu.addOption("Resume");
	pause_menu.addOption("Controls");
	pause_menu.addSlider("Volume");
	pause_menu.addOption("Quit");
}

GameState::GameState(std::string level_name) :
	State(level_name)
{
	std::cout << "load\n";
	loadLevel(level_name);

	pause_menu.addOption("Resume");
	pause_menu.addOption("Controls");
	pause_menu.addSlider("Volume");
	pause_menu.addOption("Quit");
}

void GameState::update(float dt)
{
	Solid::player = &player;
	Solid::projectiles = &projectiles;
	if (UI_elements.empty())
	{
		Solid::map = &this->map;
		auto [gx, gy] = player.getGridPosition();
		player.update(dt, enemies, items);
		projectiles.update(dt);
		enemies.update(dt);
		map.generatePathfinding(gx, gy);
	}
	else
	{
		UI_elements.top()->update();
	}
}

void GameState::keyPress(int key)
{
	if (key == sf::Keyboard::Escape)
	{
		if (UI_elements.empty())
		{
			//Refactor this to use the events system
			UI_elements.push(&pause_menu);
		}
		else
		{
			UI_elements.pop();
		}
	}
}

void GameState::draw()
{
	camera.set(player.x, player.y);

	map.render(win);
	player.render(win);
	enemies.render(win);
	items.render(win);
	projectiles.render(win);

	if (!UI_elements.empty())
		UI_elements.top()->render(win);
}

void GameState::click(int x, int y, int button)
{
	auto [relative_x, relative_y] = camera.screenToWorldPos(x, y);
	player.action(relative_x, relative_y, button, enemies);
}