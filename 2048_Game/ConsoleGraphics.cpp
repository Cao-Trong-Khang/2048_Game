#include "ConsoleGraphics.h"
#include "Mutex.h"

extern mutex m;

int timetoint(Time time)
{
	return time.hour * 3600 + time.minute * 60 + time.second;
}

void SettingScreen()		// Một số định dạng màn hình console 
{
	SetConsoleOutputCP(437);
	ShowCur(0);
	SetScreenBufferSize(170, 70);
}

void SetScreenBufferSize(SHORT width, SHORT height)		// Hàm chỉnh kích thước màn hình console - Nguồn codelearn
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD NewSize;
	NewSize.X = width;
	NewSize.Y = height;
	SetConsoleScreenBufferSize(hStdout, NewSize);
}
// Hàm lấy tọa độ con trỏ hiện tại - Nguồn ChatGPT 
void getCursorPosition(int& x, int& y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) {
		x = consoleInfo.dwCursorPosition.X;
		y = consoleInfo.dwCursorPosition.Y;
	}
}  

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void printButton(int x, int y, string text, bool highlight, int baseColor, int highColor)   // In nút
{
	m.lock();
	printText(x + 1, y + 1, text, 0, baseColor + (int)highlight * (highColor - baseColor));
	int textcolorCode = baseColor * (int)highlight;
	SetColor(0, textcolorCode);
	gotoxy(x, y);
	cout << char(201) << char(205);
	gotoxy(x + text.size(), y);
	cout << char(205) << char(187);
	gotoxy(x, y + 2);
	cout << char(200) << char(205);
	gotoxy(x + text.size(), y + 2);
	cout << char(205) << char(188);
	SetColor(0, 15);
	m.unlock();
}

void printNotice(int x, int y, string Text) 	// Hàm in bản thông báo
{
	m.lock();
	SetColor(0, 8);
	gotoxy(x, y);
	for (int i = 0; i < 4 + Text.size(); i++)
		cout << char(220);
	gotoxy(x, y + 2);
	for (int i = 0; i < 4 + Text.size(); i++)
		cout << char(223);
	SetColor(7, 0);
	gotoxy(x + 1, y + 1);
	cout << " " << Text << " ";
	SetColor(0, 15);
	m.unlock();
}

void makeFrames()
{
	m.lock();
	SetColor(0, 11);
	gotoxy(0, 0);
	cout << char(201);
	for (int i = 1; i <= 151; i++)
		cout << char(205);
	cout << char(187);
	for (int i = 1; i <= 41; i++)
	{
		SetColor(0, 11);
		gotoxy(0, i);
		cout << char(186);
		SetColor(0, 14);
		gotoxy(152, i);
		cout << char(186);
	}
	SetColor(0, 14);
	gotoxy(0, 42);
	cout << char(200);
	for (int i = 1; i <= 151; i++)
		cout << char(205);
	cout << char(188);
	m.unlock();
}

void clearScreen(int x, int y, int h, int w)
{
	m.lock();
	SetColor(0, 0);
	for (int i = 0; i < h; i++)
	{
		gotoxy(x, y + i);
		for (int j = 0; j < w; j++)
			cout << " ";
	}
	gotoxy(x, y);
	m.unlock();
}

void printText(int x, int y, string text, int colorTextCode, int colorBackGroundCode)
{
	if(colorTextCode > 15 && colorBackGroundCode > 15)
	{
		gotoxy(x, y);
		cout << "\x1b[" << colorTextCode << "m" << "\x1b[" << colorBackGroundCode << "m";
		cout << text;
		cout << "\x1b[0m";
	}
	else if (colorTextCode <= 15 && colorBackGroundCode <= 15)
	{
		gotoxy(x, y);
		SetColor(colorBackGroundCode, colorTextCode);
		cout << text;
		SetColor(0, 15);
	}
}

int br_colorCode(int n)  // Hàm tìm mã màu cho một số 
{
	while (n > 1024)
		n = n / (1024);
	if (n == 0)
		return 15;
	else if (n == 2)
		return 14;
	else if (n == 4)
		return 11;
	else if (n == 8)
		return 3;
	else if (n == 16)
		return 9;
	else if (n == 32)
		return 12;
	else if (n == 64)
		return 13;
	else if (n == 128)
		return 5;
	else if (n == 256)
		return 6;
	else
		return 1;
}

void printFrameMatrix(int x, int y, int n)
{
	m.lock();
	SetColor(7, 0);
	for (int i = 0; i <= n * 4; i++)
	{
		if (i % 4 == 0)
		{
			gotoxy(x, y + i);
			cout << "  ";
			for (int j = 1; j <= n; j++)
				cout << "        ";
		}
		else
		{
			for (int j = 0; j <= n; j++)
			{
				gotoxy(x + j * 8, y + i);
				cout << "  ";
			}
		}
	}
	m.unlock();
}

int Count(int n)			// Hàm đếm chữ số 
{
	if (n == 0)
		return 1;
	int count = 0;
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return count;
}
	
void printGuide(int x, int y)
{
	m.lock();
	printText(x, y, "Use the arrow buttons   to move buttons.", 11, 0);
	printText(x, y + 1, "Press [     ] button to choose.", 11, 0);
	SetColor(0, 14);
	gotoxy(x + 22, y);
	cout << char(26);
	gotoxy(x + 7, y + 1);
	cout << "ENTER";
	m.unlock();
	Sleep(700);
	clearScreen(x, y, 2 , 41);    
}

