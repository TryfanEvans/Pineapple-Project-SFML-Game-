#include "Map.h"

class Solid
{

public:
	int contactRadius = 10;
	int tileSize;
	int speed;
	float x;
	float y;
	float getX();
	float getY();

	void resolveCollision(Map& map);
	std::tuple<float, float> resolveCornerCollision(int ex, int ey, int tx, int ty);
	int resolveEdgeCollision(int edge, int selfPos);
	void setPosition(int x, int y, int tileSize);
	std::tuple<float, float> getGridPosition();

	int charge_duration = 0;
	double charge_progress = 0;
	void charge(float tx, float ty, double power, float dt);
	bool contact(float tx, float ty);
};