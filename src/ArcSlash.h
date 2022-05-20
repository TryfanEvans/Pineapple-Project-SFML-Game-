class ArcSlash
{
public:
	int segments = 10;
	sf::Vertex line[10];

	//This really belongs as a stand alone function
	//Credit to Nils Pipenbrinck from https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	std::tuple<float, float> rotateAroundPoint(float ox, float oy, float tx, float ty, float angle)
	{

		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		tx -= ox;
		ty -= oy;

		// rotate point
		float xnew = tx * c - ty * s;
		float ynew = tx * s + ty * c;

		// translate point back:
		tx = xnew + ox;
		ty = ynew + oy;
		return { tx, ty };
	}

	ArcSlash(int ox, int oy, int radius, double start_angle, double end_angle)
	{
		float angle_increment = (end_angle - start_angle) / segments;
		for (int i = 0; i < segments; i++)
		{
			auto [point_x, point_y] = rotateAroundPoint(ox, oy, ox + radius, oy, start_angle + (angle_increment * i));
			line[i] = sf::Vertex(sf::Vector2f(point_x, point_y));
		}
	}

	void render(sf::RenderTarget* target)
	{
		target->draw(line, segments, sf::LineStrip);
	}
};