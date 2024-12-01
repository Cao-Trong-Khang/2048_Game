#include "NewGame.h"

void getUserName(int x, int y, string& name, Top20List list)
{
	bool run = true;
	string title = "NEW GAME";
	thread thr(printTitle, 38, 8, title, ref(run), 11, 14);
	printNotice(x, y, "ENTER YOUR NAME");
	warning();
	char key;
	m.lock();
	SetColor(0, 14);
	gotoxy(x, y + 3);
	cout << char(175);
	gotoxy(x + 18, y + 3);
	cout << char(174);
	m.unlock();
	name = "";
	while (start)
	{
		key = _getch();
		clearScreen(x - 18, y + 5, 1, 58);
		if (key == '\r')
		{
			int begin, end;
			string tmp_name;
			for (begin = 0; begin < name.size() && name[begin] == ' '; begin++);		// Lược bỏ dấu cách 
			for (end = name.size() - 1; end >= 0 && name[end] == ' '; end--);
			if (begin > end || (int)name.size() == 0)
				tmp_name = "";
			else
				tmp_name = name.substr(begin, end - begin + 1);

			if (tmp_name == "")
			{
				m.lock();
				gotoxy(x - 10, y + 5);
				SetColor(0, 12);
				cout << "Invalid name. Please enter a valid name.";
				m.unlock();
				continue;
			}
			else if(checkUserName(tmp_name, list))
			{
				m.lock();
				gotoxy(x - 18 , y + 5);
				SetColor(0, 12);
				cout << "This name is already taken. Please choose a different one.";
				m.unlock();
				continue;
			}
			else
			{
				run = false;
				if (thr.joinable())
					thr.join();
				name = tmp_name;
				m.lock();
				SetColor(0, 14);
				for (int i = 0; i < 8; i++)
				{
					gotoxy(x + 17 - i, y + 3);
					cout << char(174) << " ";
					gotoxy(x + i, y + 3);
					cout << " " << char(175);
					Sleep(80);
				}
				m.unlock();
				SetColor(0, 15);
				break;
			}
		}
		if (key == prefix_value)		// Không nhận phím mũi tên 
		{
			key = _getch();
			gotoxy(x - 8, y + 5);
			SetColor(0, 12);
			cout << "Please enter only letters and numbers.";
			continue;
		}
		if (key == 8)  // Phím xóa 
		{
			if (name.size() > 0)
			{
				if (name.size() == 1)
					name = "";
				else
					name = name.substr(0, name.size() - 1);
			}
			else
				continue;
		}
		else if (((key < 65 || key > 90) && (key < 97 || key > 122) && key != ' ' && (key < 48 || key > 59)) || name.size() >= 15)	// Bỏ các kí tự khác bảng chữ cái 
		{
			m.lock();
			if (name.size() >= 15)
			{
				gotoxy(x - 7, y + 5);
				SetColor(0, 12);
				cout << "Name must be 15 characters or less.";
			}
			else
			{
				gotoxy(x - 12, y + 5);
				SetColor(0, 12);
				cout << "Please enter only alphabet letters or numbers.";
			}
			m.unlock();
			continue;
		}
		else
			name = name + char(key);
		m.lock();
		SetColor(0, 11);
		gotoxy(x + 2, y + 3);
		for (int i = 0; i < (int)name.size() - 1; i++)
			cout << name[i];
		SetColor(0, 12);
		if(name.size() > 0)
			cout << name[name.size() - 1];
		for (int i = name.size(); i < 15; i++)
			cout << " ";
		m.unlock();
	}
	clearScreen(x, y, 4, 19);		// Xóa bảng thông báo  
	SetColor(0, 15);				
}

