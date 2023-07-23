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

Map* Solid::map = new Map();
EntityVec* Solid::projectiles = new EntityVec();
EntityVec* Solid::enemies = new EntityVec();
EntityVec* Solid::items = new EntityVec();
Solid* Solid::player = new Player();

Map* Scripts::map = new Map();
EntityVec* Scripts::projectiles = new EntityVec();
EntityVec* Scripts::enemies = new EntityVec();
EntityVec* Scripts::items = new EntityVec();
Player* Scripts::player = new Player();
bool Scripts::gameover = false;

int main()
{
	//test();
	try
	{
		RenderWindow window(VideoMode(600, 600), "Pineapple Project!");

		Scripts scripts;

		State* state = new GameState(scripts, window);

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
						state->click(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && (scripts.gameover || scripts.dead))
			{
				std::cout << "Respawn!\n";
				state = new GameState(scripts, window);
				scripts.gameover = false;
				scripts.dead = false;
				state->player.x = 50;
				state->player.y = 50;
				scripts.level_index = 0;
				File::level_name = scripts.levels[scripts.level_index];
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