#ifndef Map_H
#define Map_H

class Map
{
public:
	int grid_width = 20;
	int grid_height = 20;
	int grid[400];
	int pathfinding[400];
	int tileSize = 32;

	Map();
	std::tuple<float, float> getAbsoluteSize();
	void load(std::string level);
	int getTile(int x, int y);
	void setTile(int x, int y, int value);
	void save(std::string level_name);
	void loadFromFile();
	void render(sf::RenderWindow* win);

	int getPathTile(int x, int y);
	void setPathTile(int x, int y, int value);
	void generatePathfinding(int tx, int ty);
};

#endif