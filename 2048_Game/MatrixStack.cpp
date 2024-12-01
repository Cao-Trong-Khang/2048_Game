#include "MatrixStack.h"

unsigned char log2(int n)	// Hàm tính log2 cho một số nguyên có dạng 2^k;
{
	unsigned char exp = 0;
	while (n > 1)
	{
		n /= 2;
		exp++;
	}
	return exp;
}

Node* makeNode1(int** matrix, int n, unsigned short int score)   // Hàm tạo node lưu ma trận 
{
	Node* new_nod = (Node*)calloc(sizeof(Node*) + sizeof(score) + n * n , 1);
	if (new_nod != NULL)
	{
		new_nod->next = NULL;
		new_nod->score = score;
		int nExp = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				new_nod->expArr[nExp++] = log2(matrix[i][j]);
	}
	return (Node*)new_nod;
}

bool matrixStack::empty()
{
	return this->size == 0;
}

void matrixStack::pop1(int** matrix, int n, unsigned int& score)    // Lấy ma trận ra khỏi stack và lưu vào matrix 
{
	if (this->size == 0)
		return;
	int nExp = 0;
	score = head->score;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			matrix[i][j] = pow(2, this->head->expArr[nExp++]);
			if (matrix[i][j] == 1)
				matrix[i][j] = 0;
		}
	Node* tmp = this->head;
	this->head = this->head->next;
	delete[] tmp;
	this->size--;
}

void matrixStack::push1(int** matrix, int n, unsigned int score)   // Đẩy matrix vào trong stack 
{
	Node* new_nod = makeNode1(matrix, n, score);
	new_nod->next = this->head;
	this->head = new_nod;
	this->size++;
}

Node* makeNode2(unsigned char expMatrix[], int nExp, unsigned short int score)
{
	Node* new_nod = (Node*)calloc(sizeof(Node*) + sizeof(unsigned int) + nExp, 1);
	if (new_nod != NULL)
	{
		new_nod->next = NULL;
		new_nod->score = score;
		for (int i = 0; i < nExp; i++)
			new_nod->expArr[i] = expMatrix[i];
	}
	return new_nod;
}

void matrixStack::pop2(unsigned char expMatrix[], int nExp, unsigned int& score)		// Lấy ma trận số mũ ra khỏi Stack và lưu vào expMatrix 
{
	if (this->size == 0)
		return;
	score = head->score;
	for (int i = 0; i < nExp; i++)
		expMatrix[i] = this->head->expArr[i];
	Node* tmp = this->head;
	this->head = this->head->next;
	delete[] tmp;
	this->size--;
}

void matrixStack::push2(unsigned char expMatrix[], int nExp, unsigned int score)
{
	Node* new_nod = makeNode2(expMatrix, nExp, score);
	new_nod->next = this->head;
	this->head = new_nod;
	this->size++;
}

void matrixStack::free()
{
	while (this->head != NULL)
	{
		Node* tmp = this->head;
		this->head = tmp->next;
		delete[] tmp;
	}
	this->size = 0;
}