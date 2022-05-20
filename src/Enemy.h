#include "Collsion.h"

class Enemy : public Solid
{
private:
public:
	sf::CircleShape sprite;
	std::string state = "pathfinding";

	Enemy();
	void die();
	void render(sf::RenderTarget* target);
	virtual void update(double, float, float, Map&) {};
};

class Melee : public Enemy
{
public:
	Melee();
	void update(double dt, float player_x, float player_y, Map* map);
};