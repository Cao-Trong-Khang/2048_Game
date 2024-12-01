#include "gameFunction.h"

// Tạo ma trận và random hai số 

void makeMatrix(int**& matrix, int n)
{
	matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0;
	// Random 2 phần tử mới cho ma trận 
	srand((unsigned)time(NULL));
	int i1, j1, i2, j2;
	i1 = rand() % n;
	j1 = rand() % n;
	i2 = rand() % n;
	j2 = rand() % n;
	while (i1 == i2 && j1 == j2)
	{
		i2 = rand() % n;
		j2 = rand() % n;
	}
	matrix[i1][j1] = 2 + (rand() % 2) * 2;
	matrix[i2][j2] = 2 + (rand() % 2) * 2;
}

// Giải phóng bộ nhớ 

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

bool processLeft(int** matrix, int n, unsigned int& score)
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
				score += matrix[z][idex];
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
				i = idex - 1;
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

bool processRight(int** matrix, int n, unsigned int& score)
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
				score += matrix[z][idex];
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
				i = idex + 1;
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

bool processUp(int** matrix, int n, unsigned int& score)
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
				score += matrix[idex][z];
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
				i = idex - 1;
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
	
bool processDown(int** matrix, int n, unsigned int& score)
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
				score += matrix[idex][z];
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
				i = idex + 1;
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

// Hàm random phần tử mới trong mảng

void randomItem(int** matrix, int n)
{
	// Random phần tử mới cho ma trận 
	cell* cells = (cell*) calloc(n*n, sizeof(cell));		// Mảng lưu các ô rỗng 
	if (cells == NULL)
		return;
	int ncell = 0;
	for(int row = 0; row < n; row++)
		for (int col = 0; col < n; col++)
			if (matrix[row][col] == 0)
			{
				cells[ncell].x = row;
				cells[ncell].y = col;
				ncell++;
			}
	srand((unsigned)time(NULL));
	int value = 2 + (rand() % 2)*2;							// value nhận giá trị 2 hoặc 4 
	int random = rand() % ncell;							// random phần tử trong mảng rỗng 
	matrix[cells[random].x][cells[random].y] = value; 
	free(cells);											// giải phóng bộ nhớ lưu mảng ô rỗng 
}

// Hàm kiểm tra game over 

bool checkGOV(int** matrix, int n)
{	
	for(int row = 0; row < n; row++)
		for (int col = 0; col < n; col++)
		{
			if (matrix[row][col] == 0)
				return false;
			if (row - 1 >= 0 && matrix[row][col] == matrix[row - 1][col] && matrix[row][col] != -1)
				return false;
			if (row + 1 < n && matrix[row][col] == matrix[row + 1][col] && matrix[row][col] != -1)
				return false;
			if (col - 1 >= 0 && matrix[row][col] == matrix[row][col - 1] && matrix[row][col] != -1)
				return false;
			if (col + 1 < n && matrix[row][col] == matrix[row][col + 1] && matrix[row][col] != -1)
				return false;
		}
	return true;
}

void processUndo(int xo, int yo, int** matrix, int n, int undo_redo, matrixStack& undo, matrixStack& redo, User& user, Time time, int goal, Top20List list) 			// Tính năng quay về bước trước 
{
	if (undo_redo == 0)
	{
		m.lock();
		printText(125, 19, "UNDO IS TURNED OFF", 12, 0);
		m.unlock();
	}
	else if (undo.empty())
	{
		m.lock();
		printText(124, 19, "CANNOT UNDO ANYMORE", 14, 0);
		m.unlock();
	}
	else
	{
		if(undo_redo == 2)		
			redo.push1(matrix, n, user.score);
		undo.pop1(matrix, n, user.score);
		m.lock();
		printText(139, 19, "UNDO", 14, 0);
		m.unlock();
		printMatrix(matrix, n, xo, yo);
		printInfoUser(115, 21, goal, list, user, time);
	}
}

