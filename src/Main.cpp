#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#include "State.h"

//Credit to sfml-vscode-boilerplate by rewrking for the makefile and a lot of the file structure.
//Will probably remove this framework and add homemade makefile once I no longer need the debug option
//MAKEFILE IS NOT MY OWN WORK, MARK AS ABSENT ON RUBRIC (for now)

int main()
{
	RenderWindow window(VideoMode(300, 300), "SFML works!");
	std::cout << "HMM" << std::endl;
	State* state = new GameState();
	sf::Clock deltaClock;
	float dt = 0;
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// window closed
				case sf::Event::Closed:
					window.close();
					break;

					// key pressed
					//case sf::Event::KeyPressed:
					//	... break;
				case Event::MouseButtonPressed:
					int button;
					switch (event.mouseButton.button)
					{
						case Mouse::Left:
							button = 1;
							break;
						case Mouse::Right:
							button = 2;
							break;
						default:
							break;
					}
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
					state->click(event.mouseButton.x, event.mouseButton.y, button, &window);
					break;

				// we don't process other types of events
				default:
					break;
			}
		}

		dt = deltaClock.restart().asSeconds();
		std::cout << "dt " << dt << "\n";
		state->update(dt);

		window.clear();
		state->draw(&window);
		window.display();
	}

	return 0;
}