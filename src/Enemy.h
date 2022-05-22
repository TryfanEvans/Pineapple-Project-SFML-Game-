#include "Collsion.h"

class Enemy : public Solid
{
private:
public:
	sf::CircleShape sprite;
	std::string state = "pathfinding";
	double stunned_duration = 3;
	double stunned_progress = 0;

	Enemy();
	void die();
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