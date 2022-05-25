#include "Enemy.h"

Enemy::Enemy() :
	sprite()
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

std::string Enemy::getType()
{
	return type;
}

void Enemy::setState(std::string state)
{
	this->state = state;
}

void Enemy::pathfinding(double dt, Map* map)
{
	auto [gx, gy] = this->getGridPosition();

	int tx = gx + 1;
	int ty = gy;

	int cheapest = std::min({ map->getPathTile(gx, gy + 1), map->getPathTile(gx + 1, gy), map->getPathTile(gx - 1, gy), map->getPathTile(gx, gy - 1) });
	if (map->getPathTile(gx, gy + 1) == cheapest)
	{
		tx = gx;
		ty = gy + 1;
	}
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
	tx = tx * tileSize + tileSize / 2;
	ty = ty * tileSize + tileSize / 2;

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

	move(*map);
}

bool Enemy::getObstructed(float player_x, float player_y, Map* map)
{
	auto [gx, gy] = getGridPosition();
	bool obstructed = false;
	for (int x = std::min(gx, std::floor(player_x / tileSize)); x <= std::max(gx, std::floor(player_x / tileSize)); x++)
	{
		for (int y = std::min(gy, std::floor(player_y / tileSize)); y <= std::max(gy, std::floor(player_y / tileSize)); y++)
		{

			if (map->getTile(x, y) == 1)
			{
				obstructed = true;
			}
		}
	}
	return obstructed;
}