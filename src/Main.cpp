//#include "tests.cpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

#include "States/State.h"

//CREDIT TO https://github.com/rewrking/sfml-vscode-boilerplate by Andrew King, OggyP, and LucasDoesDev for the build environment and makefile

//Credit to Music Break "Charming Lute Mysterious, Relaxed" under Creative Commons License
class EntityFactory;
class EntityVec;

std::vector<std::string> Scripts::levels = { "Arena", "Ballerina", "Dungeon" };
std::string File::level_name = Scripts::levels[0];
//TODO: Fix memory leak

Map* Solid::map;
EntityVec* Solid::projectiles;
EntityVec* Solid::enemies;
EntityVec* Solid::items;
Solid* Solid::player;

Map* Scripts::map;
EntityVec* Scripts::projectiles;
EntityVec* Scripts::enemies;
EntityVec* Scripts::items;
Player* Scripts::player;
bool Scripts::gameover = false;
bool Scripts::controls = false;
std::stack<std::string> Scripts::actions_pending;

RenderWindow* Scripts::window;
int main()
{
	//test();
	try
	{
		RenderWindow window(VideoMode(600, 600), "Pineapple Project!");

		Scripts scripts;
		Scripts::window = &window;
		State* state = new GameState(scripts, window);
		Menu pause_menu(scripts);

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
					case Event::KeyPressed:
						if (event.key.code == Keyboard::Escape)
						{
							if (scripts.show_screen)
							{
								scripts.screen->update();
							}
							else
							{
								scripts.paused = !scripts.paused;
							}
						}
						break;
					case Event::MouseButtonPressed:
						if (!scripts.paused)
						{
							state->click(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
						}
						break;

					// we don't process other types of events
					default:
						break;
				}
			}

			dt = deltaClock.restart().asSeconds();
			//std::cout << "dt " << dt << "\n";
			music.setVolume(scripts.music_volume);

			//Really need to overhaul this
			state->update(dt);
			if (scripts.paused)
			{
				pause_menu.update(window);
			}

			window.clear();
			state->draw();
			if (scripts.paused and !scripts.show_screen)
			{
				pause_menu.render(&window);
			}
			window.display();
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error! " << e.what() << "\n";
	}
	return 0;
}