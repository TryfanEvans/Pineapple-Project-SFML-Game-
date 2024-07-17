#include "../UI/Menu.h"
#include "Camera.h"
#include "Scripts.h"
#include "UI/UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#ifndef State_H
	#define State_H

//Full of all the assorted fields that need to be shared between states

class Entity;

class EntityVec;

class State
{

public:
	static sf::RenderWindow* win;
	Map map;
	Player player;
	Camera camera;

	EntityVec enemies;
	EntityVec items;
	EntityVec projectiles;

	std::stack<UI*> UI_elements;
	Menu pause_menu;

	State() :
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
	};

	void loadLevel(std::string level_name)
	{
		File::level_name = level_name;
		map.load();
		player.load();
		enemies.load();
		items.load();
	}

	virtual void update(float) = 0;
	virtual void draw() = 0;
	virtual void click(int, int, int) = 0;
	virtual void keyPress(int) {};
	//Refactor this at some point
};

class GameState : public State
{
public:
	GameState();
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

	void keyPress(int);
};

class EditorState : public State
{

	float view_x;
	float view_y;

public:
	EditorState();
	//Allows the player to move and place objects
	void update(float dt);
	//Renders absolutely everything to the screen. Also due for a refactor at some point, so everything inherits from renderable
	void draw();
	//Runs everytime the mouse is clicked

	void click(int x, int y, int button);
	void keyPress() {};
};

class TitleState : public State
{
public:
	TitleState() :
		background(Screen("title_screen"))
	{
		start_menu = Menu();
		start_menu.addOption("Continue");
		start_menu.addOption("New Game");
		start_menu.addOption("Load Game");
		start_menu.addOption("Level Editor");

		//If any two options have the same label, they do the same thing
		start_menu.addOption("Quit");
	};

	void draw()
	{
		camera.set(0, 0);
		background.render(win);
		start_menu.render(win);
	};

	void update(float)
	{
		start_menu.update();
	};

	void click(int, int, int) {};
	void keyPress() {};

	Screen background;
	Menu start_menu;
};

class EditorMenuState : public State
{
public:
	EditorMenuState() :
		background(Screen("default_screen")) {

		};

	void draw()
	{
		camera.set(0, 0);
		background.render(win);
	};

	void update(float) {};

	void click(int, int, int) {};
	void keyPress() {};

	Screen background;
};

class SaveMenuState : public State
{
public:
	SaveMenuState() :
		background(Screen("default_screen")) {

		};

	void draw()
	{
		camera.set(0, 0);
		background.render(win);
	};

	void update(float) {};

	void click(int, int, int) {};
	void keyPress() {};

	Screen background;
};

#endif