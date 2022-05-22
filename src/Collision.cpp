#include "Collsion.h"

static bool collision = false;

void Solid::setPosition(double x, double y, int tileSize)
{
	this->x = (x + 0.5) * tileSize;
	this->y = (y + 0.5) * tileSize;
	std::cout << "setPosit " << this->x << this->y;
}

float Solid::getX()
{
	return this->x;
}

float Solid::getY()
{
	return this->y;
}

std::tuple<float, float> Solid::getGridPosition()
{
	return { std::floor(x / tileSize), std::floor(y / tileSize) };
}

float Solid::getDistance(float tx, float ty)
{
	float dx = tx - x;
	float dy = ty - y;

	return std::sqrt(pow(dx, 2) + pow(dy, 2));
}

std::tuple<float, float> Solid::resolveCornerCollision(int ex, int ey, int tx, int ty)
{
	int overlapx = ex * tileSize - tx;
	int overlapy = ey * tileSize - ty;
	int overlap = std::sqrt(pow(overlapx, 2) + pow(overlapy, 2)) - contactRadius;
	if (overlap > 0)
	{
		return { tx, ty };
	};
	collision = true;

	return { (tx + (overlapx * overlap / contactRadius)), (ty + (overlapy * overlap / contactRadius)) };
}

int Solid::resolveEdgeCollision(int edge, int selfPos)
{
	int distance = edge * tileSize - selfPos;
	int overlap = std::abs(distance) - contactRadius;
	if (overlap > 0)
	{
		return selfPos;
	};
	collision = true;
	return selfPos + distance * overlap / contactRadius;
}

bool Solid::resolveCollision(Map& map)
{
	collision = false;
	tileSize = map.tileSize;
	auto [gx, gy] = this->getGridPosition();

	if (map.getTile(gx - 1, gy) == 1)
	{

		this->x = resolveEdgeCollision(gx, x);
	};
	if (map.getTile(gx - 1, gy - 1) == 1)
	{
		auto [x, y] = resolveCornerCollision(gx, gy, this->x, this->y);
		this->x = x;
		this->y = y;
	};
	if (map.getTile(gx, gy - 1) == 1)
	{

		this->y = resolveEdgeCollision(gy, y);
	};
	if (map.getTile(gx + 1, gy - 1) == 1)
	{
		auto [x, y] = resolveCornerCollision(gx + 1, gy, this->x, this->y);
		this->x = x;
		this->y = y;
	};
	if (map.getTile(gx + 1, gy) == 1)
	{
		this->x = resolveEdgeCollision(gx + 1, x);
	};
	if (map.getTile(gx + 1, gy + 1) == 1)
	{
		auto [x, y] = resolveCornerCollision(gx + 1, gy + 1, this->x, this->y);
		this->x = x;
		this->y = y;
	};
	if (map.getTile(gx, gy + 1) == 1)
	{
		this->y = resolveEdgeCollision(gy + 1, y);
	};
	if (map.getTile(gx - 1, gy + 1) == 1)
	{
		auto [x, y] = resolveCornerCollision(gx, gy + 1, this->x, this->y);
		this->x = x;
		this->y = y;
	};
	return collision;
}

void Solid::launch(float tx, float ty, double power, float dt, Map& map)
{
	float dx = tx - x;
	float dy = ty - y;

	float distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	if (charge_progress == 0)
	{
		vx = dx * dt * power / distance;
		vy = dy * dt * power / distance;
	}

	if (charge_progress > charge_duration)
	{
		float dx = vx / pow(charge_progress * 2 + 1, 5);
		float dy = vy / pow(charge_progress * 2 + 1, 5);

		float velocity = std::sqrt(pow(dx, 2) + pow(dy, 2));
		const int max_interval = 2;

		for (int i = 1; i <= std::ceil(velocity / max_interval); i++)
		{
			x += dx * max_interval / velocity;
			y += dy * max_interval / velocity;
			resolveCollision(map);
		}
	}

	charge_progress = charge_progress + dt;
}

bool Solid::contact(float tx, float ty)
{
	float distance = getDistance(tx, ty);

	//The collision will always be between two entities, or an enity and a pellet
	return (distance < 16 + contactRadius);
}

void Solid::move(Map& map)
{
	x += vx;
	y += vy;
	resolveCollision(map);
	vx = 0;
	vy = 0;
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

void Item::render(sf::RenderTarget* target)
{
	sprite.setPosition(x, y);
	sprite.setRadius(8);
	sprite.setFillColor(sf::Color(250, 250, 250));
	sprite.setOrigin(8, 8);
	target->draw(sprite);
}