#include "Enemy.h"

#ifndef Melee_H
	#define Melee_H
class Melee : public Enemy
{
public:
	sf::Texture texture;

	Melee();
	void update(double dt, float player_x, float player_y, Map* map, bool& alive);
};

#endif