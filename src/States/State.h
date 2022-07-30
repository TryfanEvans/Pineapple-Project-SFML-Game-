#include "Camera.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>

#ifndef State_H
	#define State_H

//Full of all the assorted fields that need to be shared between states

class State
{

public:
	sf::RenderWindow& win;
	StateData& stateData;
	Map map;
	Player player;
	Camera camera;

	EntityVec enemies;
	EntityVec items;
	Menu menu;
	State(StateData& stateData, sf::RenderWindow& win) :
		win(win),
		stateData(stateData),
		map(),
		player(),
		camera(win, map),
		enemies("enemy", new EnemyFactory()),
		items("items", new ItemFactory()),
		menu(stateData)
	{
		Solid::map = &map;
	};
	virtual void update(float) = 0;
	virtual void draw() = 0;
	virtual void click(int, int, int) = 0;
	//Refactor this at some point
};

class GameState : public State
{

	Screen death_screen;
	Screen win_screen;

public:
	GameState(StateData& stateData, sf::RenderWindow& win);
	//Populates the world with enemies, possible needing to be refactored into another class/template
	void loadEnemies(std::string level_name);
	//Populates the world with items, possible needing to be refactored into another class/template
	void loadItems(std::string level_name);
	//Handles enemies, items and the player
	void update(float dt);
	//Renders absolutely everything to the screen. Also due for a refactor at some point, so everything inherits from renderable
	void draw();
	//Runs everytime the mouse is clicked
	void click(int x, int y, int button);
};

class EditorState : public State
{

	float view_x;
	float view_y;

public:
	EditorState(StateData&, sf::RenderWindow& win);
	//Allows the player to move and place objects
	void update(float dt);
	//Renders absolutely everything to the screen. Also due for a refactor at some point, so everything inherits from renderable
	void draw();
	//Runs everytime the mouse is clicked

	void click(int x, int y, int button);
};

#endif