bool checkUserName(string userName, Top20List list)   // true - trùng , false không trùng 
{
	node* tmp = list.top1;
	while (tmp != NULL)
	{
		if (tmp->user.name == userName)
			return true;
		tmp = tmp->next;
	}
	User user[6];
	string fileName[6] = { "Account_1.bin", "Account_2.bin", "Account_3.bin", "Account_4.bin", "Account_5.bin" , "Account_6.bin" };
	fstream file;
	int check, empty = 0;
	for (int i = 0; i < 6; i++) 
	{
		file.open(fileName[i], ios::in | ios::binary);
		if (!file.is_open())
		{
			ofstream newFile(fileName[i], ios::out | ios::binary);		// Khởi tạo file cho lần đầu tiên chạy chương trình 
			newFile.write((char*)&empty, sizeof(int));
			newFile.close();
		}
		else
		{
			file.read((char*)&check, sizeof(check));
			if (check)
			{
				file.read((char*)user[i].name.c_str(), sizeof(string));
				file.read((char*)&user[i].score, sizeof(user[i].score));
				file.read((char*)&user[i].time.hour, sizeof(user[i].time));
			}
			file.close();
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (userName == user[i].name)
			return true;
	}
	return false;
}

void Plus(Time& time, int second)
{
	time.second += second;
	if (time.second >= 60)
	{
		time.minute += time.second / 60;
		time.second = time.second % 60;
		if (time.minute >= 60)
		{
			time.hour += time.minute / 60;
			time.minute = time.minute % 60;
		}
	}
}

void getRank(Top20List list, User user, int& rank)
{
	rank = 1;
	node* tmp = list.top1;
	while (tmp != NULL)
	{
		if (user.name == tmp->user.name)
			break;
		if ((user.score > tmp->user.score) || (user.score == tmp->user.score && timetoint(user.time) < timetoint(tmp->user.time)))
			break;
		rank++;
		tmp = tmp->next;
	}
}

void WinGameScene(int x, int y, Scene& scene, int& goal, User user, Top20List list)			// Hàm thông báo win game
{
	string win = "WIN";
	bool run = true;
	thread thr(printTitle, x, y, win, ref(run), 11, 14);
	char key;
	int mode = 1, rank;
	getRank(list, user, rank);
	m.lock();
	SetColor(0, 14);
	gotoxy(x + 2, y + 7);
	cout << char(219) << " " << user.name;
	gotoxy(x + 2, y + 8);
	cout << char(219);
	SetColor(0, 11);
	gotoxy(x + 4, y + 8);
	cout << "RANK: ";
	if (rank <= 20)
		cout << rank;
	else
		cout << "20+";
	gotoxy(x + 14, y + 8);
	cout << "SCORE: " << user.score;
	m.unlock();
	while (start)
	{
		printButton(x - 4, y + 10, " BACK MENU ", false, 11, 14);
		printButton(x + 16, y + 10, " CONTINUE  ", false, 11, 14);
		if(mode == 0)
			printButton(x - 4, y + 10, " BACK MENU ", true, 11, 14);
		else
			printButton(x + 16, y + 10, " CONTINUE  ", true, 11, 14);
		key = _getch();
		if (key == Right)
			mode = 1;
		else if (key == Left)
			mode = 0;
		else if (key == Enter)
			break;
	}
	if (mode == 1)
	{
		if (goal == 2048)
			goal = 4096;
		else if (goal == 4096)
			goal = 8192;
		else
			goal = -1;
	}
	else
		scene = menu;
	run = false;
	if (thr.joinable())
		thr.join();
}

void GameOverScene(int x, int y, Scene& scene, int& goal, int undo_redo, User user, Top20List list, int& mode)			// Hàm thông báo game over
{
	string loss = "LOSS";
	bool run = true;
	thread thr(printTitle, x - 6, y, loss, ref(run), 11, 12);
	char key;
	int rank;
	getRank(list, user, rank);
	m.lock();
	SetColor(0, 11);
	gotoxy(x + 2, y + 7);
	cout << char(219) << " " << user.name;
	gotoxy(x + 2, y + 8);
	cout << char(219);
	SetColor(0, 12);
	gotoxy(x + 4, y + 8);
	cout << "RANK: ";
	if (rank <= 20)
		cout << rank;
	else
		cout << "20+";
	gotoxy(x + 14, y + 8);
	cout << "SCORE: " << user.score;
	m.unlock();
	while (start)
	{
		printButton(x - 9, y + 10, " BACK MENU ", false, 11, 12);
		printButton(x + 9, y + 10, " NEW GAME ", false, 11, 12);
		if(undo_redo != 0)
			printButton(x + 25, y + 10, " UNDO ", false, 11, 12);
		else
			printButton(x + 25, y + 10, " UNDO ", false, 8, 8);
		if (mode == 0)
			printButton(x - 9, y + 10, " BACK MENU ", true, 11, 12);
		else if(mode == 1)
			printButton(x + 9, y + 10, " NEW GAME ", true, 11, 12);
		else
			printButton(x + 25, y + 10, " UNDO ", true, 11, 12);
		key = _getch();
		if (key == Right)
		{
			mode++;
			if (undo_redo != 0)
				mode %= 3;
			else
				mode %= 2;
		}
		else if (key == Left)
		{
			if (mode > 0)
				mode--;
			else
			{
				if (undo_redo != 0)
					mode = 2;
				else
					mode = 1;
			}
		}
		else if (key == Enter)
			break;
	}
	if (mode == 0)
		scene = menu;
	else if(mode == 1)
		scene = newgame;
	run = false;
	if (thr.joinable())
		thr.join();
}

int** InitMatrix(int n)
{
	int** matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrix[i][j] = 0;
	return matrix;
}

void CopyMatrix(int** dest, int** src, int n)
{
	if (dest != NULL && src != NULL)
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				dest[i][j] = src[i][j];
	}
}

