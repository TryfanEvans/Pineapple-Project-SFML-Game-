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

	float vy;
	float vx;
	void move(Map& map);

	float getDistance(float tx, float ty);

	bool resolveCollision(Map& map);
	std::tuple<float, float> resolveCornerCollision(int ex, int ey, int tx, int ty);
	int resolveEdgeCollision(int edge, int selfPos);
	void setPosition(double x, double y, int tileSize);
	std::tuple<float, float> getGridPosition();

	double charge_duration = 0;
	double charge_progress = 0;
	void launch(float tx, float ty, double power, float dt, Map& map);
	bool contact(float tx, float ty);
};

//The Player and the ranged enemy will shoot these out
class Pellet : public Solid
{
public:
	int contactRadius = 4;
	bool stored = true;
	bool active = false;
	sf::CircleShape sprite;
	void render(sf::RenderTarget* target);
};

class Item : public Solid
{
public:
	sf::CircleShape sprite;
	void render(sf::RenderTarget* target);
};