int selectAccount(int xo, int yo, int mode, Scene& scene, bool lock)		// Chọn account để chơi tiếp hoặc để xóa và lưu tài khoản mới, trả về index tài khoản được chọn
{
	User user[6];
	string fileName[6] = { "Account_1.bin", "Account_2.bin", "Account_3.bin", "Account_4.bin", "Account_5.bin" , "Account_6.bin"};
	fstream file;
	char key;
	int check, counter = 0, empty = 0;
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
	clearScreen(xo - 1, yo - 1, 27, 90);
	int x_space = 33, y_space = 13;  // khoảng cách giữa các tài khoản 
	while (start)
	{
		if (!lock)
		{
			m.lock();
			gotoxy(xo + 3, yo - 4);
			if (mode == 0)
			{
				SetColor(12, 0);
				cout << " DELETE ";
				SetColor(0, 11);
				cout << char(220);
				gotoxy(xo + 4, yo - 3);
				for (int i = 1; i <= 8; i++)
					cout << char(223);
			}
			else
			{
				SetColor(11, 0);
				cout << "  PLAY  ";
				SetColor(0, 12);
				cout << char(220);
				gotoxy(xo + 4, yo - 3);
				for (int i = 1; i <= 8; i++)
					cout << char(223);
			}
			m.unlock();
		}
		for(int i = 0; i < 6; i++)
			printAccount(xo + (i % 3) * x_space, yo + (i / 3) * y_space, false, i, user[i], mode);
		printAccount(xo + (counter % 3) * x_space, yo + (counter / 3 ) * y_space, true, counter, user[counter], mode);
		key = _getch();
		if (key == Enter)
		{
			if (user[counter].name == "")
			{
				SetColor(0, 8);
				m.lock();
				gotoxy(xo + (counter % 3) * x_space - 2, yo + (counter / 3) * y_space + 10);
				cout << "Please select another account.";
				m.unlock();
				Sleep(500);
				clearScreen(xo + (counter % 3) * x_space - 2, yo + (counter / 3) * y_space + 10, 1, 31);
				continue;
			}
			else
			{
				if (mode == 0 && !lock)
				{
					bool agree = Question(48, 22, "Account will be permanently deleted.Continue ?");
					if (agree)
					{
						file.open(fileName[counter], ios::out | ios::binary | ios::trunc);
						file.write((char*)&empty, sizeof(empty));
						file.close();
						user[counter].name = "";
					}
					continue;
				}
				else
					return counter;
			}
		}
		if (key == prefix_value)
			key = _getch();
		if (key == 'c' && !lock)
		{
			mode = 1 - mode;
			continue;
		}
		if (key == 'b' && !lock)
		{
			scene = menu;
			return -1;
		}
		if (key == Right)
			counter++;
		if (key == Left)
			counter--;
		if (counter == -1)
			counter = 5;
		else
			counter %= 6;
	}
}

void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = CursorVisibility;

	SetConsoleCursorInfo(handle, &ConCurInf);
}

void SetColor(int backgound_color, int text_color)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color_code = backgound_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

void DisableSelection()		// Hàm vô hiệu hóa bôi đen màn hình console - Nguồn codelearn
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
}

bool Question(int x, int y, string text)			// Hàm hỏi người chơi để thực hiện chức năng gì đó true - đồng ý, false - không đồng ý  
{
	clearScreen(x - 4, y - 1, 8, text.size() + 11);
	m.lock();
	SetColor(0, 7);
	gotoxy(x - 3, y - 1);	
	// In khung hình
	cout << char(218);
	for (int i = 0; i < text.size() + 8; i++)
		cout << char(196);
	cout << char(191);
	for (int i = 0; i < 6; i++)
	{
		gotoxy(x - 3, y + i);
		cout << char(179);
		gotoxy(x + text.size() + 6, y + i);
		cout << char(179);
	}
	gotoxy(x - 3, y + 6);
	cout << char(192);
	for (int i = 0; i < text.size() + 8; i++)
		cout << char(196);
	cout << char(217);
	m.unlock();
	// Kết thúc in khung 
	printNotice(x, y, text);
	int counter = 0;
	char key;
	while (start)
	{
		printButton(x + text.size() / 2 - 7, y + 3, " Yes ", false, 11, 14);
		printButton(x + text.size() / 2 + 3, y + 3, " No  ", false, 11, 14);
		if (counter == 0)
			printButton(x + text.size() / 2 - 7, y + 3, " Yes ", true, 11, 14);
		else
			printButton(x + text.size() / 2 + 3, y + 3, " No  ", true, 11, 14);
		key = _getch();
		if (key == Enter)
		{
			clearScreen(x - 4, y - 1, 8, text.size() + 11);
			if (counter == 0)
				return true;
			else
				return false;
		}
		if (key == prefix_value)
			continue;
		if (key == Left)
			counter = 0;
		if (key == Right)
			counter = 1;
	}
}

