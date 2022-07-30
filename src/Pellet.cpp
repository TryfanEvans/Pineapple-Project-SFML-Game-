#include "Pellet.h"

Pellet::Pellet() :
	Solid(8)
{
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

void Pellet::drop(EntityVec& items)
{
	active = false;
	items.add("any", x, y);
}

void Pellet::toss(int x, int y)
{
	stored = false;
	setPosition(x, y);
	active = true;
	charge_progress = 0;
}

void Pellet::update(float dt, int tx, int ty)
{
	if (active)
	{
		launch(tx, ty, 1600, dt);

		//if (contact(player_x, player_y))
		//{
		//	std::cout << "shot the player, gosh!";
		//	dead = true;
		//}

		if ((resolveCollision()) || (vx == 0 && vy == 0))
		{
			active = false;
		}
	}
}

Item::Item(int x, int y) :
	Entity(8)
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

std::string Item::serialise()
{
	return std::string(std::to_string(x) + " " + std::to_string(y) + "\n");
}

Entity* ItemFactory::deserialise(std::string line)
{
	int space = line.find(" ");
	float x = std::stoi(line.substr(0, space + 1));
	float y = std::stoi(line.substr(space));

	//This could be a method of the enemies class
	return new Item(x, y);
}