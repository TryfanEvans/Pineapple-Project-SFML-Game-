#include "../UI/Menu.h"
#include "Camera.h"
#include "Scripts.h"
#include <SFML/Graphics.hpp>

#ifndef State_H
	#define State_H

//Full of all the assorted fields that need to be shared between states

class Entity;

class EntityVec;

class State
{

public:
	sf::RenderWindow& win;
	Scripts& scripts;
	Map map;
	Player player;
	Camera camera;

	EntityVec enemies;
	EntityVec items;
	EntityVec projectiles;

	State(Scripts& scripts, sf::RenderWindow& win) :
		win(win),
		scripts(scripts),
		map(),
		player(),
		camera(win, map),
		enemies("enemy", new EnemyFactory()),
		items("items", new ItemFactory()),
		projectiles("projectiles", new ItemFactory())
	{
		Solid::map = &map;
		Solid::projectiles = &projectiles;
		Solid::enemies = &enemies;
		Solid::items = &items;
		Solid::player = &player;

		Scripts::map = &map;
		Scripts::projectiles = &projectiles;
		Scripts::enemies = &enemies;
		Scripts::items = &items;
		Scripts::player = &player;
	};

	virtual void update(float) = 0;
	virtual void draw() = 0;
	virtual void click(int, int, int) = 0;
	virtual void keyPress() = 0;
	//Refactor this at some point
};

class GameState : public State
{
public:
	GameState(Scripts& scripts, sf::RenderWindow& win);
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

	void keyPress() {};
};

class EditorState : public State
{

	float view_x;
	float view_y;

public:
	EditorState(Scripts&, sf::RenderWindow& win);
	//Allows the player to move and place objects
	void update(float dt);
	//Renders absolutely everything to the screen. Also due for a refactor at some point, so everything inherits from renderable
	void draw();
	//Runs everytime the mouse is clicked

	void click(int x, int y, int button);
	void keyPress() {};
};

//class TitleScreen : public State
//{
//public:
//	TitleScreen();
//	void update(float dt);
//	void draw();
//	void click(int x, int y, int button);
//	Menu title_menu;
//}

#endif