void printBestScore(int score)
{
	m.lock();
	SetColor(0, 11);
	gotoxy(133, 16);
	cout << "BEST SCORE " << char(219);
	gotoxy(133, 17);
	SetColor(0, 0);
	for (int i = 1; i <= 10 - Count(score); i++)
		cout << " ";
	SetColor(0, 14);
	cout << score;
	SetColor(0, 11);
	cout << " " << char(219);
	m.unlock();
}

void printInfoUser(int x, int y, int goal, Top20List list, User user, Time time)
{
	int rank;
	user.time = time;
	Plus(user.time, 1);
	getRank(list, user, rank);
	if (list.top1 != NULL && list.top1->user.score > user.score)
		printBestScore(list.top1->user.score);
	else
		printBestScore(user.score);
	m.lock();
	SetColor(0, 11);
	gotoxy(144, 19);
	cout << char(219);
	gotoxy(x + 5, y + 4);
	cout << "RANK  : ";
	gotoxy(x + 5, y + 6);
	cout << "GOAL  : ";
	gotoxy(x + 5, y + 8);
	cout << "SCORE : ";
	SetColor(0, 14);
	gotoxy(x + 13, y + 4);
	if (rank > 20)
		cout << " 20+";
	else
		cout << " " << rank << "  ";
	gotoxy(x + 13, y + 6);
	if (goal == -1)
		cout << " Infinite";
	else
		cout << " " << goal << "    ";
	gotoxy(x + 13, y + 8);
	cout << " " << user.score;
	for (int i = 1; i <= 11 - Count(user.score); i++)
		cout << " ";
	m.unlock();
}

void copy(int** dest, int src[10][10], int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			dest[i][j] = src[i][j];
}