void printAccount(int xo, int yo, bool highlight, int accIndex, User user, int mode)		// In Account ra màn hình để người dùng chọn 
{
	int br_color = 14 + (int)highlight * (-1 * mode - 2);		    // background color 
	int te_color = 0;								// text color 
	if (user.name == "")
		br_color = 8;
	m.lock();
	SetColor(br_color, te_color);
	gotoxy(xo, yo);
	cout << "                         ";
	gotoxy(xo, yo + 1);
	cout << "    ";
	SetColor(7 , te_color);
	if (user.name == "")
		SetColor(15, te_color);
	cout << "   ";
	SetColor(br_color, te_color);
	cout << "   ACCOUNT " << accIndex + 1 << "      ";
	gotoxy(xo, yo + 2);
	cout << "   ";
	SetColor(8 , te_color);
	if (user.name == "")
		SetColor(7, te_color);
	cout << "     ";
	SetColor(br_color, te_color);
	cout << "                 ";
	SetColor(0, br_color);
	gotoxy(xo, yo + 3);
	cout << char(186);
	gotoxy(xo, yo + 4);
	cout << char(186);
	gotoxy(xo, yo + 5);
	cout << char(186);
	gotoxy(xo, yo + 6);
	cout << char(186);
	gotoxy(xo, yo + 7);
	cout << char(186);
	gotoxy(xo + 24, yo + 3);
	cout << char(186);
	gotoxy(xo + 24, yo + 4);
	cout << char(186);
	gotoxy(xo + 24, yo + 5);
	cout << char(186);
	gotoxy(xo + 24, yo + 6);
	cout << char(186);
	gotoxy(xo + 24, yo + 7);
	cout << char(186);
	gotoxy(xo, yo + 8);
	cout << char(200);
	for (int i = 1; i <= 23; i++)
		cout << char(205);
	cout << char(188);

	SetColor(0, 15);
	if (user.name == "")
	{
		gotoxy(xo + 1, yo + 4);
		cout << "                       ";
		gotoxy(xo + 1, yo + 5);
		cout << "       NO DATA         ";
		gotoxy(xo + 1, yo + 6);
		cout << "                       ";
	}
	else
	{
		gotoxy(xo + 1, yo + 4);
		cout << " Name : ";
		cout << user.name;
		gotoxy(xo + 1, yo + 5);
		cout << " Score: ";
		cout << user.score;
		gotoxy(xo + 1, yo + 6);
		cout << " Time : ";
		if (user.time.hour < 10)
			cout << 0 << user.time.hour << "h";
		else
			cout << user.time.hour << "h";
		if (user.time.minute < 10)
			cout << 0 << user.time.minute << "m";
		else
			cout << user.time.minute << "m";
		if (user.time.second < 10)
			cout << 0 << user.time.second << "s";
		else
			cout << user.time.second << "s";
	}
	if (highlight)
		if (user.name == "")
			te_color = 8;
		else
			te_color = 14;
	else
		te_color = 0;
	SetColor(0, te_color);
	gotoxy(xo - 1, yo - 1);
	cout << char(201) << char(205);
	gotoxy(xo + 24, yo - 1);
	cout << char(205) << char(187);
	gotoxy(xo - 1, yo);
	cout << char(186);
	gotoxy(xo + 25, yo);
	cout << char(186);
	gotoxy(xo - 1, yo + 8);
	cout << char(186);
	gotoxy(xo + 25, yo + 8);
	cout << char(186);
	gotoxy(xo - 1, yo + 9);
	cout << char(200) << char(205);
	gotoxy(xo + 24, yo + 9);
	cout << char(205) << char(188);
	SetColor(0, 15);
	m.unlock();
}

void printMatrix(int** matrix, int n, int x, int y)
{
	m.lock();
	x += 3; y += 1;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == -1 || matrix[i][j] == -2)
			{
				if (matrix[i][j] == -1)
					SetColor(15, 8);
				else
					SetColor(11, 14);
				gotoxy(x + j * 8 - 1, y + i * 4);
				cout << char(222) << char(222) << char(223) << char(223) << char(222) << char(222);
				gotoxy(x + j * 8 - 1, y + i * 4 + 1);
				cout << char(223) << char(223) << char(222) << char(222) << char(223) << char(223);
				gotoxy(x + j * 8 - 1, y + i * 4 + 2);
				cout << char(222) << char(222) << char(223) << char(223) << char(222) << char(222);
				continue;
			}
			if(br_colorCode(matrix[i][j]) != 1)
				SetColor(br_colorCode(matrix[i][j]), 0);
			else
				SetColor(br_colorCode(matrix[i][j]), 15);
			gotoxy(x + j * 8 - 1, y + i * 4);
			cout << "      ";
			gotoxy(x + j * 8 - 1, y + i * 4 + 1);
			if (matrix[i][j] == 0)
				cout << "      ";
			else if (matrix[i][j] < 10)
				cout << "  " << matrix[i][j] << "   ";
			else if (matrix[i][j] < 100)
				cout << "  " << matrix[i][j] << "  ";
			else if (matrix[i][j] < 1000)
				cout << " " << matrix[i][j] << "  ";
			else if (matrix[i][j] < 10000)
				cout << " " << matrix[i][j] << " ";
			else if (matrix[i][j] < 100000)
				cout << matrix[i][j] << " ";
			else
				cout << matrix[i][j];				
			gotoxy(x + j * 8 - 1, y + i * 4 + 2);
			cout << "      ";
		}
	}
	m.unlock();
}

