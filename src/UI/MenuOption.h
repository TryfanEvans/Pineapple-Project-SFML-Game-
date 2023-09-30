#include "../States/Scripts.h"
#include "./Fonts.h"
#include <SFML/Graphics.hpp>

#ifndef MenuOption_H
	#define MenuOption_H

//class Checkbox
//{
//public:
//	bool checked;
//	Checkbox();
//	void update();
//	//void set(bool checked);
//	void toggle();
//	void getSelected();
//};

class MenuOptionBackpanel
{
public:
	bool hover = false;
	sf::RectangleShape sprite;
	float width = 100;
	float height = 40;
	MenuOptionBackpanel();
	void render(sf::RenderWindow* win, float x, float y, float width, float height);
	void update(float mouse_x, float mouse_y);
	bool getSelected(float x, float y);
};

class MenuOption
{
public:
	MenuOptionBackpanel backpanel;
	std::string label;
	float width = 100;
	float height = 40;
	MenuOption(std::string label);
	MenuOption();
	//Renders the individual option to the screen, showing highlighting when selected and the label
	virtual void render(sf::RenderWindow* win, float x, float y, float width, float height);
	//Determines if the mouse is hovering over this option

	//Handles mouse clicks
	virtual void update(float mouse_x, float mouse_y);
	//There is no slider option, in fact it's a bit weird the code is set up like this
	//Seems like a refactor where both function and slider based options inherit from a common class would work better
	//In fact is it even necessary taht this exists anyway? Seems like I could straight up remove it

	//Violation of litzeschovs subsitution principle
	//Not gonna fuck with this shiiiiiit
};

class MenuButton : public MenuOption
{
};
class MenuSlider : public MenuOption
{
public:
	//Checkbox checkbox;
	MenuSlider(std::string label);
	//Converts the value of bound_var to a renderable position
	float slider_position;
	//Renders to the screen with location depending on the value of bound_var
	void render(sf::RenderWindow* win, float x, float y, float width, float height);
	//Makes the slider position match the mouse, within bounds
	void setSliderPosition(float mouse_x);
	void update(float mouse_x, float mouse_y);
};

#endif