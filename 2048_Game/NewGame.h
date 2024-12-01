#pragma once
#include "ConsoleGraphics.h"
#include "Setting.h"
#include "gameFunction.h"
#include "Mutex.h"

using namespace std;

extern mutex m;

void getUserName(int x, int y, string& name, Top20List list);	// Lấy tên người dùng 

bool checkUserName(string userName, Top20List list);	// Kiểm tra tên có trùng với top 10 không 

void printInfoUser(int x, int y, int goal, Top20List list, User user, Time time);  // In thông tin người chơi hiện tại 

void NewGameScene(Scene& scene, int accIndex);	// Hàm điều khiển hoạt động scene Newgame 

void WinGameScene(int x, int y, Scene& scene, int& goal, User user, Top20List list);		// Hàm thông báo win game 

void GameOverScene(int x, int y, Scene& scene, int& goal,int undo_redo, User user, Top20List list, int& mode);			// Hàm thông báo game over 

void countDown(int x, int y);       // Đếm ngược trước khi chơi 

bool checkResume();   // Hàm kiểm tra chức năng resume có sử dụng được hay không     

int** InitMatrix(int n);







           