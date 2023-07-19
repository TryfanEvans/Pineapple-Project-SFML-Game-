#include "Projectile.h"

//TODO: Fix Memory Leak
//The constructors are duplicate between each enemy types, this is just in case of any new enemies requiring custom constructors
#ifndef ENEMY_H
	#define ENEMY_H

class Enemy : public Entity
{

public:
	std::string type;

	double stunned_duration = 2;
	double stunned_progress = 0;

	virtual void takeHit()
	{
		state = "dead";
	};

	Enemy(std::string type);
	//Returns Melee or Ranged, useful for saving map layouts
	virtual std::string getType() const;
	//If there are any walls within a square boundary with corners at the player and Enemy
	bool getObstructed(float player_x, float player_y);
	//Draws the enemy onto the window
	virtual void render(sf::RenderTarget* target);
	//Handles the behaviour of the enemy in all it's states
	virtual void update(double) = 0;
	//Changes the enemies state
	void setState(std::string state);
	//Uses the pathfinding overlay of the map, bringing the enemy towards the adjacent tile with the lowest distance from the player
	void pathfinding(double dt);
	//Turns the enemy coordinate and type fields into a string
	std::string serialise();
};

class Melee : public Enemy
{
public:
	sf::Texture texture;

	//Spawns a Melee enemy at the given grid coordinates
	Melee(int x, int y);
	//Handles the pathfinding, passive, attacking and stunned states. Launches itself at the player
	void update(double dt);
};

//Projectiles get deleted when Ranged is against wall, due to collision.
//TODO: FIX THAT
class Ranged : public Enemy
{
public:
	float const cooldown_duration = 2;
	float cooldown_progress = 0;
	sf::Texture texture;

	bool loaded = true;

	float tx;
	float ty;

	//Spawns a Ranged enemy at the given grid coordinates
	Ranged(int x, int y);
	//Handles the pathfinding, passive, attacking and stunned states. Launches pellets at the player
	void update(double dt);
	//Renders the enemy and the pellets it shoots out
	void render(sf::RenderTarget* target);
};
#endif