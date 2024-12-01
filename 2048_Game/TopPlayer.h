#pragma once
#include "ConsoleGraphics.h";
#include "Mutex.h"

extern mutex m;
struct node
{
	User user;
	node* next = NULL;
};

struct Top20List
{
	node* top1 = NULL;
	int nUser = 0;
	void addUser(User user);   // Thêm một user mà vẫn giữ nguyên tính sắp xếp và số lượng user vẫn đảm bảo <= 20 
	void free();			   // Giải phóng bộ nhớ 
};

void printTop20(int x, int y, Top20List list);   // In Top 20

void writeTopPlayerList(Top20List& list);  // Ghi file topplayer.bin 

void readTopPlayerList(Top20List& list);  // Đọc file topplayer.bin - top 20 player cao điểm nhất 

void TopPlayerScene(Scene &scene);