void processRedo(int xo, int yo, int** matrix, int n, int undo_redo, matrixStack& undo, matrixStack& redo, User& user, Time time, int goal, Top20List list)			// Tính năng quay về bước sau 
{
	if (undo_redo == 0 || undo_redo == 1)
	{
		m.lock();
		printText(125, 19, "REDO IS TURNED OFF", 12, 0);
		m.unlock();
	}
	else if (redo.empty())
	{
		m.lock();
		printText(124, 19, "CANNOT REDO ANYMORE", 14, 0);
		m.unlock();
	}
	else
	{
		undo.push1(matrix, n, user.score);
		redo.pop1(matrix, n, user.score);
		m.lock();
		printText(139, 19, "REDO", 14, 0);
		m.unlock();
		printMatrix(matrix, n, xo, yo);
		printInfoUser(115, 21, goal, list, user, time);
	}
}

bool checkWin(int** matrix, int n, int goal)	// Hàm kiểm tra win game
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (matrix[i][j] == goal)
				return true;
	return false;
}

void saveAccount(string fileName, User user, matrixStack& undo, matrixStack& redo, int** matrix, int n, int goal, int undo_redo, int speed, int countdown)	// Lưu thông tin người chơi vào các account  
{
	fstream file;
	unsigned char expMatrix[100];
	int nExp = n * n, check = 1;

	file.open(fileName, ios::out | ios::binary | ios::trunc);
	file.write((char*)&check, sizeof(int));			// check - 4 byte 
	file.write((char*)user.name.c_str(), sizeof(string));				// user name - 40 byte 
	file.write((char*)&user.score, sizeof(unsigned int));			// user score - 4 byte 
	file.write((char*)&user.time.hour, sizeof(Time));				// time - 8 byte 
	file.write((char*)&n, sizeof(n));								// n - 4 byte 
	file.write((char*)&goal, sizeof(goal));							// goal - 4 byte 
	file.write((char*)&undo_redo, sizeof(undo_redo));				// undo_redo - 4 byte 
	file.write((char*)&speed, sizeof(speed));						// speed - 4 byte 
	file.write((char*)&countdown, sizeof(countdown));				// countdown - 4 byte 
	// lưu ma trận hiện tại - lưu từng dòng  
	for (int i = 0; i < n; i++)  
		file.write((char*)matrix[i], sizeof(int) * n);  
	 
	file.write((char*)&undo.size, sizeof(undo.size));				// số phần tử undo  
	while (!undo.empty())											// lưu undo  
	{
		undo.pop2(expMatrix, nExp, user.score); 
		file.write((char*)&user.score, sizeof(user.score)); 
		file.write((char*)expMatrix, sizeof(unsigned char) * 100); 
	}
	file.write((char*)&redo.size, sizeof(redo.size));				// số phần tử redo  
	while (!redo.empty())											// lưu redo  
	{ 
		redo.pop2(expMatrix, nExp, user.score); 
		file.write((char*)&user.score, sizeof(user.score)); 
		file.write((char*)expMatrix, sizeof(unsigned char) * 100); 
	} 
	file.close();
	freeMatrix(matrix, n);									// Giải phóng bộ nhớ 
}

