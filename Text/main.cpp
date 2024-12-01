#include<iostream>
#include <windows.h>
#include <string>
#include <iostream>
#include <conio.h>

using namespace std;

bool processLeft(int** matrix, int n);			// Tính năng dồn mảng qua trái và cộng các phần tử liền kề giống nhau 

bool processRight(int** matrix, int n);			// Tính năng dồn mảng qua phải và cộng các phần tử liền kề giống nhau

bool processUp(int** matrix, int n);				// Tính năng dồn mảng lên trên và cộng các phần tử liền kề giống nhau

bool processDown(int** matrix, int n);			// Tính năng dồn mảng xuống dưới và cộng các phần tử liền kề giống nhau

void randomItem(int** matrix, int n);			// Hàm random phần tử mới cho ma trận 

bool checkGOV(int** matrix, int n);				// Hàm check game over

void makeMatrix(int**& matrix, int n);

void freeMatrix(int** matrix, int n);

void printMatrix(int** a, int n, int x, int y);

void gotoxy(int x, int y);



int main()
{
	//char ch;

	//std::cout << "Nhan phim mui ten de di chuyen (Nhan 'q' de thoat):\n";
	//while (true) {
	//	ch = _getch(); // Đọc một ký tự từ bàn phím mà không hiển thị lên màn hình
	//	cout << (int)ch << endl;
	//}
	int** a, n = 4;
	makeMatrix(a, n);
	bool done = false;
	randomItem(a, n);
	printMatrix(a, n, 0, 0);
	char k;
	while (true)
	{
		done = false;
		k = _getch();
		if (k == -32)
			continue;
		else if (k == 72)
			done = processUp(a, n);
		else if (k == 80)
			done = processDown(a, n);
		else if (k == 75)
			done = processLeft(a, n);
		else if (k == 77)
			done = processRight(a, n);
		if (!done)
		{
			gotoxy(30, 0);
			cout << "Khong hop le";
			Sleep(1000);
			gotoxy(30, 0);
			cout << "             ";
			continue;
		}
		printMatrix(a, n, 0, 0);
		randomItem(a, n);
		Sleep(1000);
		printMatrix(a, n, 0, 0);
		if (checkGOV(a, n))
		{
			gotoxy(30, 0);
			cout << "Game Over";
			break;
		}
	}
	freeMatrix(a, n);
}

int colorCode(int n)  // Hàm tìm mã màu cho một số 
{
	while (n > 64)
		n = n / (64);
	if (n == 2)
		return 46;
	else if (n == 4)
		return 42;
	else if (n == 8)
		return 43;
	else if (n == 16)
		return 41;
	else if (n == 32)
		return 45;
	else if (n == 64)
		return 44;
	else return 0;
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void printMatrix(int** matrix, int n, int x, int y)
{
	x += 2; y++;
	gotoxy(x, y++);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << "\x1b[" << colorCode(matrix[i][j]) << "m" << "        \x1b[0m";
			if (j != n - 1)
				cout << "  ";
		}
		gotoxy(x, y++);
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == 0)
				cout << "        ";
			else
			{
				cout << "\x1b[" << colorCode(matrix[i][j]) << "m";
				if (matrix[i][j] < 10)
					cout << "   " << matrix[i][j] << "    ";
				else if (matrix[i][j] < 100)
					cout << "   " << matrix[i][j] << "   ";
				else if (matrix[i][j] < 1000)
					cout << "  " << matrix[i][j] << "   ";
				else if (matrix[i][j] < 10000)
					cout << "  " << matrix[i][j] << "  ";
				else if (matrix[i][j] < 100000)
					cout << " " << matrix[i][j] << "  ";
				else
					cout << " " << matrix[i][j] << " ";
			}
			cout << "\x1b[0m";
			if (j != n - 1)
				cout << "  ";
		}
		gotoxy(x, y++);
		for (int j = 0; j < n; j++)
		{
			cout << "\x1b[" << colorCode(matrix[i][j]) << "m" << "        \x1b[0m";
			if (j != n - 1)
				cout << "  ";
		}
		y++;
		gotoxy(x, y++);
	}
	gotoxy(0, 0);
}

void makeMatrix(int**& matrix, int n)
{
	matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0;
	// Random 1 phần tử mới cho ma trận 
	srand((unsigned)time(NULL));
	int i, j;
	i = rand() % n;
	j = rand() % n;
	matrix[i][j] = 2 + (rand() % 2) * 2;
}

