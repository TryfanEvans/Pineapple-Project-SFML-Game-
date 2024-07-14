#include "Collision.h"

static bool collision = false;

Solid::Solid(int contactRadius)
{
	this->contactRadius = contactRadius;
}

void Solid::setGridPosition(double gx, double gy)
{
	x = (gx + 0.5) * map->tileSize;
	y = (gy + 0.5) * map->tileSize;
}

void Solid::setPosition(double x, double y)
{
	this->x = (x + 0.5);
	this->y = (y + 0.5);
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
	return { std::floor(x / map->tileSize), std::floor(y / map->tileSize) };
}

float Solid::getDistance(float tx, float ty)
{
	float dx = tx - x;
	float dy = ty - y;

	return std::sqrt(pow(dx, 2) + pow(dy, 2));
}

float Solid::getOrientation(float tx, float ty)
{
	return (-atan2(tx - x, ty - y) + 3.14 / 2);
}

std::tuple<float, float> Solid::resolveCornerCollision(int ex, int ey, int tx, int ty)
{
	int overlapx = ex * map->tileSize - tx;
	int overlapy = ey * map->tileSize - ty;
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
	int distance = edge * map->tileSize - selfPos;
	int overlap = std::abs(distance) - contactRadius;
	if (overlap > 0)
	{
		return selfPos;
	};
	collision = true;
	return selfPos + distance * overlap / contactRadius;
}

//Could be split into edge and corner, with for loop to iterate over each?
bool Solid::resolveCollision()
{

	collision = false;
	auto [gx, gy] = this->getGridPosition();

	if (!map->isFloor(gx - 1, gy))
	{
		this->x = resolveEdgeCollision(gx, x);
	}
	if (!map->isFloor(gx - 1, gy - 1))
	{

		auto [x, y] = resolveCornerCollision(gx, gy, this->x, this->y);
		this->x = x;
		this->y = y;
	}
	if (!map->isFloor(gx, gy - 1))
	{

		this->y = resolveEdgeCollision(gy, y);
	}
	if (!map->isFloor(gx + 1, gy - 1))
	{

		auto [x, y] = resolveCornerCollision(gx + 1, gy, this->x, this->y);
		this->x = x;
		this->y = y;
	}
	if (!map->isFloor(gx + 1, gy))
	{

		this->x = resolveEdgeCollision(gx + 1, x);
	}
	if (!map->isFloor(gx + 1, gy + 1))
	{

		auto [x, y] = resolveCornerCollision(gx + 1, gy + 1, this->x, this->y);
		this->x = x;
		this->y = y;
	}
	if (!map->isFloor(gx, gy + 1))
	{

		this->y = resolveEdgeCollision(gy + 1, y);
	}
	if (!map->isFloor(gx - 1, gy + 1))
	{
		auto [x, y] = resolveCornerCollision(gx, gy + 1, this->x, this->y);
		this->x = x;
		this->y = y;
	}
	return collision;
}

void Solid::launch(float tx, float ty, double power, float dt)
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
		float dx = vx / pow(charge_progress * 3 + 1, 2);
		float dy = vy / pow(charge_progress * 3 + 1, 2);

		float velocity = std::sqrt(pow(dx, 2) + pow(dy, 2));
		const int max_interval = 2;

		if (max_interval < velocity)
		{
			for (int i = 1; i <= std::ceil(velocity / max_interval); i++)
			{
				x += dx * max_interval / velocity;
				y += dy * max_interval / velocity;
				resolveCollision();
			}
		}
		else
		{
			vx = 0;
			vy = 0;
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

void Solid::move()
{
	x += vx;
	y += vy;
	resolveCollision();
	vx = 0;
	vy = 0;
}