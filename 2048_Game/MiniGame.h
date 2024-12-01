#pragma once
#include "ConsoleGraphics.h"
#include "gameFunction.h"

void miniGameScene(Scene& scene);  // Giao diện mini game 

void printDetail(int x, int y, int space, int size, int mode, int goal, int time, int countdown, int counter);  // In thông tin cho phần setting PvP 

void pvpScene(Scene& scene);  // Giao diện phần PvP của mini game 

void blocksScene(Scene& scene);  // Giao diện phần chướng ngại vật của mini game 

void playPvP(int size, int mode, int goal, int time, int countdown);  // chơi chế độ pvp 

void playBlocks(int** map, int n);  // chơi game chế độ chướng ngại vật 

void getNameTwoPlayer(int x, int y, string& name1, string& name2);  // Lấy tên hai người chơi của PvP 

void printInfoUser(int x, int y, User user, int size, int index);  

bool winScene(int winIndex);		// true - chơi tiếp , false - thoát , giao diện thắng game của PvP 

//-----------------------------
bool processLeftMiniGame(int** matrix, int n, unsigned int& score);

bool processRightMiniGame(int** matrix, int n, unsigned int& score);

bool processUpMiniGame(int** matrix, int n, unsigned int& score);

bool processDownMiniGame(int** matrix, int n, unsigned int& score);