// Hàm in kí tự kiểu To có màu 
void printLetter(int x, int y, char c, int textColor, int brColor)    
{
	string Letter[15][6];
	string cell1 = "  ";
	string cell2 = cell1 + cell1;
	string cell3 = cell2 + cell1;
	string cell4 = cell3 + cell1;

	if (c == 'R')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell3;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell3;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 5, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell1;
		gotoxy(x + 5, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 6, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 1);
		cout << char(220);
		gotoxy(x + 6, y + 2);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 3);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 3);
		cout << char(220);
		gotoxy(x + 2, y + 4);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 6, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'E')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell3 << " ";
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell2 << " ";
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell3 << " ";
		gotoxy(x, y + 5);
		SetColor(0, brColor);
		gotoxy(x + 7, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 5, y + 2);
		cout << char(220);
		gotoxy(x + 2, y + 3);
		cout << char(219) << char(223) << char(223) << char(223);
		gotoxy(x + 7, y + 4);
		cout << char(220);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'S')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell3 << " ";
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell3 << " ";
		gotoxy(x + 5, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell3 << " ";
		SetColor(0, brColor);
		gotoxy(x + 7, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 7, y + 2);
		cout << char(220);
		gotoxy(x + 1, y + 3);
		cout << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 7, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'U')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1;
		gotoxy(x + 6, y);
		cout << cell1;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 6, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 6, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 6, y + 3);
		cout << cell1;
		gotoxy(x + 1, y + 4);
		cout << cell3;
		SetColor(0, brColor);
		gotoxy(x + 2, y);
		cout << char(220);
		gotoxy(x + 8, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219);
		gotoxy(x + 8, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 8, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 8, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 10, y);
	}
	else if (c == 'M')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1 << " ";
		gotoxy(x + 7, y);
		cout << cell1 << " ";
		gotoxy(x, y + 1);
		cout << cell2;
		gotoxy(x + 6, y + 1);
		cout << cell2;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 3, y + 2);
		cout << cell2;
		gotoxy(x + 8, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 4, y + 3);
		cout << cell1;
		gotoxy(x + 8, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell1;
		gotoxy(x + 8, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 3, y);
		cout << char(220);
		gotoxy(x + 10, y);
		cout << char(220);
		gotoxy(x + 4, y + 1);
		cout << char(220);
		gotoxy(x + 10, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 7, y + 2);
		cout << char(219);
		gotoxy(x + 10, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 6, y + 3);
		cout << char(219) << char(223);
		gotoxy(x + 10, y + 3);
		cout << char(219);
		gotoxy(x + 2, y + 4);
		cout << char(219);
		gotoxy(x + 5, y + 4);
		cout << char(223) << char(223);
		gotoxy(x + 10, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 9, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 12, y);
	}
	else if (c == 'T')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell4;
		gotoxy(x + 3, y + 1);
		cout << cell1;
		gotoxy(x + 3, y + 2);
		cout << cell1;
		gotoxy(x + 3, y + 3);
		cout << cell1;
		gotoxy(x + 3, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 8, y);
		cout << char(220);
		gotoxy(x + 1, y + 1);
		cout << char(223) << char(223);
		gotoxy(x + 5, y + 1);
		cout << char(219) << char(223) << char(223) << char(223);
		gotoxy(x + 5, y + 2);
		cout << char(219);
		gotoxy(x + 5, y + 3);
		cout << char(219);
		gotoxy(x + 5, y + 4);
		cout << char(219);
		gotoxy(x + 4, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 10, y);
	}
	else if (c == 'I')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 2, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 2, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 4, y);
	}
	else if (c == 'N')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1 << " ";
		gotoxy(x + 7, y);
		cout << cell1;
		gotoxy(x, y + 1);
		cout << cell2;
		gotoxy(x + 7, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 3, y + 2);
		cout << cell1;
		gotoxy(x + 7, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 4, y + 3);
		cout << cell1;
		gotoxy(x + 7, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell1;
		gotoxy(x + 5, y + 4);
		cout << cell2;
		SetColor(0, brColor);
		gotoxy(x + 3, y);
		cout << char(220);
		gotoxy(x + 9, y);
		cout << char(220);
		gotoxy(x + 4, y + 1);
		cout << char(220);
		gotoxy(x + 9, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 5, y + 2);
		cout << char(220);
		gotoxy(x + 9, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 6, y + 3);
		cout << char(220);
		gotoxy(x + 9, y + 3);
		cout << char(219);
		gotoxy(x + 2, y + 4);
		cout << char(219);
		gotoxy(x + 9, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 6, y + 5);
		cout << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 11, y);
	}
	else if (c == 'G')
	{
		SetColor(textColor, 0);
		gotoxy(x + 1, y);
		cout << cell3;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 5, y + 2);
		cout << cell1 << " ";
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 6, y + 3);
		cout << cell1;
		gotoxy(x + 1, y + 4);
		cout << cell4;
		SetColor(0, brColor);
		gotoxy(x + 7, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 8, y + 2);
		cout << char(220);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 8, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 10, y);
	}
	else if (c == 'O')
	{
		SetColor(textColor, 0);
		gotoxy(x + 1, y);
		cout << cell3;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 6, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 6, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 6, y + 3);
		cout << cell1;
		gotoxy(x + 1, y + 4);
		cout << cell3;
		SetColor(0, brColor);
		gotoxy(x + 7, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223) << char(223);
		gotoxy(x + 8, y + 1);
		cout << char(220);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 8, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 8, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 10, y);
	}
	else if (c == 'P')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell3;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell3;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 6, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 1);
		cout << char(220);
		gotoxy(x + 6, y + 2);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 3);
		cout << char(219) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 2, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'L')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell3 << " ";
		SetColor(0, brColor);
		gotoxy(x + 2, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(220);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'A')
	{
		SetColor(textColor, 0);
		gotoxy(x + 1, y);
		cout << cell2 << " ";
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell3 << " ";
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 5, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell1;
		gotoxy(x + 5, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 6, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 1);
		cout << char(220);
		gotoxy(x + 7, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 3);
		cout << char(219);
		gotoxy(x + 2, y + 4);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 6, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'Y')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1;
		gotoxy(x + 6, y);
		cout << cell1;
		gotoxy(x + 1, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x + 2, y + 2);
		cout << cell2;
		gotoxy(x + 3, y + 3);
		cout << cell1;
		gotoxy(x + 3, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 2, y);
		cout << char(220);
		gotoxy(x + 8, y);
		cout << char(220);
		gotoxy(x + 3, y + 1);
		cout << char(220);
		gotoxy(x + 7, y + 1);
		cout << char(219) << char(223);
		gotoxy(x + 6, y + 2);
		cout << char(219) << char(223);
		gotoxy(x + 5, y + 3);
		cout << char(219) << char(223);
		gotoxy(x + 5, y + 4);
		cout << char(219);
		gotoxy(x + 4, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 10, y);
	}
	else if (c == 'D')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell3;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 5, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 5, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell3;
		SetColor(0, brColor);
		gotoxy(x + 6, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 7, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 3);
		cout << char(219);
		gotoxy(x + 6, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 9, y);
	}
	else if (c == 'W')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1;
		gotoxy(x + 7, y);
		cout << cell1;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 7, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 4, y + 2);
		cout << " ";
		gotoxy(x + 7, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 3, y + 3);
		cout << cell1 << " ";
		gotoxy(x + 7, y + 3);
		cout << cell1;
		gotoxy(x + 1, y + 4);
		cout << cell1 << " ";
		gotoxy(x + 5, y + 4);
		cout << cell1 << " ";
		SetColor(0, brColor);
		gotoxy(x + 2, y);
		cout << char(220);
		gotoxy(x + 9, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219);
		gotoxy(x + 9, y + 1);
		cout << char(219);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 9, y + 2);
		cout << char(219);
		gotoxy(x + 2, y + 3);
		cout << char(219);
		gotoxy(x + 9, y + 3);
		cout << char(219);
		gotoxy(x + 5, y + 2);
		cout << char(220);
		gotoxy(x + 6, y + 3);
		cout << char(220);
		gotoxy(x + 4, y + 4);
		cout << char(219);
		gotoxy(x + 8, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 5);
		cout << char(223) << char(223) << char(223);
		gotoxy(x + 6, y + 5);
		cout << char(223) << char(223) << char(223);
		gotoxy(x + 11, y);
	}
	else if (c == '2')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell3;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x + 1, y + 2);
		cout << cell2 << " ";
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x, y + 4);
		cout << cell3 << " ";
		SetColor(0, brColor);
		gotoxy(x + 6, y);
		cout << char(220);
		gotoxy(x + 1, y + 1);
		cout << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 7, y + 1);
		cout << char(220);
		gotoxy(x + 6, y + 2);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 3);
		cout << char(219) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 7, y + 4);
		cout << char(220);
		gotoxy(x + 1, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 8, y);
	}
	else if (c == '0')
	{
		SetColor(textColor, 0);
		gotoxy(x + 1, y);
		cout << cell3;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 4, y + 1);
		cout << cell2;
		gotoxy(x, y + 2);
		cout << cell1;
		gotoxy(x + 3, y + 2);
		cout << cell1;
		gotoxy(x + 6, y + 2);
		cout << cell1;
		gotoxy(x, y + 3);
		cout << cell2;
		gotoxy(x + 6, y + 3);
		cout << cell1;
		gotoxy(x + 1, y + 4);
		cout << cell3;
		SetColor(0, brColor);
		gotoxy(x + 7, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223);
		gotoxy(x + 8, y + 1);
		cout << char(220);
		gotoxy(x + 2, y + 2);
		cout << char(219);
		gotoxy(x + 5, y + 2);
		cout << char(219);
		gotoxy(x + 8, y + 2);
		cout << char(219);
		gotoxy(x + 4, y + 3);
		cout << char(219) << char(223);
		gotoxy(x + 8, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223)<< char(223);
		gotoxy(x + 9, y);
	}
	else if (c == '4')
	{
		SetColor(textColor, 0);
		gotoxy(x, y);
		cout << cell1;
		gotoxy(x + 5, y);
		cout << cell1;
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x, y + 2);
		cout << cell3 << " ";
		gotoxy(x + 5, y + 3);
		cout << cell1;
		gotoxy(x + 5, y + 4);
		cout << cell1;
		SetColor(0, brColor);
		gotoxy(x + 2, y);
		cout << char(220);
		gotoxy(x + 7, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219);
		gotoxy(x + 7, y + 1);
		cout << char(219);
		gotoxy(x + 7, y + 2);
		cout << char(219);
		gotoxy(x + 1, y + 3);
		cout << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 7, y + 3);
		cout << char(219);
		gotoxy(x + 7, y + 4);
		cout << char(219);
		gotoxy(x + 6, y + 5);
		cout << char(223) << char(223);
		gotoxy(x + 8, y);
	}
	else if (c == '8')
	{
		SetColor(textColor, 0);
		gotoxy(x + 1, y);
		cout << cell2 << " ";
		gotoxy(x, y + 1);
		cout << cell1;
		gotoxy(x + 5, y + 1);
		cout << cell1;
		gotoxy(x + 1, y + 2);
		cout << cell2 << " ";
		gotoxy(x, y + 3);
		cout << cell1;
		gotoxy(x + 5, y + 3);
		cout << cell1;
		gotoxy(x + 1, y + 4);
		cout << cell2 << " ";
		SetColor(0, brColor);
		gotoxy(x + 6, y);
		cout << char(220);
		gotoxy(x + 2, y + 1);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 1);
		cout << char(220);
		gotoxy(x + 6, y + 2);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 3);
		cout << char(219) << char(223) << char(223);
		gotoxy(x + 7, y + 3);
		cout << char(220);
		gotoxy(x + 6, y + 4);
		cout << char(219) << char(223);
		gotoxy(x + 2, y + 5);
		cout << char(223) << char(223) << char(223) << char(223) << char(223);
		gotoxy(x + 8, y);
	}
	else if (c == ' ')
	{
		gotoxy(x + 4, y);
	}
	SetColor(0, 15);
}