void freeMatrix(int** matrix, int n)
{
	if (matrix != NULL)
	{
		for (int i = 0; i < n; i++)
			if (matrix[i] != NULL)
				delete[] matrix[i];
		delete[] matrix;
	}
}

// Tính năng dồn mảng qua trái và cộng các phần tử liền kề giống nhau 

bool processLeft(int** matrix, int n)
{
	bool change = false;									// Biến đánh dấu ma trận đã thay đổi 
	for (int z = 0; z < n; z++)								// Duyệt từng dòng của ma trận 
	{
		int idex = 0, i, j;									// idex là chỉ số sẽ di chuyển từ trái qua phải để dồn các phần tử về trái  
		for (i = 0; i < n; i++)
		{
			if (matrix[z][i] == 0)								// Tìm phần tử khác 0 
				continue;
			for (j = i + 1; j < n && matrix[z][j] == 0; j++);	// Tìm phần tử khác 0 kế tiếp 
			if (j == n)										// Nếu không còn phần tử khác 0 thì dừng duyệt hàng 
				break;
			if (matrix[z][i] == matrix[z][j])							// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn về bên trái, tiếp tục duyệt mảng 
			{
				matrix[z][idex] = matrix[z][i] + matrix[z][j];
				if (i != idex)
					matrix[z][i] = 0;
				matrix[z][j] = 0;
				idex++;
				change = true;								// đánh dấu mảng đã thay đổi
			}
			else
			{
				if (i != idex)								// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này về trái và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
				{
					matrix[z][idex] = matrix[z][i];
					matrix[z][i] = 0;
					change = true;
				}
				idex++;
				if (j != idex)
				{
					matrix[z][idex] = matrix[z][j];
					matrix[z][j] = 0;
					change = true;
				}
				i = j - 1;
			}
		}
		if (i < n && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn về trái 
		{
			matrix[z][idex] = matrix[z][i];
			matrix[z][i] = 0;
			change = true;
		}
	}
	return change;
}

// Tính năng dồn mảng qua phải và cộng các phần tử liền kề giống nhau

bool processRight(int** matrix, int n)
{
	bool change = false;										// Biến đánh dấu ma trận đã thay đổi 
	for (int z = 0; z < n; z++)									// Duyệt từng dòng của ma trận 
	{
		int idex = n - 1, i, j;									// idex là chỉ số sẽ di chuyển từ phải qua trái để dồn các phần tử về phải   
		for (i = n - 1; i >= 0; i--)
		{
			if (matrix[z][i] == 0)									// Tìm phần tử khác 0 
				continue;
			for (j = i - 1; j >= 0 && matrix[z][j] == 0; j--);		// Tìm phần tử khác 0 kế tiếp 
			if (j == -1)										// Nếu không còn phần tử khác 0 thì dừng duyệt hàng 
				break;
			if (matrix[z][i] == matrix[z][j])								// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn về bên phải, tiếp tục duyệt mảng 
			{
				matrix[z][idex] = matrix[z][i] + matrix[z][j];
				if (i != idex)
					matrix[z][i] = 0;
				matrix[z][j] = 0;
				idex--;
				change = true;									// đánh dấu mảng đã thay đổi
			}
			else
			{
				if (i != idex)									// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này về phải và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
				{
					matrix[z][idex] = matrix[z][i];
					matrix[z][i] = 0;
					change = true;
				}
				idex--;
				if (j != idex)
				{
					matrix[z][idex] = matrix[z][j];
					matrix[z][j] = 0;
					change = true;
				}
				i = j + 1;
			}
		}
		if (i >= 0 && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn về phải  
		{
			matrix[z][idex] = matrix[z][i];
			matrix[z][i] = 0;
			change = true;
		}
	}
	return change;
}

// Tính năng dồn mảng lên trên và cộng các phần tử liền kề giống nhau

bool processUp(int** matrix, int n)
{
	bool change = false;									// Biến đánh dấu ma trận đã thay đổi 
	for (int z = 0; z < n; z++)								// Duyệt từng cột của ma trận 
	{
		int idex = 0, i, j;									// idex là chỉ số sẽ di chuyển từ trên xuống dưới để dồn các phần tử lên trên   
		for (i = 0; i < n; i++)
		{
			if (matrix[i][z] == 0)								// Tìm phần tử khác 0 
				continue;
			for (j = i + 1; j < n && matrix[j][z] == 0; j++);	// Tìm phần tử khác 0 kế tiếp 
			if (j == n)										// Nếu không còn phần tử khác 0 thì dừng duyệt cột 
				break;
			if (matrix[i][z] == matrix[j][z])							// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn lên trên, tiếp tục duyệt mảng 
			{
				matrix[idex][z] = matrix[i][z] + matrix[j][z];
				if (i != idex)
					matrix[i][z] = 0;
				matrix[j][z] = 0;
				idex++;
				change = true;								// đánh dấu mảng đã thay đổi
			}
			else
			{
				if (i != idex)								// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này lên trên và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
				{
					matrix[idex][z] = matrix[i][z];
					matrix[i][z] = 0;
					change = true;
				}
				idex++;
				if (j != idex)
				{
					matrix[idex][z] = matrix[j][z];
					matrix[j][z] = 0;
					change = true;
				}
				i = j - 1;
			}
		}
		if (i < n && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn lên trên  
		{
			matrix[idex][z] = matrix[i][z];
			matrix[i][z] = 0;
			change = true;
		}
	}
	return change;
}

// Tính năng dồn mảng xuống dưới và cộng các phần tử liền kề giống nhau		

bool processDown(int** matrix, int n)
{
	bool change = false;										// Biến đánh dấu ma trận đã thay đổi 
	for (int z = 0; z < n; z++)									// Duyệt từng cột của ma trận 
	{
		int idex = n - 1, i, j;									// idex là chỉ số sẽ di chuyển từ dưới lên trên để dồn các phần tử xuống dưới   
		for (i = n - 1; i >= 0; i--)
		{
			if (matrix[i][z] == 0)									// Tìm phần tử khác 0 
				continue;
			for (j = i - 1; j >= 0 && matrix[j][z] == 0; j--);		// Tìm phần tử khác 0 kế tiếp 
			if (j == -1)										// Nếu không còn phần tử khác 0 thì dừng duyệt cột  
				break;
			if (matrix[i][z] == matrix[j][z])								// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn xuống dưới, tiếp tục duyệt mảng 
			{
				matrix[idex][z] = matrix[i][z] + matrix[j][z];
				if (i != idex)
					matrix[i][z] = 0;
				matrix[j][z] = 0;
				idex--;
				change = true;									// đánh dấu mảng đã thay đổi
			}
			else
			{
				if (i != idex)									// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này xuống dưới và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
				{
					matrix[idex][z] = matrix[i][z];
					matrix[i][z] = 0;
					change = true;
				}
				idex--;
				if (j != idex)
				{
					matrix[idex][z] = matrix[j][z];
					matrix[j][z] = 0;
					change = true;
				}
				i = j + 1;
			}
		}
		if (i >= 0 && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn xuống dưới  
		{
			matrix[idex][z] = matrix[i][z];
			matrix[i][z] = 0;
			change = true;
		}
	}
	return change;
}

struct cell
{
	int x, y;
};

// Hàm random phần tử mới trong mảng

void randomItem(int** matrix, int n)
{
	// Random phần tử mới cho ma trận 
	cell* cells = (cell*)calloc(n * n, sizeof(cell));		// Mảng lưu các ô rỗng 
	if (cells == NULL)
		return;
	int ncell = 0;
	for (int row = 0; row < n; row++)
		for (int col = 0; col < n; col++)
			if (matrix[row][col] == 0)
			{
				cells[ncell].x = row;
				cells[ncell].y = col;
				ncell++;
			}
	srand((unsigned)time(NULL));
	int value = 2 + (rand() % 2) * 2;							// value nhận giá trị 2 hoặc 4 
	int random = rand() % ncell;							// random phần tử trong mảng rỗng 
	matrix[cells[random].x][cells[random].y] = value;
	ncell--;												// trừ đi 1 ô rỗng 
	free(cells);											// giải phóng bộ nhớ lưu mảng ô rỗng 
}

// Hàm kiểm tra game over 

bool checkGOV(int** matrix, int n)
{
	// Nếu còn ô rỗng hoặc duyệt ma trận nếu tồn tại hai ô liền kề (có cạnh chung) có cùng giá trị thì có thể chơi tiếp 
	for (int row = 0; row < n; row++)
		for (int col = 0; col < n; col++)
		{
			if (matrix[row][col] == 0)
				return false;
			if (row - 1 >= 0 && matrix[row][col] == matrix[row - 1][col])
				return false;
			if (row + 1 < n && matrix[row][col] == matrix[row + 1][col])
				return false;
			if (col - 1 >= 0 && matrix[row][col] == matrix[row][col - 1])
				return false;
			if (col + 1 < n && matrix[row][col] == matrix[row][col + 1])
				return false;
		}
	return true;
}

