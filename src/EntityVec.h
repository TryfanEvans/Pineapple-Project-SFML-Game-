#include "Entity.h"
#ifndef ENTITYVEC_H
	#define ENTITYVEC_H

class EntityFactory
{
public:
	virtual Entity* deserialise(std::string type) = 0;
};

class EntityVec
{
public:
	std::vector<Entity*> entities;
	File file;
	//TODO: FIX memory leak
	EntityFactory* factory;
	EntityVec(std::string file_name, EntityFactory* factory);
	EntityVec();

	void render(sf::RenderWindow* win);
	void update(float dt);
	void save();
	void load();
	void add(std::string type, int map_x, int map_y);
	void add(Entity* spawn);
	int getSize();
	int getAtPosition(int x, int y);
	void remove(int key);
	Entity* getEntity(int key);
};

#endif