// In tiêu đề chữ kiểu
void printTitle(int x, int y, string title, bool& run, int color1, int color2)
{
	int mmsecond = 500;
	int count = 0;
	m.lock();
	int old_x, old_y, next_x = x, next_y = y, index = 0;
	gotoxy(x, y);
	for (int i = 0; i < title.size(); i++)
	{
		printLetter(next_x, next_y, title[i], 11, 12);
		getCursorPosition(next_x, next_y);
	}
	m.unlock();
	int te_Color = color1, br_Color = color2;
	while (run)
	{
		next_x = x, next_y = y;
		m.lock();
		getCursorPosition(old_x, old_y);
		for (int i = 0; i < title.size(); i++)
		{
			if(i <= index)
				printLetter(next_x, next_y, title[i], te_Color, br_Color);
			else
				printLetter(next_x, next_y, title[i], br_Color, te_Color);
			getCursorPosition(next_x, next_y);
		}
		gotoxy(old_x, old_y);
		m.unlock();
		index++;
		if (index >= title.size())
		{
			mmsecond = 150;
			count++;
			swap(te_Color, br_Color);
			if (count == 9)
			{
				mmsecond = 500;
				index = 0;
				count = 0;
			}
		}
		Sleep(mmsecond);
	}
}

// Hàm in số 
void printNumber(int x, int y, int n)
{
	if (n == 0)
	{
		gotoxy(x, y);
		cout << char(201) << char(205) << char(187);
		gotoxy(x, y + 1);
		cout << char(186) << " " << char(186);
		gotoxy(x, y + 2);
		cout << char(200) << char(205) << char(188);
	}
	else if (n == 1)
	{
		gotoxy(x, y);
		cout << " " << char(183) << " ";
		gotoxy(x, y + 1);
		cout << " " << char(186) << " ";
		gotoxy(x, y + 2);
		cout << " " << char(202) << " ";
	}
	else if (n == 2)
	{
		gotoxy(x, y);
		cout << char(213) << char(205) << char(187);
		gotoxy(x, y + 1);
		cout << char(201) << char(205) << char(188);
		gotoxy(x, y + 2);
		cout << char(200) << char(205) << char(190);
	}
	else if (n == 3)
	{
		gotoxy(x, y);
		cout << char(213) << char(205) << char(187);
		gotoxy(x, y + 1);
		cout << " " << char(205) << char(185);
		gotoxy(x, y + 2);
		cout << char(212) << char(205) << char(188);
	}
	else if (n == 4)
	{
		gotoxy(x, y);
		cout << char(214) << " " << char(183);
		gotoxy(x, y + 1);
		cout << char(200) << char(205) << char(185);
		gotoxy(x, y + 2);
		cout << "  " << char(189);
	}
	else if (n == 5)
	{
		gotoxy(x, y);
		cout << char(201) << char(205) << char(184);
		gotoxy(x, y + 1);
		cout << char(200) << char(205) << char(187);
		gotoxy(x, y + 2);
		cout << char(212) << char(205) << char(188);
	}
	else if (n == 6)
	{
		gotoxy(x, y);
		cout << char(201) << char(205) << char(184);
		gotoxy(x, y + 1);
		cout << char(204) << char(205) << char(187);
		gotoxy(x, y + 2);
		cout << char(200) << char(205) << char(188);
	}
	else if (n == 7)
	{
		gotoxy(x, y);
		cout << char(213) << char(205) << char(187);
		gotoxy(x, y + 1);
		cout << "  " << char(186);
		gotoxy(x, y + 2);
		cout << "  " << char(189);
	}
	else if (n == 8)
	{
		gotoxy(x, y);
		cout << char(201) << char(205) << char(187);
		gotoxy(x, y + 1);
		cout << char(204) << char(205) << char(185);
		gotoxy(x, y + 2);
		cout << char(200) << char(205) << char(188);
	}
	else if (n == 9)
	{
		gotoxy(x, y);
		cout << char(201) << char(205) << char(187);
		gotoxy(x, y + 1);
		cout << char(200) << char(205) << char(185);
		gotoxy(x, y + 2);
		cout << char(212 ) << char(205) << char(188);
	}
	gotoxy(x + 3, y);
	
}

