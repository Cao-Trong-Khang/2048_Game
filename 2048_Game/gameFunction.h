#pragma once
#include <stdlib.h>
#include <ctime>
#include "MatrixStack.h"
#include "ConsoleGraphics.h"
#include "TopPlayer.h"
#include "NewGame.h"
#include "Mutex.h"

extern mutex m;

void makeMatrix(int**& matrix, int n);	// Khởi tạo ma trận 

void freeMatrix(int** matrix, int n);	// Giải phóng bộ nhớ 

bool processLeft(int** matrix, int n, unsigned int& score);			// Tính năng dồn mảng qua trái và cộng các phần tử liền kề giống nhau 

bool processRight(int** matrix, int n, unsigned int& score);			// Tính năng dồn mảng qua phải và cộng các phần tử liền kề giống nhau

bool processUp(int** matrix, int n, unsigned int& score);				// Tính năng dồn mảng lên trên và cộng các phần tử liền kề giống nhau

bool processDown(int** matrix, int n, unsigned int& score);			// Tính năng dồn mảng xuống dưới và cộng các phần tử liền kề giống nhau

void randomItem(int** matrix, int n);			// Hàm random phần tử mới cho ma trận 

bool checkGOV(int** matrix, int n);				// Hàm kiểm tra game over 

bool checkWin(int** matrix, int n, int goal);	// Hàm kiểm tra win game

void saveAccount(string fileName, User user, matrixStack& undo, matrixStack& redo, int** matrix, int n, int goal, int undo_redo, int speed, int countdown);	// Lưu thông tin người chơi vào các account  

void readAccount(string fileName, User& user, matrixStack& undo, matrixStack& redo, int** &matrix, int& n, int& goal, int& undo_redo, int& speed, int& countdown);	// đọc thông tin người chơi từ các account  

int emptyAccount();		// Hàm trả về index account rỗng 

void processUndo(int xo, int yo, int** matrix, int n, int undo_redo, matrixStack& undo, matrixStack& redo, User& user, Time time, int goal, Top20List list);			// Tính năng quay về bước trước 

void processRedo(int xo, int yo, int** matrix, int n, int undo_redo, matrixStack& undo, matrixStack& redo, User& user, Time time, int goal, Top20List list);			// Tính năng quay về bước sau 

bool processExit(User user, matrixStack& undo, matrixStack& redo, int** matrix, int n, int goal, int undo_redo,int speed, int countdown, Top20List& list);		// Tính năng tạm dừng ván game (lưu thông tin khi mở lại)