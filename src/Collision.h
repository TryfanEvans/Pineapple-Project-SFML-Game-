#include "Map.h"
#include "States/Scripts.h"

#ifndef Solid_H
	#define Solid_H

class EntityVec;
class Solid
{
public:
	static EntityVec* projectiles;
	static EntityVec* enemies;
	static EntityVec* items;
	static Solid* player;
	static Map* map;

	int contactRadius;
	int speed;
	float x;
	float y;
	//Abstraction for getting the x coordinate
	float getX();
	//Abstraction for getting the y coordinate
	float getY();

	float vy = 0;
	float vx = 0;
	//Moves the Solid by it's velocity vector and deletes the velocity
	void move();

	//Refactor: Could move some code into coordinate class li
	Solid() {};
	Solid(int contactradius);
	//Returns the distance of the center of the Solid from the coordinates given
	float getDistance(float tx, float ty);
	//Returns the angle between the center of the Solid and the coordinates given
	float getOrientation(float tx, float ty);
	//Prevents the solid from phasing through the map
	bool resolveCollision();
	//Prevents the player from entering tiles diagonally
	std::tuple<float, float> resolveCornerCollision(int ex, int ey, int tx, int ty);
	//Prevents the player from entering adjacent tiles
	int resolveEdgeCollision(int edge, int selfPos);
	//Takes the tile coordinates of the position for the entity
	void setGridPosition(double gx, double gy);
	//Takes the pixel/absolute coordinates of the position for the entity
	void setPosition(double x, double y);
	//Converts the pixel coordinates to grid coordinates. Feels like this could be in the map class? More semantically appropriate IMO.
	std::tuple<float, float> getGridPosition();

	double charge_duration = 0;
	double charge_progress = 0;
	//Moves the Solid rapidly until momentumn is depleted
	void launch(float tx, float ty, double power, float dt);
	//When the Solid meets another Solid at the coordinates given
	bool contact(float tx, float ty);
};
#endif