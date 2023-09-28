#include "Entity.h"
#ifndef ENTITYVEC_H
	#define ENTITYVEC_H

class EntityFactory
{
public:
	virtual Entity* deserialise(std::string type) = 0;
};

class EnemyFactory : public EntityFactory
{
public:
	Entity* deserialise(std::string type);
};

class ItemFactory : public EntityFactory
{
public:
	Entity* deserialise(std::string type);
};

class ProjectileFactory : public EntityFactory
{
public:
	Entity* deserialise(std::string type);
};

class EntityVec
{
public:
	std::vector<Entity*> entities;
	File file;
	//TODO: FIX memory leak
	EntityFactory* factory;
	EntityVec(std::string file_name, EntityFactory* factory) :
		file(file_name),
		factory(factory)
	{}

	EntityVec() :
		file("dumb")
	{}

	void render(sf::RenderWindow* win)
	{
		for (uint key = 0; key < entities.size(); key++)
		{
			Entity* value = entities[key];
			value->render(win);
		}
	}
	void update(float dt)
	{
		for (uint key = 0; key < entities.size(); key++)
		{

			Entity* value = entities[key];
			//Could be interesting to see how stunned state could be handled here
			value->update(dt);
			if (value->state == "dead")
			{
				remove(key);
			}
		}
	}
	void save()
	{
		std::string serialise;

		for (uint key = 0; key < entities.size(); key++)
		{
			Entity* entity = entities[key];
			serialise.append(entity->serialise());
		}
		file.setFile(serialise);
	}
	void load()
	{
		entities.clear();
		std::ifstream loadfile = file.getFile();
		std::string line;
		while (std::getline(loadfile, line))
		{
			entities.push_back(factory->deserialise(line));
		}
	}
	void add(std::string type, int map_x, int map_y)
	{
		std::string line = std::to_string(map_x) + " " + std::to_string(map_y) + "," + type;
		entities.push_back(factory->deserialise(line));
	}

	void add(Entity* spawn)
	{
		entities.push_back(spawn);
	}

	int getSize()
	{
		return entities.size();
	}
	int getAtPosition(int x, int y)
	{
		for (uint key = 0; key < entities.size(); key++)
		{
			Entity* value = entities[key];
			if (value->contact(x, y))
			{
				return key;
			}
		}
		return -1;
	}
	void remove(int key)
	{
		if (key == -1)
			return;
		delete entities[key];
		entities.erase(entities.begin() + key);
	}
	Entity* getEntity(int key)
	{
		return entities[key];
	};
};

#endif