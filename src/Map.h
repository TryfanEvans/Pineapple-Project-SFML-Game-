#ifndef Map_H
#define Map_H

class Map
{
public:
	int grid_width = 20;
	int grid_height = 20;
	int* grid;
	int* pathfinding;
	int tileSize = 32;

	Map();
	~Map();

	//Returns the width in pixels of the map, not including the borders
	std::tuple<float, float> getAbsoluteSize();
	//Returns the absolute position of the center of the tile with the grid coordinates given
	std::tuple<float, float> GridtoAbsolutePos(int x, int y);
	//Abstraction for using cartesian coordinates to access the grid array
	int getTile(int x, int y);
	//Abstraction for using cartesian coordinates to access the grid array
	void setTile(int x, int y, int value);
	//Returns whether the tile is navigatable or not
	bool isFloor(int x, int y);
	//Returns whether the tile is solid or not
	bool isSolid(int x, int y);
	//Passes tiles and dimensions to a text file
	void save(std::string level_name);
	//Parses tiles and dimensions from a text file
	void load(std::string level);
	//Renders each tile individually
	void render(sf::RenderWindow* win, std::string level_name);
	//Abstraction for using cartesian coordinates to access the pathfinding array
	int getPathTile(int x, int y);
	//Abstraction for using cartesian coordinates to access the pathfinding array
	void setPathTile(int x, int y, int value);
	//Generates an overlay of the map with the distance to navigate to the target
	void generatePathfinding(int tx, int ty);
};

#endif