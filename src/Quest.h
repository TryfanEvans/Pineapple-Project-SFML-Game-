#include "States/State.h"

//This will be default level, use polymorphism for unique ones.
//It should be possible to do most with base class
class Quest
{
public:
	State* state;

	//Victory Conditions
	bool isCleared(EntityVec &enemies)
	{
		return enemies.empty();
	};

	bool isItemAquired(bool grail)
	{
		return grail;
	};

	bool isButton()
	{
		static bool primed = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		{
			if (primed)
			{
				primed = false;

				return true;
			}
			primed = false;
		}
		else
		{
			primed = true;
		}
		return false;
	};
	//Whatever else I can think of
	//bool complete = false;
	std::vector<std::string> victory_conditions;

	Quest(State* state, std::vector<std::string> victory_conditions) :
		state(state),
		victory_conditions(victory_conditions) {};

	bool isComplete(EntityVec &enemies, bool grail)
	{
		bool complete = true;
		for (auto condition : victory_conditions)
		{
			if (condition == "Cleared")
			{
				if (!isCleared(enemies))
					complete = false;
			}
			if (condition == "ItemAquired")
			{
				if (!isItemAquired(grail))
					complete = false;
			}
			if (condition == "Button")
			{
				if (!isButton())
					complete = false;
			}
		}
		return complete;
	};

	void start()
	{
	}
};