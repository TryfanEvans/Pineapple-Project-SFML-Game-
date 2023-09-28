#include "EntityVec.h"

EntityVec::EntityVec(std::string file_name, EntityFactory* factory) :
	file(file_name),
	factory(factory)
{}

EntityVec::EntityVec() :
	file("dumb")
{}

void EntityVec::render(sf::RenderWindow* win)
{
	for (uint key = 0; key < entities.size(); key++)
	{
		Entity* value = entities[key];
		value->render(win);
	}
}
void EntityVec::update(float dt)
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
void EntityVec::save()
{
	std::string serialise;

	for (uint key = 0; key < entities.size(); key++)
	{
		Entity* entity = entities[key];
		serialise.append(entity->serialise());
	}
	file.setFile(serialise);
}
void EntityVec::load()
{
	entities.clear();
	std::ifstream loadfile = file.getFile();
	std::string line;
	while (std::getline(loadfile, line))
	{
		entities.push_back(factory->deserialise(line));
	}
}
void EntityVec::add(std::string type, int map_x, int map_y)
{
	std::string line = std::to_string(map_x) + " " + std::to_string(map_y) + "," + type;
	entities.push_back(factory->deserialise(line));
}

void EntityVec::add(Entity* spawn)
{
	entities.push_back(spawn);
}

int EntityVec::getSize()
{
	return entities.size();
}
int EntityVec::getAtPosition(int x, int y)
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
void EntityVec::remove(int key)
{
	if (key == -1)
		return;
	delete entities[key];
	entities.erase(entities.begin() + key);
}

Entity* EntityVec::getEntity(int key)
{
	return entities[key];
}