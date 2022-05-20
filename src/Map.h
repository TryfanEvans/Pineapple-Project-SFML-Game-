class Map
{
public:
	int grid_width = 20;
	int grid_height = 20;
	int grid[400];
	int pathfinding[400];
	int tileSize = 32;

	Map(std::string level);
	int getTile(int x, int y);
	void setTile(int x, int y, int value);
	void save();
	void loadFromFile();
	void render(sf::RenderWindow* win);

	int getPathTile(int x, int y);
	void setPathTile(int x, int y, int value);
	void generatePathfinding(int tx, int ty);
};
