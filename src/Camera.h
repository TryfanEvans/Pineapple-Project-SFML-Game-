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
	std::tuple<float, float> viewPosition(float px, float py);
	void set(float px, float py);
	std::tuple<int, int> worldToScreenPos(int x, int y);
	std::tuple<int, int> screenToWorldPos(int x, int y);
};