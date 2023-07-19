#include "Projectile.h"

Projectile::Projectile(int x, int y, int tx, int ty, Entity* parent) :
	Entity(8),
	tx(tx),
	ty(ty),
	parent(parent)
{
	setPosition(x, y);

	//Probably going to pare these down
	active = true;
	charge_progress = 0;
}

void Projectile::render(sf::RenderTarget* target)
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

void Projectile::drop()
{
	active = false;
	items->add("any", x, y);
	state = "dead";
}

void Projectile::toss(int x, int y)
{
	setPosition(x, y);
	active = true;
	charge_progress = 0;
}

void Projectile::update(double dt)
{
	launch(tx, ty, 900, dt);

	for (int key = 0; key < enemies->getSize(); key++)
	{
		Entity* value = enemies->getEntity(key);
		if (contact(value->getX(), value->getY()) and value != parent)
		{
			std::cout << "shot!";
			value->setState("stunned");
			drop();
		}
	}
	if (contact(player->x, player->y) and player != parent)
	{
		std::cout << "Shot the player, Gosh!\n";
		drop();
	}
	if ((resolveCollision() && !contact(x, y)) || (vx == 0 && vy == 0))
	{
		drop();
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

//To do check for invalid lines
Entity* ItemFactory::deserialise(std::string line)
{
	int space = line.find(" ");
	float x = std::stoi(line.substr(0, space + 1));
	float y = std::stoi(line.substr(space));

	//This could be a method of the enemies class
	return new Item(x, y);
}

//Dunno if I'll use this code
//Entity* ProjectileFactory::deserialise(std::string line)
//{
//	std::string buffer;
//	std::vector<int> arr;
//	std::istringstream str(line);
//	while (getline(str, buffer, ' '))
//	{
//		arr.push_back(std::stoi(buffer));
//	}
//
//	return new Projectile(arr[0], arr[1], arr[2], arr[3], new Player()); //That last bit is probably going to bug
//}