// In khung đồng hồ 
void clockFrame(int x, int y)
{
	SetColor(0, 8);
	gotoxy(x - 2, y - 1);
	for (int i = 0; i < 24; i++)
		cout << char(220);
	gotoxy(x - 2, y);
	cout << char(219);
	gotoxy(x + 21, y);
	cout << char(219);
	gotoxy(x - 2, y + 1);
	cout << char(219);
	gotoxy(x + 21, y + 1);
	cout << char(219);
	gotoxy(x - 2, y + 2);
	cout << char(219);
	gotoxy(x + 21, y + 2);
	cout << char(219);
	gotoxy(x - 2, y + 3);
	for (int i = 0; i < 24; i++)
		cout << char(223);
	SetColor(7, 0);
	gotoxy(x - 1, y);
	cout << " ";
	gotoxy(x + 20, y);
	cout << " ";
	gotoxy(x - 1, y + 1);
	cout << " ";
	gotoxy(x + 20, y + 1);
	cout << " ";
	gotoxy(x - 1, y + 2);
	cout << " ";
	gotoxy(x + 20, y + 2);
	cout << " ";
}

// Hàm tính thời gian người chơi
void Clock(int x, int y, Time& time, bool& run, bool& pause)
{
	int x_next, y_next;
	m.lock();
	clockFrame(x, y);
	printNumber(x, y, time.hour / 10);
	getCursorPosition(x_next, y_next);
	printNumber(x_next, y_next, time.hour % 10);
	getCursorPosition(x_next, y_next);
	gotoxy(x_next, y_next);
	cout << " ";
	gotoxy(x_next, y_next + 1);
	cout << ":";
	gotoxy(x_next, y_next + 2);
	cout << " ";
	getCursorPosition(x_next, y_next);
	printNumber(x_next, y_next - 2, time.minute / 10);
	getCursorPosition(x_next, y_next);
	printNumber(x_next, y_next, time.minute % 10);
	getCursorPosition(x_next, y_next);
	gotoxy(x_next, y_next);
	cout << " ";
	gotoxy(x_next, y_next + 1);
	cout << ":";
	gotoxy(x_next, y_next + 2);
	cout << " ";
	getCursorPosition(x_next, y_next);
	printNumber(x_next, y_next - 2, time.second / 10);
	getCursorPosition(x_next, y_next);
	printNumber(x_next, y_next, time.second % 10);
	m.unlock();
	while(run)
	{
		while (run && !pause)
		{
			Sleep(1000);
			time.second++;
			if (time.second == 60)
			{
				time.second = 0;
				time.minute++;
			}
			if (time.minute == 60)
			{
				time.minute = 0;
				time.hour++;
			}
			m.lock();
			SetColor(7, 0);
			printNumber(x, y, time.hour / 10);
			getCursorPosition(x_next, y_next);
			printNumber(x_next, y_next, time.hour % 10);
			getCursorPosition(x_next, y_next);
			gotoxy(x_next, y_next);
			cout << " ";
			gotoxy(x_next, y_next + 1);
			cout << ":";
			gotoxy(x_next, y_next + 2);
			cout << " ";
			getCursorPosition(x_next, y_next);
			printNumber(x_next, y_next - 2, time.minute / 10);
			getCursorPosition(x_next, y_next);
			printNumber(x_next, y_next, time.minute % 10);
			getCursorPosition(x_next, y_next);
			gotoxy(x_next, y_next);
			cout << " ";
			gotoxy(x_next, y_next + 1);
			cout << ":";
			gotoxy(x_next, y_next + 2);
			cout << " ";
			getCursorPosition(x_next, y_next);
			printNumber(x_next, y_next - 2, time.second / 10);
			getCursorPosition(x_next, y_next);
			printNumber(x_next, y_next, time.second % 10);
			m.unlock();
		}
	}
}

