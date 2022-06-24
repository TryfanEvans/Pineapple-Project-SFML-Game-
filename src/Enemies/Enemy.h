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
	//Returns Melee or Ranged, useful for saving map layouts
	std::string getType();
	//If there are any walls within a square boundary with corners at the player and Enemy
	bool getObstructed(float player_x, float player_y, Map* map);
	//Draws the enemy onto the window
	virtual void render(sf::RenderTarget* target);
	//Handles the behaviour of the enemy in all it's states
	virtual void update(double, float, float, Map*, bool& alive) = 0;
	//Changes the enemies state
	void setState(std::string state);
	//Uses the pathfinding overlay of the map, bringing the enemy towards the adjacent tile with the lowest distance from the player
	void pathfinding(double dt, Map* map);
};

class Melee : public Enemy
{
public:
	sf::Texture texture;
	std::string type = "Melee";

	Melee();
	void update(double dt, float player_x, float player_y, Map* map, bool& alive);
};

class Ranged : public Enemy
{
public:
	std::string type = "Ranged";
	Pellet pellet;
	float const cooldown_duration = 3;
	float cooldown_progress = 0;
	sf::Texture texture;

	float tx;
	float ty;

	Ranged();
	void update(double dt, float player_x, float player_y, Map* map, bool& alive);
	void render(sf::RenderTarget* target);
};

#endif