#include "Collision.h"

#ifndef ENTITY_H
	#define ENTITY_H
class Entity : public Solid
{
public:
	sf::Sprite sprite;
	static sf::Texture texture;
	std::string state;

	Entity(int collision_radius) :
		Solid(collision_radius),
		sprite(),
		state("passive") {};

	virtual void takeHit() {};
	//Draws the enemy onto the window
	virtual void render(sf::RenderTarget* target) = 0;
	virtual void setState(std::string state) = 0;
	//Handles the behaviour of the enemy in all it's states
	virtual void update(double) {};
	//Turns the entity coordinate and type fields into a string
	virtual std::string serialise() = 0;
	//Gets the entity coordinate and type from string
	virtual ~Entity() = default;
};

#endif