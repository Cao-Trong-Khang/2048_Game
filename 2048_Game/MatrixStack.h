#pragma once
#include <iostream>

using namespace std;

struct Node
{
	Node* next;
	unsigned short int score;
	unsigned char expArr[1];		// expArr mảng lưu số mũ ma trận, ma trận 4x4 thông thường cần 4x4x4 = 64 byte lưu trữ 
};						// sử dụng mảng expArr chỉ cần 8 byte lưu trữ, 1 phần tử expArr (1 byte) lưu hai số mũ của hai phần tử liên tiếp trong ma trận 

struct matrixStack     // Stack lưu trữ các ma trận 
{
	Node* head;
	int size;
	bool empty();
	void pop1(int** matrix, int n, unsigned int& score);
	void push1(int** matrix, int n, unsigned int score);
	void pop2(unsigned char expMatrix[], int nExp, unsigned int& score);
	void push2(unsigned char expMatrix[], int nExp, unsigned  int score);
	void free();
	matrixStack()
	{
		this->head = NULL;
		this->size = 0;
	}
};

