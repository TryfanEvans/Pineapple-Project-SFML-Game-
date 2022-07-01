//#include "tests.cpp"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

#include "States/State.h"

//CREDIT TO https://github.com/rewrking/sfml-vscode-boilerplate by Andrew King, OggyP, and LucasDoesDev for the build environment and makefile

//Credit to Music Break "Charming Lute Mysterious, Relaxed" under Creative Commons License

int main()
{
	//test();
	try
	{
		RenderWindow window(VideoMode(300, 300), "Pineapple Project!");

		StateData stateData;

		State* state = new GameState(stateData, window);

		sf::Clock deltaClock;
		float dt = 0;
		//Ok this is really weird getting rid of this breaks the physics system? wtf
		window.setFramerateLimit(60);

		sf::Music music;
		if (!music.openFromFile("content/backgroundmusic.ogg"))
		{
			std::cout << "Failed to  music \n";
		} // error
		music.setLoop(true);

		music.play();

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
							case Mouse::Middle:
								button = 3;
								break;
							default:
								button = 0;
								break;
						}
						state->click(event.mouseButton.x, event.mouseButton.y, button);
						break;

					// we don't process other types of events
					default:
						break;
				}
			}

			dt = deltaClock.restart().asSeconds();
			//std::cout << "dt " << dt << "\n";
			music.setVolume(stateData.music_volume);

			//Really need to overhaul this
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && (stateData.gameover || stateData.dead))
			{
				std::cout << "Respawn!\n";
				state = new GameState(stateData, window);
				stateData.gameover = false;
				stateData.dead = false;
				state->player.x = 50;
				state->player.y = 50;
			}
			state->update(dt);

			window.clear();
			state->draw();
			window.display();
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error! " << e.what() << "\n";
	}
	return 0;
}