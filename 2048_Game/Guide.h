#pragma once
#include "ConsoleGraphics.h"
#include "Mutex.h"

extern mutex m;

void GuideScene(Scene& scene);

void printGameRules(int x, int y);

void printControlGuide(int x, int y);
