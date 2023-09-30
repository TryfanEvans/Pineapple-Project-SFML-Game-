#include "../States/Scripts.h"
#include "./Fonts.h"
#include <SFML/Graphics.hpp>

#ifndef MenuOption_H
	#define MenuOption_H

static sf::Texture cross;

class Checkbox
{
public:
	sf::Sprite sprite;

	bool checked = false;
	bool hover = false;
	Checkbox()
	{
		//This might break when I make two
		cross.loadFromFile("content/cross.png");
		sprite.setTexture(cross);
	};

	void update(float x, float y)
	{
		//How u gonna click on two at once? Dummy
		static bool flag = false;

		hover = false;
		//This will execute when the cursor starts outside the zone but otherwise it's good
		if (getSelected(x, y))
		{
			hover = true;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				flag = true;
			}
			else if (flag)
			{
				flag = false;
				toggle();
			}
		}
		else
			flag = false;
	};

	sf::RectangleShape box;
	float width = 10;
	float height = 10;
	void render(sf::RenderWindow* win, float x, float y, float width, float height)
	{
		box.setFillColor(sf::Color(100, 100, 100));

		box.setPosition(sf::Vector2f(x, y));
		box.setSize(sf::Vector2f(width, height));

		win->draw(box);

		box.setOutlineThickness(0);
		if (hover)
		{
			box.setOutlineThickness(-2);
		}
		if (checked)
		{
			//Add an X to be shown when checked
			sprite.setPosition(sf::Vector2f(x, y));
			sprite.setScale(sf::Vector2f(0.5, 0.5));
			win->draw(sprite);
		}
	};
	//void set(bool checked);
	void toggle()
	{
		checked = !checked;
	};

	bool getSelected(float x, float y)
	{
		auto position = box.getPosition();
		auto size = box.getSize();
		return (x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y);
	};
};

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
	Checkbox checkbox;
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