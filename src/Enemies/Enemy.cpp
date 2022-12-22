#include "Enemy.h"

Enemy::Enemy(std::string type) :
	Entity(14),
	type(type)
{
}

void Enemy::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	if (state == "stunned")
	{
		sprite.setColor(sf::Color(100, 100, 100));
	}
	else
	{
		sprite.setColor(sf::Color(255, 255, 255));
	}
	target->draw(sprite);
}

std::string Enemy::getType() const
{
	return type;
}

void Enemy::setState(std::string state)
{
	this->state = state;
}

void Enemy::pathfinding(double dt)
{
	auto [gx, gy] = this->getGridPosition();

	int tx;
	int ty;

	int cheapest = std::min({ map->getPathTile(gx, gy + 1), map->getPathTile(gx + 1, gy), map->getPathTile(gx - 1, gy), map->getPathTile(gx, gy - 1) });
	// Test pathfinding
	// std::cout << "cheapest: " << cheapest << "\n";
	// std::cout << "right: " << map->getPathTile(gx + 1, gy) << "\n";
	// std::cout << "down: " << map->getPathTile(gx, gy + 1) << "\n";
	// std::cout << "left: " << map->getPathTile(gx - 1, gy) << "\n";

	if (map->getPathTile(gx - 1, gy) == cheapest)
	{
		tx = gx - 1;
		ty = gy;
	}
	if (map->getPathTile(gx, gy - 1) == cheapest)
	{
		tx = gx;
		ty = gy - 1;
	}
	if (map->getPathTile(gx + 1, gy) == cheapest)
	{
		tx = gx + 1;
		ty = gy;
	}
	if (map->getPathTile(gx, gy + 1) == cheapest)
	{
		tx = gx;
		ty = gy + 1;
	}
	tx = (tx + 0.5) * map->tileSize;
	ty = (ty + 0.5) * map->tileSize;

	double dx = tx - x;
	double dy = ty - y;

	double distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	vx = dx / distance * dt * speed;
	vy = dy / distance * dt * speed;

	if (vx > 0)
	{
		vx = std::ceil(vx);
	}
	else
	{
		vx = std::floor(vx);
	}
	if (vy > 0)
	{
		vy = std::ceil(vy);
	}
	else
	{
		vy = std::floor(vy);
	}

	move();
}

bool Enemy::getObstructed(float player_x, float player_y)
{
	auto [gx, gy] = getGridPosition();
	bool obstructed = false;
	for (int x = std::min(gx, std::floor(player_x / map->tileSize)); x <= std::max(gx, std::floor(player_x / map->tileSize)); x++)
	{
		for (int y = std::min(gy, std::floor(player_y / map->tileSize)); y <= std::max(gy, std::floor(player_y / map->tileSize)); y++)
		{

			if (map->getTile(x, y) == 1)
			{
				obstructed = true;
			}
		}
	}
	return obstructed;
}

std::string Enemy::serialise()
{
	auto [gx, gy] = getGridPosition();
	return std::string(std::to_string(gx) + " " + std::to_string(gy) + "," + getType() + "\n");
}

Entity* EnemyFactory::deserialise(std::string line)
{
	int space = line.find(" ");
	int comma = line.find(",");
	float x = std::stoi(line.substr(0, space + 1));
	float y = std::stoi(line.substr(space, comma));
	std::string type = line.substr(comma + 1);

	//This could be a method of the enemies class
	if (type == "Melee")
	{
		return new Melee(x, y);
	}
	else if (type == "Ranged")
	{
		return new Ranged(x, y);
	}
	else
	{
		std::cout << type << " is not a valid enemy type!\n";
		return new Melee(x, y);
	}
}
