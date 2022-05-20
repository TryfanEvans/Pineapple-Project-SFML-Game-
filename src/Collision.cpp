#include "Collsion.h"

void Solid::setPosition(int x, int y, int tileSize)
{
	this->x = x * tileSize;
	this->y = y * tileSize;
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

std::tuple<float, float> Solid::resolveCornerCollision(int ex, int ey, int tx, int ty)
{
	int overlapx = ex * tileSize - tx;
	int overlapy = ey * tileSize - ty;
	int overlap = std::sqrt(pow(overlapx, 2) + pow(overlapy, 2)) - contactRadius;
	if (overlap > 0)
	{
		return { tx, ty };
	};

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
	return selfPos + distance * overlap / contactRadius;
}

void Solid::resolveCollision(Map& map)
{
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
}

void Solid::charge(float tx, float ty, double power, float dt)
{
	float dx = tx - x;
	float dy = ty - y;

	float distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	static float vx;
	static float vy;

	if (charge_progress == 0)
	{
		std::cout << "init";
		vx = dx * dt * power / distance;
		vy = dy * dt * power / distance;
	}

	if (charge_progress > charge_duration)
	{
		//std::cout << "x += vx / pow(charge_progress, 9); " << x << " " << pow(charge_progress, 9) << " " << charge_progress << "\n";
		x += vx / pow(charge_progress, 9);
		y += vy / pow(charge_progress, 9);
	}
	std::cout << x << " " << y << " " << vx << "\n";

	charge_progress = charge_progress + dt;
}

bool Solid::contact(float tx, float ty)
{
	float dx = tx - x;
	float dy = ty - y;
	float distance = std::sqrt(pow(dx, 2) + pow(dy, 2));

	if (distance < 32)
	{
		std::cout << "HIT";
		return true;
	}
	return false;
}