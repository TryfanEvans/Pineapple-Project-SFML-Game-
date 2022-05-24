#include "Melee.h"

#ifndef Ranged_H
	#define Ranged_H
class Ranged : public Enemy
{
public:
	Ranged();
	void update(double dt, float player_x, float player_y, Map* map);
};

#endif
