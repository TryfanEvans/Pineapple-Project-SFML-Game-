#include "Screen.h"

Screen::Screen(std::string image_name) :
	view(sf::FloatRect(0.f, 0.f, 600.f, 600.f))
{
	label = image_name;
	if (!texture.loadFromFile("content/" + image_name + ".png"))
	{
		// error...
	}
}

void Screen::render(sf::RenderTarget* win)
{
	auto view_position = view.getCenter();
	auto view_size = view.getSize();
	//Gets the position of the top left corner
	view_position.x -= view_size.x / 2;
	view_position.y -= view_size.y / 2;

	sprite.setPosition(view_position.x, view_position.y);
	sprite.setSize(sf::Vector2f(view_size));
	sprite.setTexture(&texture);
	sprite.setOutlineThickness(-2);
	win->setView(view);
	win->draw(sprite);
}

void Screen::update()
{
	Scripts::actions_pending.push(label);
}
