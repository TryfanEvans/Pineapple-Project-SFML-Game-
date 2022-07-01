#include "player.h"

class Camera
{
public:
	float x;
	float y;
	sf::RenderWindow& win;
	sf::View view;
	Map& map;
	Camera(sf::RenderWindow& win, Map& map);
	//Calculates where the camera is supposed to be in relation to the map
	std::tuple<float, float> viewPosition(float px, float py);
	//Updates and applies the camera to the view
	void set(float px, float py);
	//Converts the world position given to a pixel within the window
	std::tuple<int, int> worldToScreenPos(int x, int y);
	//Converts the screen position given to a pixel within the world
	std::tuple<int, int> screenToWorldPos(int x, int y);
};