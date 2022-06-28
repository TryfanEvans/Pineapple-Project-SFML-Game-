#include "Map.h"

static sf::Texture tile_atlas;

int Map::getTile(int x, int y)
{
	if ((x < grid_width && x >= 0) && (y < grid_height && y >= 0))
	{
		return grid[x + grid_width * y];
	}
	else
	{
		return 1;
	}
}

void Map::setTile(int x, int y, int value)
{
	if ((x < grid_width && x >= 0) && (y < grid_height && y >= 0))
	{
		grid[x + grid_width * y] = value;
	}
	else
	{
		std::cout << "Tried setting a tile outside of the map domain\n";
	}
}

Map::Map()
{
	tile_atlas.loadFromFile("content/tile_atlas.png");
	for (int i = 0; i < grid_width * grid_height; i++)
	{
		grid[i] = 0;
	}
}

//loads level into map
void Map::load(std::string level_name)
{
	std::ifstream loadfile("levels/" + level_name + "/map.txt");
	std::string line;
	int i = 0;
	while (std::getline(loadfile, line) && i < grid_width * grid_height)
	{
		if (line == "")
		{
			break;
		}
		grid[i] = stoi(line);
		i++;
	}
}

//start optimisation here, it is so laggy
void Map::render(sf::RenderWindow* win)
{

	static sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	tile.setTexture(&tile_atlas);
	for (int x = -1; x < grid_width + 1; x++)
	{
		for (int y = -1; y < grid_height + 1; y++)
		{
			tile.setPosition(x * tileSize, y * tileSize);

			if (getTile(x, y) == 0 || getTile(x, y) == 2)
			{
				tile.setTextureRect(sf::IntRect(0, 0, 32, 32));
			}
			else if (getTile(x, y) == 1)
			{
				tile.setTextureRect(sf::IntRect(32, 0, 32, 32));
			}
			else if (getTile(x, y) == 3)
			{
				tile.setTextureRect(sf::IntRect(64, 0, 32, 32));
			}

			win->draw(tile);
		}
	}
}

void Map::save(std::string level_name)
{
	//Deletes old map
	std::string file_name = "./levels/" + level_name + "/map.txt";
	remove(file_name.c_str());

	//Saves current map
	std::ofstream mapfile;
	mapfile.open(file_name, std::ios_base::app);
	for (int i = 0; i < grid_width * grid_height; i++)
	{
		mapfile << grid[i] << "\n";
	}
	mapfile.close();
}

int Map::getPathTile(int x, int y)
{
	if ((x < grid_width && x >= 0) && (y < grid_height && y >= 0))
	{
		return pathfinding[x + grid_width * y];
	}
	else
	{
		return std::numeric_limits<int>::max();
	}
}

void Map::setPathTile(int x, int y, int value)
{
	if ((x < grid_width && x >= 0) && (y < grid_height && y >= 0))
	{
		pathfinding[x + grid_width * y] = value;
	}
}

void Map::generatePathfinding(int tx, int ty)
{
	for (int i = 0; i < grid_width * grid_height; i++)
	{
		pathfinding[i] = 0;
	}
	this->setPathTile(tx, ty, 1);
	int cost = 1;
	bool complete = false;
	while (!complete)
	{
		complete = true;
		cost++;

		for (int x = 0; x < grid_width; x++)
		{
			for (int y = 0; y < grid_height; y++)
			{
				if (this->getPathTile(x, y) > 0 && this->getPathTile(x, y) != cost)
				{
					if (x < grid_width && this->getTile(x + 1, y) != 1 && this->getPathTile(x + 1, y) == 0)
					{
						this->setPathTile(x + 1, y, cost);
						complete = false;
					}
					if (y < grid_height && this->getTile(x, y + 1) != 1 && this->getPathTile(x, y + 1) == 0)
					{
						this->setPathTile(x, y + 1, cost);
						complete = false;
					}
					if (x > 0 && this->getTile(x - 1, y) != 1 && this->getPathTile(x - 1, y) == 0)
					{
						this->setPathTile(x - 1, y, cost);
						complete = false;
					}
					if (y > 0 && this->getTile(x, y - 1) != 1 && this->getPathTile(x, y - 1) == 0)
					{
						this->setPathTile(x, y - 1, cost);
						complete = false;
					}
				}
			}
		}
	}
	for (int x = -1; x <= grid_width; x++)
	{
		for (int y = -1; y <= grid_height; y++)
		{
			if (this->getTile(x, y) == 1)
			{
				this->setPathTile(x, y, std::numeric_limits<int>::max());
			}
		}
	}
}