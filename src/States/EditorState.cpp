#include "State.h"
#include <experimental/filesystem>

EditorState::EditorState()
{
	player.setGridPosition(2, 2);
}

EditorState::EditorState(std::string level_name, int width, int height) : State(level_name)
{
	std::cout << width << "\t" << height << "\n";
	player.setGridPosition(2, 2);
	loadLevel(level_name);
}

void EditorState::click(int x, int y, int button)
{
	auto [world_x, world_y] = camera.screenToWorldPos(x, y);
	int map_x = std::floor(world_x / map.tileSize);
	int map_y = std::floor(world_y / map.tileSize);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (button == 1)
		{
			items.add("any", world_x, world_y);
		}
		else
		{
			items.remove(items.getAtPosition(world_x, world_y));
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (button == 1)
		{
			enemies.add("Melee", map_x, map_y);
		}
		else if (button == 3)
		{
			enemies.add("Ranged", map_x, map_y);
		}
		else if (button == 2)
		{
			enemies.remove(enemies.getAtPosition(world_x, world_y));
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
	{
		if (button == 1)
		{
			map.setTile(map_x, map_y, 2);
		}
		else if (button == 2)
		{
			map.setTile(map_x, map_y, 4);
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

void EditorState::draw()
{
	camera.set(player.x, player.y);
	map.render(win);
	player.render(win);
	enemies.render(win);
	items.render(win);
}

void EditorState::update(float dt)
{

	player.update(dt, enemies, items);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		std::cout << "save" << std::endl;
		File::level_name = "Dungeon";
		std::experimental::filesystem::create_directory("./levels/" + File::level_name);

		//Remember to finish the filesystem refactoring
		//Editing other levels breaks this sometimes? WTF
		map.save();
		player.save();
		enemies.save();
		items.save();
	}
}