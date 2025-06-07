#include "Camera.h"
#include <SFML/Graphics.hpp>
using namespace sf;

Camera::Camera(sf::RenderWindow* win, Map& map) :
	win(win),
	view(sf::FloatRect(0.f, 0.f, 600.f, 400.f)),
	map(map)
{
}

std::tuple<float, float> Camera::viewPosition(float px, float py)
{
	auto [window_width, window_height] = win->getSize();
	auto [map_x, map_y] = map.getAbsoluteSize();
	float view_x = std::max(px, window_width / 2.f - map.tileSize);
	view_x = std::min(view_x, map_x - (window_width / 2.f) + map.tileSize);
	float view_y = std::max(py, window_height / 2.f - map.tileSize);
	view_y = std::min(view_y, map_y - (window_height / 2.f) + map.tileSize);

	return { view_x, view_y };
}

void Camera::set(float px, float py)
{
	auto [view_x, view_y] = viewPosition(px, py);
	x = view_x;
	y = view_y;
	view.setCenter(sf::Vector2f(x, y));
	win->setView(view);
}

std::tuple<int, int> Camera::worldToScreenPos(int tx, int ty)
{
	auto [window_width, window_height] = win->getSize();
	float screen_x = tx - x + (window_width / 2);
	float screen_y = ty - y + (window_height / 2);
	return { screen_x, screen_y };
}

std::tuple<int, int> Camera::screenToWorldPos(int tx, int ty)
{
	auto [window_width, window_height] = win->getSize();

	return { (x - window_width / 2 + tx), (y - window_height / 2 + ty) };
}
