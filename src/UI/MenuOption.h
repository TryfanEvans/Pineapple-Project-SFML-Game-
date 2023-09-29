#include "../States/Scripts.h"
#include "./Fonts.h"
#include <SFML/Graphics.hpp>

#ifndef MenuOption_H
	#define MenuOption_H

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