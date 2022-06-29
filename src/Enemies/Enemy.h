#include "Pellet.h"

//TODO: Fix Memory Leak
//The constructors are duplicate between each enemy types, this is just in case of any new enemies requiring custom constructors
#ifndef ENEMY_H
	#define ENEMY_H
class Enemy : public Solid
{

public:
	std::string type;

	sf::Sprite sprite;
	static sf::Texture texture;

	std::string state = "passive";
	double stunned_duration = 2;
	double stunned_progress = 0;

	Enemy(std::string type);
	//Returns Melee or Ranged, useful for saving map layouts
	virtual std::string getType() const;
	//If there are any walls within a square boundary with corners at the player and Enemy
	bool getObstructed(float player_x, float player_y);
	//Draws the enemy onto the window
	virtual void render(sf::RenderTarget* target);
	//Handles the behaviour of the enemy in all it's states
	virtual void update(double, float, float, bool& dead) = 0;
	//Changes the enemies state
	void setState(std::string state);
	//Uses the pathfinding overlay of the map, bringing the enemy towards the adjacent tile with the lowest distance from the player
	void pathfinding(double dt);
};

class Melee : public Enemy
{
public:
	sf::Texture texture;
	std::string type;

	//Spawns a Melee enemy at the given grid coordinates
	Melee(Map* map, int x, int y);
	void update(double dt, float player_x, float player_y, bool& dead);
};

class Ranged : public Enemy
{
public:
	std::string type;

	Pellet pellet;
	float const cooldown_duration = 3;
	float cooldown_progress = 0;
	sf::Texture texture;

	float tx;
	float ty;

	//Spawns a Ranged enemy at the given grid coordinates
	Ranged(Map* map, int x, int y);
	void update(double dt, float player_x, float player_y, bool& dead);
	void render(sf::RenderTarget* target);
};

#endif