// Hàm in bảng hướng dẫn  
void structionBoard(int x, int y, string title, int h, int w)
{
	int color1 = 14, color2 = 11;
	m.lock();      
	gotoxy(x, y);
	SetColor(0, color1);
	cout << char(201);
	for (int i = 1; i <= 8; i++)
		cout << char(205);
	SetColor(0, color2);
	for (int i = 1; i <= w; i++)
		cout << char(196);
	cout << "[ ";
	SetColor(0, color1);
	cout << title;
	SetColor(0, color2);
	cout << " ]" << char(196) << char(191);
	for (int i = 1; i < 5; i++)
	{
		SetColor(0, color1);
		gotoxy(x, y + i);
		cout << char(186);
		SetColor(0, color2);
		gotoxy(x + 14 + w + title.size(), y + i);
		cout << char(179);
	}
	SetColor(0, color2);
	for (int i = 5; i < h + 5; i++)
	{
		gotoxy(x, y + i);
		cout << char(179);
		gotoxy(x + 14 + w + title.size(), y + i);
		cout << char(179);
	}
	for (int i = h + 5; i <= h + 8; i++)
	{
		SetColor(0, color2);
		gotoxy(x, y + i);
		cout << char(179);
		SetColor(0, color1);
		gotoxy(x + 14 + w + title.size(), y + i);
		cout << char(186);
	}
	SetColor(0, color2);
	gotoxy(x, y + h + 9);
	cout << char(192) << char(196);
	for (int i = 1; i <= w + title.size() + 4; i++)
		cout << char(196);
	SetColor(0, color1);
	for (int i = 1; i <= 8; i++)
		cout << char(205);
	cout << char(188);
	m.unlock();
}

void warning()
{
	m.lock();
	SetColor(0, 11);
	gotoxy(114, 40);
	cout << "[   ] Playing for more than 180 ";
	gotoxy(112, 41);
	cout << "minutes a day may impact your health";
	SetColor(0, 14);
	gotoxy(115, 40);
	cout << "18+";
	m.unlock();
}

// In các số trong tiêu đề 2048 - ITUS
void print2(int x, int y, int index, int c1, int c2)	// c1 - color 1, c2 - color 2
{
	m.lock();
	if (index == 0)
	{
		SetColor(c1, c1);
		gotoxy(x, y);
		cout << "                ";
		gotoxy(x, y + 1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 1, y + 2);
		cout << "           ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 11, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 11, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 11, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x, y + 6);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x, y + 7);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x, y + 8);
		cout << "    ";
		SetColor(c2, c2);
		cout << "             ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		gotoxy(x, y + 13);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 1, y + 14);
		cout << "                ";
	}
	else if (index == 1)
	{
		gotoxy(x + 10, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 10, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 10, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 10, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 14, y + 6);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";

		gotoxy(x + 14, y + 7);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 15, y + 8);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 2)
	{
		gotoxy(x + 9, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 9, y + 3);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 9, y + 4);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 9, y + 5);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 13, y + 6);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 1, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 13, y + 7);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 1, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";

		gotoxy(x + 1, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 14, y + 8);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 3)
	{
		gotoxy(x + 8, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 8, y + 3);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 8, y + 4);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 8, y + 5);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 12, y + 6);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 2, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 12, y + 7);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 2, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";

		gotoxy(x + 2, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 13, y + 8);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 4)
	{
		gotoxy(x + 7, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 7, y + 3);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 7, y + 4);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 7, y + 5);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 11, y + 6);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 3, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 11, y + 7);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 3, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";

		gotoxy(x + 3, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 8);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 5)
	{
		gotoxy(x + 6, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 6, y + 3);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 6, y + 4);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(0, 0);
		gotoxy(x + 6, y + 5);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 10, y + 6);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 4, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		SetColor(c1, c1);
		gotoxy(x + 10, y + 7);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 4, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";

		gotoxy(x + 4, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 11, y + 8);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 6)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 2);
		cout << "     ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << "       ";

		gotoxy(x + 6, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 6, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 6, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "                ";
	}
	m.unlock();
}   // In 

void print0(int x, int y, int index, int c1, int c2)
{
	m.lock();
	if (index == 0)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << "        ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 11, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		cout << "                ";
	}
	else if (index == 1)
	{
		gotoxy(x, y + 2);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 10, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 10, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 14, y + 12);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 14, y + 13);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 15, y + 14);
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 2)
	{
		gotoxy(x + 1, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 9, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 9, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 12);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 13, y + 12);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 1, y + 13);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 13, y + 13);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 14);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 14, y + 14);
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 3)
	{
		gotoxy(x + 2, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 8, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 2, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 8, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 12);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 12, y + 12);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 2, y + 13);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 12, y + 13);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 14);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 13, y + 14);
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 4)
	{
		gotoxy(x + 3, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 3, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 8, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 12);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 11, y + 12);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 3, y + 13);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 11, y + 13);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 14);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 14);
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 5)
	{
		gotoxy(x + 4, y + 2);
		SetColor(c2, c2);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 4, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 7, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 12);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 10, y + 12);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 4, y + 13);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << " ";
		gotoxy(x + 10, y + 13);
		SetColor(c1, c1);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 14);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 11, y + 14);
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";
	}
	else if (index == 6)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 2);
		cout << "     ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << "       ";

		gotoxy(x + 5, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 6);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 7);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 9);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 10);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 11);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 12);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 5, y + 13);
		SetColor(0, 0);
		cout << " ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 6, y + 14);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";
	}
	m.unlock();
}

