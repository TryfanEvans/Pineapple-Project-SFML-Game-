#include "Pellet.h"

//TODO: Fix Memory Leak
#ifndef ENEMY_H
	#define ENEMY_H
class Enemy : public Solid
{
private:
public:
	sf::Sprite sprite;
	static sf::Texture texture;
	std::string type = "faw";

	std::string state = "passive";
	double stunned_duration = 2;
	double stunned_progress = 0;

	Enemy();
	std::string getType();
	bool getObstructed(float player_x, float player_y, Map* map);
	virtual void render(sf::RenderTarget* target);
	virtual void update(double, float, float, Map*, bool& alive) = 0;
	void setState(std::string state);
	void pathfinding(double dt, Map* map);
};

#endif