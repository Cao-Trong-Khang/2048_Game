#pragma once
#include "ConsoleGraphics.h"
#include "Mutex.h"

extern mutex m;

using namespace std;

void readSettingFile(int& level, int& goal, int& undo_redo, int& speed, int& countdown);

void writeSettingFile(int level, int goal, int undo_redo, int speed, int countdown);

void SettingScene(Scene &scene);


