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

	std::vector<Enemy*> enemies;
	std::vector<Item> items;
	Menu menu;
	State(StateData& stateData, sf::RenderWindow& win) :
		win(win),
		stateData(stateData),
		map(),
		player(&map),
		camera(win, map),
		menu(stateData) {};
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
	void loadEnemies(std::string level_name);
	void loadItems(std::string level_name);
	void update(float dt);
	void draw();
	//Runs everytime the mouse is clicked
	void click(int x, int y, int button);
};

class EditorState : public State
{

	std::vector<Enemy*> enemies;
	std::vector<Item> items;

	float view_x;
	float view_y;

public:
	EditorState(StateData&, sf::RenderWindow& win);
	void update(float dt);
	void draw();
	//Runs everytime the mouse is clicked

	void click(int x, int y, int button);
};

#endif