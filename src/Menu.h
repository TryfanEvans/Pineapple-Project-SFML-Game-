#include "./States/Scripts.h"

class Scripts;

#ifndef Menu_H
	#define Menu_H
class Screen
{
public:
	sf::Texture texture;
	sf::RectangleShape sprite;
	std::string label;
	Screen(std::string image_name);
	//Covers the pause_menu with an image
	void render(sf::RenderTarget* win, sf::View view);
	void update();
};

class MenuOption
{
public:
	sf::RectangleShape sprite;
	std::string label;
	float width = 100;
	float height = 40;
	MenuOption(std::string label);
	MenuOption();
	//Renders the individual option to the screen, showing highlighting when selected and the label
	virtual void render(sf::RenderWindow* win, float x, float y, float width, float height);
	//Determines if the mouse is hovering over this option
	bool getSelected(float x, float y);

	//Handles mouse clicks
	virtual void update(float mouse_x, float mouse_y);
	//There is no slider option, in fact it's a bit weird the code is set up like this
	//Seems like a refactor where both function and slider based options inherit from a common class would work better
	//In fact is it even necessary taht this exists anyway? Seems like I could straight up remove it
	virtual void setSliderPosition(float) {};
};

class MenuSlider : public MenuOption
{
public:
	MenuSlider(std::string label, float* bound_var);
	float* bound_var;
	//Converts the value of bound_var to a renderable position
	float slider_position;
	//Renders to the screen with location depending on the value of bound_var
	void render(sf::RenderWindow* win, float x, float y, float width, float height);
	//Makes the slider position match the mouse, within bounds
	void setSliderPosition(float mouse_x);
	void update(float mouse_x, float mouse_y);
};

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
};
#endif