void print4(int x, int y, int index, int c1, int c2)
{
	m.lock();
	if (index == 0)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "    ";

		gotoxy(x, y + 1);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 5);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "           ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 15);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 16);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 17);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 13, y + 18);
		SetColor(c2, c2);
		cout << "    ";
	}
	else if (index == 1)
	{
		gotoxy(x + 12, y + 4);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x + 12, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 12, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 16);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 17);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x + 12, y + 18);
		SetColor(0, 0);
		cout << "     ";
	}
	else if (index == 2)
	{
		gotoxy(x + 12, y + 3);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x + 12, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 12, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 15);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 16);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x + 12, y + 17);
		SetColor(0, 0);
		cout << "     ";
	}
	else if (index == 3)
	{
		gotoxy(x + 12, y + 2);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x + 12, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 12, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 14);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 15);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x + 12, y + 16);
		SetColor(0, 0);
		cout << "     ";
	}
	else if (index == 4)
	{
		gotoxy(x + 12, y + 1);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x + 12, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 12, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "                ";

		gotoxy(x + 12, y + 15);
		SetColor(0, 0);
		cout << "     ";
	}
	else if (index == 5)
	{
		gotoxy(x + 12, y);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x + 12, y + 1);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x + 12, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "                ";

		gotoxy(x + 12, y + 15);
		SetColor(0, 0);
		cout << "     ";
	}
	else if (index == 6)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "    ";
		gotoxy(x + 12, y);
		cout << "    ";

		gotoxy(x, y + 1);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 1);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 7);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "                ";

		gotoxy(x + 12, y + 15);
		SetColor(0, 0);
		cout << "     ";
	}
	m.unlock();
}

void print8(int x, int y, int index, int c1, int c2)
{
	m.lock();
	if (index == 0)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 1);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << "        ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 6);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 7);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << "        ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "                ";
	}
	else if (index == 1)
	{
		gotoxy(x + 12, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 5);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x + 12, y + 6);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

	}
	else if (index == 2)
	{
		gotoxy(x + 12, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 4);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x + 12, y + 5);
		SetColor(0, 0);
		cout << "     ";
	}
	else if (index == 3)
	{
		gotoxy(x + 12, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 12, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x + 12, y + 4);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x, y + 8);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
	}
	else if (index == 4)
	{
		gotoxy(x, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "     ";

		gotoxy(x, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
	}
	else if (index == 5)
	{
		gotoxy(x, y + 9);
		SetColor(0, 0);
		cout << "     ";

		gotoxy(x, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
	}
	else if (index == 6)
	{
		gotoxy(x, y);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 1);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 2);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << "        ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 3);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";
		gotoxy(x + 12, y + 3);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "    ";

		gotoxy(x, y + 4);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 5);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 6);
		SetColor(c1, c1);
		cout << "                ";

		gotoxy(x, y + 7);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 8);
		SetColor(0, 0);
		cout << " ";
		SetColor(c2, c2);
		cout << "           ";
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 9);
		SetColor(0, 0);
		cout << "     ";
		gotoxy(x + 12, y + 9);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 10);
		SetColor(0, 0);
		cout << "     ";
		gotoxy(x + 12, y + 10);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(0, 0);
		cout << " ";
		gotoxy(x + 12, y + 11);
		SetColor(c1, c1);
		cout << "    ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 12);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x, y + 13);
		SetColor(c1, c1);
		cout << "                ";
		SetColor(c2, c2);
		cout << " ";

		gotoxy(x + 1, y + 14);
		SetColor(c2, c2);
		cout << "                ";
	}
	m.unlock();
}

void makeTitle2048(int x, int y, bool& run)
{
	int index = 0, num = 0, move = 0, color1 = 11, color2 = 14, count = 0, time = 1000, control = 1;
	print2(x, y, index, color1, color2);
	print0(x + 22, y, index, color1, color2);
	print4(x + 44, y, index, color1, color2);
	print8(x + 66, y, index, color1, color2);
	while (run)
	{
		if (move)
		{
			index = index + control;
			print2(x, y, index, color1, color2);
			print0(x + 22, y, index, color1, color2);
			print4(x + 44, y, index, color1, color2);
			print8(x + 66, y, index, color1, color2);
			if (index == 0 || index == 6)
			{
				control *= -1;
				move = 0;
				Sleep(500);
				continue;
			}
			Sleep(100);
			continue;
		}
		if(num == 0)
		{
			print2(x, y, index, color2, color1);
			num++;
		}
		else if(num == 1)
		{
			print0(x + 22, y, index, color2, color1);
			num++;
		}
		else if(num == 2)
		{
			print4(x + 44, y, index, color2, color1);
			num++;
		}
		else if(num == 3)
		{
			print8(x + 66, y, index, color2, color1);
			num++;
			Sleep(30);
			continue;
		}
		else if (num == 4)
		{
			if (count <= 5)
			{
				swap(color1, color2);
				print2(x, y, index, color1, color2);
				print0(x + 22, y, index, color1, color2);
				print4(x + 44, y, index, color1, color2);
				print8(x + 66, y, index, color1, color2);
				count++;
			}
			else
			{
				count = 0;
				num = 0;
				move = 1;
			}
			Sleep(300);
			continue;
		}
		Sleep(time);
	}
}

