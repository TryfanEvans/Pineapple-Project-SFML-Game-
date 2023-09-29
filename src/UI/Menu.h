#include "./States/Scripts.h"
#include "Screen.h"

class Scripts;

#ifndef Menu_H
	#define Menu_H

	#include "MenuOption.h"
class Menu
{
public:
	Scripts& scripts;

	sf::RectangleShape sprite;
	//Could this be an entityVec?
	std::vector<MenuOption*> options;
	sf::View view;

	float relative_mouse_x = 0;
	float relative_mouse_y = 0;

	float option_width = 150;
	float option_height = 30;
	Menu(Scripts& scripts);
	//Renders the pause_menu with all of the options and screens
	void render(sf::RenderWindow* win);
	//Handles player input within the pause_menu
	void update(sf::Window& win);
	//When the player presses escape navigate through subscreens and the pause_menu
	void toggle();
	//Escapes from the pause_menu
	void resume();
	//Used in a constructor
	void addOption(std::string label);
	void addSlider(std::string label);
};
#endif