void readAccount(string fileName, User& user, matrixStack& undo, matrixStack& redo, int**& matrix, int& n, int& goal, int& undo_redo, int& speed, int& countdown)	// đọc thông tin người chơi từ các account
{
	fstream file;
	matrixStack tmp;
	unsigned char expMatrix[100];
	int nExp, check, size;
	unsigned int score;
	
	file.open(fileName, ios::in | ios::binary);
	file.read((char*)&check, sizeof(int));							// check - 4 byte 
	file.read((char*)user.name.c_str(), sizeof(string));			// user name - 40 byte 
	file.read((char*)&user.score, sizeof(unsigned int));			// user score - 4 byte 
	file.read((char*)&user.time.hour, sizeof(Time));				// time - 8 byte 
	file.read((char*)&n, sizeof(n));								// n - 4 byte
	file.read((char*)&goal, sizeof(goal));							// goal - 4 byte
	file.read((char*)&undo_redo, sizeof(undo_redo));				// undo_redo - 4 byte
	file.read((char*)&speed, sizeof(speed));						// speed - 4 byte
	file.read((char*)&countdown, sizeof(countdown));				// countdown - 4 byte
	nExp = n * n;
	matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	// đoc ma trận hiện tại - đọc từng dòng 
	for (int i = 0; i < n; i++)	
		file.read((char*)matrix[i], sizeof(int) * n);

	file.read((char*)&size, sizeof(size));				// số phần tử undo 
	for (int i = 0; i < size; i++)
	{
		file.read((char*)&score, sizeof(score));
		file.read((char*)expMatrix, sizeof(unsigned char) * 100);
		tmp.push2(expMatrix, nExp, score);
	}
	while (!tmp.empty())						// Đảo ngược để được stack undo 
	{
		tmp.pop2(expMatrix, nExp, score);
		undo.push2(expMatrix, nExp, score);
	}
	file.read((char*)&size, sizeof(size));				// số phần tử redo 
	for (int i = 0; i < size; i++)
	{
		file.read((char*)&score, sizeof(score));
		file.read((char*)expMatrix, sizeof(unsigned char) * 100);
		tmp.push2(expMatrix, nExp, score);
	}
	while (!tmp.empty())						// Đảo ngược để được stack redo 
	{
		tmp.pop2(expMatrix, nExp, score);
		redo.push2(expMatrix, nExp, score);
	}
	file.close();
	check = 0;
	file.open(fileName, ios::out | ios::binary | ios::trunc);		// Đánh dấu file đã đọc và không còn dữ liệu 
	file.write((char*)&check, sizeof(check));
	file.close();
}

int emptyAccount()			// Hàm trả về index account rỗng -1 nếu không có account rỗng 
{
	int check, empty = 0;
	string fileName[6] = { "Account_1.bin", "Account_2.bin" , "Account_3.bin" , "Account_4.bin" , "Account_5.bin" , "Account_6.bin"};
	fstream file;
	for (int i = 0; i < 6; i++)
	{
		file.open(fileName[i], ios::in | ios::binary);
		if (!file.is_open())
		{
			ofstream newFile(fileName[i], ios::out | ios::binary);		// Khởi tạo file cho lần đầu tiên chạy chương trình 
			newFile.write((char*)&empty, sizeof(int));
			newFile.close();
			return i;
		}
		else
		{
			file.read((char*)&check, sizeof(check));
			file.close();
		}
		if (check == 0)
			return i;
	}
	return -1;
}

bool processExit(User user, matrixStack& undo, matrixStack& redo, int** matrix, int n, int goal, int undo_redo, int speed, int countdown, Top20List& list)		// Tính năng thoát ván game (lưu thông tin khi mở lại)
{
	bool agree = Question(60, 15, "  Do you really want to exit the game?  ");
	if (agree)
	{
		Scene dummy = menu;
		string fileName[6] = { "Account_1.bin", "Account_2.bin" , "Account_3.bin" , "Account_4.bin" , "Account_5.bin", "Account_6.bin" };
		int index = emptyAccount();
		clearScreen(1, 1, 41, 83);
		if (index == -1)
		{
			printNotice(35, 7, "SELECT AN ACCOUNT TO REPLACE");
			index = selectAccount(6, 13, 0, dummy, true);
			while (!Question(24, 20, "CONFIRMING  DELETION OF THIS ACCOUNT, ARE YOU SURE?"))
			{
				index = selectAccount(6, 13, 0, dummy, true);
			}
		}
		saveAccount(fileName[index], user, undo, redo, matrix, n, goal, undo_redo, speed, countdown);
		list.addUser(user);
		writeTopPlayerList(list);
		return true;
	}
	else
		return false; 
}

