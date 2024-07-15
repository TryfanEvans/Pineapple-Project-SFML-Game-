#include "../States/Scripts.h"
#ifndef Screen_H
	#define Screen_H

class Screen
{
public:
	sf::View view;
	sf::Texture texture;
	sf::RectangleShape sprite;
	std::string label;
	Screen(std::string image_name);
	//Covers the pause_menu with an image
	//Refactor this to not use the sf::View
	void render(sf::RenderTarget* win);
	void update();
};

#endif