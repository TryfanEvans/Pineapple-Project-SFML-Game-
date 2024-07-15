#ifndef SaveManager_H
#define SaveManager_H
#include "File.h"

namespace SaveManager
{
extern bool isWin();
extern std::vector<std::string> levels;
extern uint level_index;
extern uint level_unlock;

extern void newGame();

//Load current level_unlock from file
extern void continueGame();

extern void loadGame(uint level);

extern void completeLevel();
}
#endif