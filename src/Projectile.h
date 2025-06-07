#include "EntityVec.h"

//TODO: Fix Memory Leak
//What memory leak u fucking liar
#ifndef PELLET_H
	#define PELLET_H
class ItemFactory : public EntityFactory
{
public:
	Entity* deserialise(std::string type);
};

class Item : public Entity
{
public:
	Item(int x, int y);
	sf::CircleShape sprite;
	//Right now the item is just a white circle, would be good to give it a few different textures for flavour
	void render(sf::RenderTarget* target);
	void setState(std::string) {};
	std::string serialise();
};

class ProjectileFactory : public EntityFactory
{
public:
	Entity* deserialise(std::string type);
};

//The Player and the ranged enemy will shoot these out
class Projectile : public Entity
{
public:
	int tx;
	int ty;

	Entity* parent;
	Projectile(float, float, int, int, Entity*);
	int contactRadius = 2;
	bool active = true;
	sf::CircleShape sprite;
	//Right now the pellet is just a white circle, would be good to give it a few different textures for flavour. Matching the item picked up
	void render(sf::RenderTarget* target);
	//Replaces the pellet with an item at it's location
	void drop();
	//Removes the pellet from the inventory and adds it to the world
	void toss(int x, int y);
	//Keeps the pellet in motion if it is active, otherwise nothing
	void update(double dt);

	std::string serialise()
	{
		return "";
	};
	void setState(std::string) {};

	void pickUp();
};

#endif