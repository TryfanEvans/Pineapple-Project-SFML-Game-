#include "Pellet.h"

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
	Item item;
	item.setPosition(x, y);
	items.push_back(item);
}

void Item::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	sprite.setRadius(8);
	sprite.setFillColor(sf::Color(250, 250, 250));
	sprite.setOrigin(8, 8);
	target->draw(sprite);
}