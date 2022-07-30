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

bool Map::isFloor(int x, int y)
{
	return (getTile(x, y) == 0 || getTile(x, y) == 2);
}

//Seems a little redundant till I add tiles that are pits and spikes
bool Map::isSolid(int x, int y)
{
	return (getTile(x, y) == 1 || getTile(x, y) == 4);
}

Map::Map() :
	file("map")
{
	grid = new int[grid_width * grid_height];
	pathfinding = new int[grid_width * grid_height];
	tile_atlas.loadFromFile("content/tile_atlas.png");
	for (int i = 0; i < grid_width * grid_height; i++)
	{
		grid[i] = 0;
	}
}

Map::~Map()
{
	delete grid;
	delete pathfinding;
}

//loads level into map
//Might want to refactor the loading process to call the destructor?
void Map::load()
{
	std::ifstream loadfile = file.getFile();
	std::string line;
	int i = 0;
	while (std::getline(loadfile, line))
	{
		if (i == 0)
		{
			int space = line.find(" ");
			grid_width = std::stoi(line.substr(0, space + 1));
			grid_height = std::stoi(line.substr(space));
			delete grid;
			delete pathfinding;
			grid = new int[grid_width * grid_height];
			pathfinding = new int[grid_width * grid_height];
		}
		else
		{
			for (int j = 0; j < grid_width * grid_height; j++)
			{
				//Converts unicode to int
				grid[j] = line.at(j) - '0';
			}
		}
		i++;
	}
}

void Map::save()
{
	std::string serialise = std::to_string(grid_width) + " " + std::to_string(grid_height) + "\n";
	for (int i = 0; i < grid_width * grid_height; i++)
	{
		serialise.append(std::to_string(grid[i]));
	}
	file.setFile(serialise);
}

//start optimisation here, it is so laggy
void Map::render(sf::RenderWindow* win)
{

	static sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	int tileset = 0;
	if (File::level_name == "Arena")
	{
		tileset = 32;
	}
	tile.setTexture(&tile_atlas);
	for (int x = -1; x < grid_width + 1; x++)
	{
		for (int y = -1; y < grid_height + 1; y++)
		{
			tile.setPosition(x * tileSize, y * tileSize);
			//Needs an invalid texture
			//Refactor: Could be made more mathematical
			if (getTile(x, y) == 0 || getTile(x, y) == 2)
			{
				tile.setTextureRect(sf::IntRect(0, tileset, 32, 32));
			}
			else if (getTile(x, y) == 1)
			{
				tile.setTextureRect(sf::IntRect(32, tileset, 32, 32));
			}
			else if (getTile(x, y) == 3)
			{
				tile.setTextureRect(sf::IntRect(64, tileset, 32, 32));
			}
			else if (getTile(x, y) == 4)
			{
				tile.setTextureRect(sf::IntRect(96, tileset, 32, 32));
			}
			win->draw(tile);
		}
	}
}

std::tuple<float, float> Map::getAbsoluteSize()
{
	return { ((grid_width)*tileSize), ((grid_height)*tileSize) };
}

std::tuple<float, float> Map::GridtoAbsolutePos(int x, int y)
{
	return { ((x + 0.5) * tileSize), ((y + 0.5) * tileSize) };
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
					if (x < grid_width && this->isFloor(x + 1, y) && this->getPathTile(x + 1, y) == 0)
					{
						this->setPathTile(x + 1, y, cost);
						complete = false;
					}
					if (y < grid_height && this->isFloor(x, y + 1) && this->getPathTile(x, y + 1) == 0)
					{
						this->setPathTile(x, y + 1, cost);
						complete = false;
					}
					if (x > 0 && this->isFloor(x - 1, y) && this->getPathTile(x - 1, y) == 0)
					{
						this->setPathTile(x - 1, y, cost);
						complete = false;
					}
					if (y > 0 && this->isFloor(x, y - 1) && this->getPathTile(x, y - 1) == 0)
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