#include "Collsion.h"

//TODO: Fix Memory Leak

class Enemy : public Solid
{
private:
public:
	sf::CircleShape sprite;
	std::string state = "passive";
	double stunned_duration = 2;
	double stunned_progress = 0;

	Enemy();
	bool getObstructed(float player_x, float player_y, Map* map);
	void render(sf::RenderTarget* target);
	virtual void update(double, float, float, Map*) = 0;
	void setState(std::string state);
	void pathfinding(double dt, Map* map);
};

class Melee : public Enemy
{
public:
	Melee();
	void update(double dt, float player_x, float player_y, Map* map);
};