void NewGameScene(Scene& scene, int accIndex)
{
	User user;
	matrixStack undo, redo;	
	Top20List list;
	Time time;
	int** matrix, **pre_matrix, n, goal, undo_redo, speed,countdown, xo, yo;
	char button;
	bool change;

	SetColor(0, 0);
	system("cls");
	makeFrames();
	bool run = true, pause = false;
	string title = "2 0 4 8";
	readTopPlayerList(list);
	if(accIndex == -1)		// Người chơi sử dụng New Game để chơi 
	{
		getUserName(65, 20, user.name, list);		// Lấy thông tin người chơi
		clearScreen(38, 8, 6, 75);					// Xóa chữ NEW GAME
		readSettingFile(n, goal, undo_redo, speed, countdown);		// Lấy thông tin chế độ chơi 
		makeMatrix(matrix, n);
	}
	else					// Người chơi sử dụng Resume để chơi 
	{	
		string fileName = "Account_" + to_string(accIndex + 1) + ".bin";
		readAccount(fileName, user, undo, redo, matrix, n, goal, undo_redo, speed, countdown);
	}
	xo = 2 + (10 - n) * 6, yo = 1 + (10 - n) * 2;     // Tọa độ ma trận 	
	time = user.time;
	if(countdown)
		countDown(65, 15);
	thread thr(printTitle, 100, 3, title, ref(run), 11, 14);
	thread clo(Clock, 123, 11, ref(time), ref(run), ref(pause));
	//--Test hàm win------------------------------------------				
	/*int a[10][10] = { {1024, 1024, 1024, 1024}, {1024, 1024, 1024, 1024 } , {1024, 1024, 1024, 1024}, {1024, 1024, 1024, 1024 } };
	copy(matrix, a, n);*/
	//--------------------------------------------------------	
	//--Test ham loss-----------------------------------------
	/*int a[10][10] = { {4, 8, 16, 32}, {32, 16, 4, 8 } , {4, 8, 16, 32}, {0, 32, 16, 8} };
	copy(matrix, a, n);*/
	//--------------------------------------------------------
	printFrameMatrix(xo, yo, n);		
	printMatrix(matrix, n, xo, yo);				
	structionBoard(115, 21, user.name, 3, 15 - user.name.size());								
	printInfoUser(115, 21, goal, list, user, time);		
	change = false;		
	pre_matrix = InitMatrix(n);		
	while (start)   
	{		
		if (_kbhit())
		{
			button = _getch();
		}
		else
		{
			printInfoUser(115, 21, goal, list, user, time);
			continue;
		}		
		clearScreen(124, 19, 1, 20);
		if (button == prefix_value)						// Loại bỏ giá trị tiền đề (prefix value) cho các phím mũi tên 
			button = _getch();					
		if(undo_redo != 0)								// Có đang bật chức năng undo redo		
			CopyMatrix(pre_matrix, matrix, n);			// Lưu matrix hiện tại qua pre_matrix	
		if(button == Up || button == W)	  
			change = processUp(matrix, n, user.score);	  
		else if (button == Down || button == S)    
			change = processDown(matrix, n, user.score);	
		else if (button == Left || button == A)	   
			change = processLeft(matrix, n, user.score);	
		else if (button == Right || button == D)	 
			change = processRight(matrix, n, user.score);	 
		else if (button == Undo)	
		{	
			processUndo(xo, yo, matrix, n, undo_redo, undo, redo,user, time, goal, list);
			continue;
		} 
		else if (button == Redo)
		{
			processRedo(xo, yo, matrix, n, undo_redo, undo, redo, user, time, goal, list);
			continue;
		}
		else if (button == Exit)
		{
			pause = true;
			user.time = time;
			Plus(user.time, 1);
			if (processExit(user, undo, redo, matrix, n, goal, undo_redo, speed, countdown, list))
			{
				run = false;
				if (thr.joinable())
					thr.join();
				if (clo.joinable())
					clo.join();
				scene = menu;
				freeMatrix(pre_matrix, n);
				return;
			}
			else
			{
				if(countdown)
				{
					clearScreen(1, 1, 41, 83);
					countDown(40, 15);
				}
				pause = false;
				printFrameMatrix(xo, yo, n);
				printMatrix(matrix, n, xo, yo);
				continue;
			}
		}
		if (change == false)	// Nếu thao tác người chơi không làm thay đổi ma trận thì thong báo nước đi không hợp lệ 
		{
			m.lock();
			printText(131, 19, "INVALID MOVE", 12, 0);
			m.unlock();
			continue;
		}
		change = false;
		if (undo_redo != 0)
		{
			undo.push1(pre_matrix, n, user.score);
			redo.free();
		}
		printMatrix(matrix, n, xo, yo);
		printInfoUser(115, 21, goal, list, user, time);
		randomItem(matrix, n);
		Sleep(speed);
		printMatrix(matrix, n, xo, yo);
		if (checkWin(matrix, n, goal))
		{
			pause = true;
			user.time = time;
			Plus(user.time, 1);
			clearScreen(115, 21, 13, 30);
			WinGameScene(108, 21, scene, goal, user, list);
			if (scene == menu)
				break;
			else
			{
				pause = false;
				clearScreen(108 - 4, 21, 13, 34);
				printMatrix(matrix, n, xo, yo);
				structionBoard(115, 21, user.name, 3, 15 - user.name.size());
				printInfoUser(115, 21, goal, list, user, time);
				continue;
			}
		}
		if (checkGOV(matrix, n))
		{
			int mode = 1;
			pause = true;
			user.time = time;
			Plus(user.time, 1);
			clearScreen(115, 21, 13, 30);
			GameOverScene(108, 21, scene, goal, undo_redo, user, list, mode);
			if (mode == 2)
			{
				pause = false;
				clearScreen(100, 21, 13, 41);
				processUndo(xo, yo, matrix, n, undo_redo, undo, redo, user, time, goal, list);
				structionBoard(115, 21, user.name, 3, 15 - user.name.size());
				continue;
			}
			else
				break;
		}
	}
	run = false;
	if (thr.joinable())
		thr.join();
	if (clo.joinable())
		clo.join();
	// Thêm người chơi vào top 20 (nếu đủ điều kiện)
	list.addUser(user);
	writeTopPlayerList(list);
	// Giải phóng bộ nhớ
	list.free();
	undo.free();
	redo.free();
	freeMatrix(matrix, n);
	freeMatrix(pre_matrix, n);
}
// Đếm ngược trước khi chơi 
void countDown(int x, int y)
{
	clearScreen(x - 10, y - 5, 20, 48);
	m.lock();
	SetColor(0, 8);
	gotoxy(x + 5, y);
	cout << char(219);
	gotoxy(x + 6, y + 1);
	cout << char(219) << char(220);
	gotoxy(x + 7, y + 2);
	cout << char(219);
	SetColor(0, 10);
	gotoxy(x + 10, y);
	cout << char(220) << char(219) << char(219) << char(219) << char(219) << char(220);
	gotoxy(x + 8, y + 1);
	cout << char(220) << char(219) << char(223) << char(223) << char(223) << char(223);
	gotoxy(x + 3, y + 3);
	SetColor(12, 12);
	cout << "          ";
	gotoxy(x + 1, y + 4);
	cout << "              ";
	gotoxy(x , y + 5);
	cout << "                ";
	gotoxy(x , y + 6);
	cout << "                ";
	gotoxy(x, y + 7);
	cout << "                ";
	gotoxy(x + 1, y + 8);
	cout << "              ";
	gotoxy(x + 3, y + 9);
	cout << "          ";
	SetColor(7, 7);
	gotoxy(x + 5, y + 4);
	cout << "      ";
	gotoxy(x + 9, y + 5);
	cout << "  ";
	gotoxy(x + 5, y + 6);
	cout << "      ";
	gotoxy(x + 9, y + 7);
	cout << "  ";
	gotoxy(x + 5, y + 8);
	cout << "      ";
	Sleep(1000);
	SetColor(12, 12);
	gotoxy(x + 9, y + 7);
	cout << "  ";
	gotoxy(x + 5, y + 7);
	SetColor(7, 7);
	cout << "  ";
	Sleep(1000);
	gotoxy(x + 5, y + 4);
	SetColor(12, 12);
	cout << "  ";
	SetColor(7, 7);
	cout << "  ";
	SetColor(12, 12);
	cout << "  ";
	gotoxy(x + 5, y + 7);
	cout << "  ";
	SetColor(7, 7);
	cout << "  ";
	gotoxy(x + 5, y + 6);
	SetColor(12, 12);
	cout << "  ";
	SetColor(7, 7);
	cout << "  ";
	SetColor(12, 12);
	cout << "  ";
	gotoxy(x + 7, y + 5);
	SetColor(7, 7);
	cout << "  ";
	SetColor(12, 12);
	cout << "  ";
	Sleep(1000);
	SetColor(0, 12);
	gotoxy(x + 5, y + 4);
	for (int i = 1; i <= 6; i++)
		cout << char(223);
	gotoxy(x + 4, y + 5);
	cout << "        " << char(223) << char(223) << char(223) << char(223);
	gotoxy(x + 4, y + 6);
	cout << "      " << char(220) << char(220) << char(220) << char(220) << char(220) << char(220);
	gotoxy(x + 4, y + 7);
	cout << "      " << char(223) << char(223);
	gotoxy(x + 5, y + 8);
	for (int i = 1; i <= 6; i++)
		cout << char(220);
	gotoxy(x + 19, y + 4);
	SetColor(0, 7);
	cout << char(220);
	SetColor(7, 0);
	cout << "      ";
	SetColor(0, 7);
	cout << char(220);
	SetColor(7, 0);
	gotoxy(x + 18, y + 5);
	cout << "  " << char(220);
	gotoxy(x + 25, y + 5);
	cout << char(220) << "  ";
	gotoxy(x + 18, y + 6);
	cout << "  ";
	gotoxy(x + 26, y + 6);
	cout << "  ";
	gotoxy(x + 18, y + 7);
	cout << "  " << char(223);
	gotoxy(x + 25, y + 7);
	cout << char(223) << "  ";
	gotoxy(x + 19, y + 8);
	SetColor(0, 7);
	cout << char(223);
	SetColor(7, 0);
	cout << "      ";
	SetColor(0, 7);
	cout << char(223);
	Sleep(1000);
	m.unlock();
	clearScreen(x - 10, y - 5, 20, 48);
}





