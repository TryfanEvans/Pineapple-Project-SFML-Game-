#include "Map.h"

#ifndef Solid_H
	#define Solid_H
class Solid
{

public:
	Map* map;

	int contactRadius;
	int speed;
	float x;
	float y;
	float getX();
	float getY();

	float vy = 0;
	float vx = 0;
	void move();

	float getDistance(float tx, float ty);
	Solid() {};
	Solid(int contactradius);
	bool resolveCollision();
	std::tuple<float, float> resolveCornerCollision(int ex, int ey, int tx, int ty);
	int resolveEdgeCollision(int edge, int selfPos);
	//Takes the tile coordinates of the position for the entity
	void setGridPosition(double gx, double gy);
	//Takes the free/absolute coordinates of the position for the entity
	void setPosition(double x, double y);
	std::tuple<float, float> getGridPosition();

	double charge_duration = 0;
	double charge_progress = 0;
	void launch(float tx, float ty, double power, float dt);
	bool contact(float tx, float ty);
};

#endif