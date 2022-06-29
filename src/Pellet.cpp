#include "Pellet.h"

Pellet::Pellet(Map* map) :
	Solid(8)
{
	this->map = map;
}

void Pellet::render(sf::RenderTarget* target)
{
	if (active)
	{
		sprite.setPosition(x, y);
		sprite.setRadius(8);
		sprite.setFillColor(sf::Color(250, 250, 250));
		sprite.setOrigin(8, 8);
		target->draw(sprite);
	}
}

void Pellet::drop(std::vector<Item>& items)
{
	active = false;
	Item item(map, x, y);
	items.push_back(item);
}

Item::Item(Map* map, int x, int y) :
	Solid(8)

{
	this->map = map;
	setPosition(x, y);
}

void Item::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	sprite.setRadius(8);
	sprite.setFillColor(sf::Color(250, 250, 250));
	sprite.setOrigin(8, 8);
	target->draw(sprite);
}