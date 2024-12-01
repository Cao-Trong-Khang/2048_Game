#include "MiniGame.h"

// Phần Mini game 

void printHowToPlayPVP(int x, int y)
{
	m.lock();
	printText(x + 2, y + 3, "     Two players will compete against each", 14, 0);
	printText(x + 2, y + 4, "other in PvP (Player versus Player) mode. ", 14, 0);
	printText(x + 2, y + 5, "There are two gameplay modes: countdown & ", 14, 0);
	printText(x + 2, y + 6, "target achievement. The player with higher", 14, 0);
	printText(x + 2, y + 7, "score when time runs out or the player who", 14, 0);
	printText(x + 2, y + 8, "reaches the target first will win.Player 1", 14, 0);
	printText(x + 2, y + 9, "uses the arrow keys, and Player 2 uses the", 14, 0);
	printText(x + 2, y + 10, "A,S,W,D keys to move.                     ", 14, 0);
	m.unlock();
}

void printHowToPlayBlocks(int x, int y)
{
	m.lock();
	printText(x + 2, y + 3, "      Embark on the challenging journey of", 14, 0);
	printText(x + 2, y + 4, "the 8x8 grid version of 2048,where hurdles", 14, 0);
	printText(x + 2, y + 5, "test your puzzle-solving skills . Maneuver", 14, 0);
	printText(x + 2, y + 6, "through  the maze of walls  strategically,", 14, 0);
	printText(x + 2, y + 7, "merging  same tiles  to  reach the coveted", 14, 0);
	printText(x + 2, y + 8, "2048 tile.Swipe left, right, up,or down to", 14, 0);
	printText(x + 2, y + 9, "shift tiles,but remember,walls are blocked", 14, 0);
	printText(x + 2, y + 10, "barriers.                                 ", 14, 0);
	m.unlock();
}

void miniGameScene(Scene& scene)
{
	m.lock();
	SetColor(0, 0);
	system("cls");
	m.unlock();
	makeFrames();
	warning();

	string title = "MINI GAME";
	bool run = true;
	int counter = 0, mode, x = 20, y = 15;
	char key;
	thread thr(printTitle, 40, 2, title, ref(run), 11, 14);
	structionBoard(x + 34, y, "HOW TO PLAY", 5, 22);

	while (start)
	{
		printButton(x, y, "    PVP MODE   ", false, 11, 14);
		printButton(x, y + 2, "  BLOCKS MODE  ", false, 11, 14);
		printButton(x, y + 4, "   BACK MENU   ", false, 11, 14);
		if (counter == 0)
		{
			printButton(x, y, "    PVP MODE   ", true, 11, 14);
			structionBoard(x + 34, y, "HOW TO PLAY", 5, 22);
			printHowToPlayPVP(x + 35, y);
		}
		else if (counter == 1)
		{
			printButton(x, y + 2, "  BLOCKS MODE  ", true, 11, 14);
			structionBoard(x + 34, y, "HOW TO PLAY", 5, 22);
			printHowToPlayBlocks(x + 35, y);
		}
		else if (counter == 2)
		{
			printButton(x, y + 4, "   BACK MENU   ", true, 11, 14);
			clearScreen(x + 33, y, 15, 54);
		}
		key = _getch();
		if (key == Enter)
		{
			run = false;
			if (counter == 3)
				scene = menu;
			break;
		}
		if (key == prefix_value)
			key = _getch();
		if (key == Down)
			counter++;
		else if (key == Up)
			counter--;
		if (counter == -1)
			counter = 2;
		counter %= 3;
	}
	if (thr.joinable())
		thr.join();
	if (counter == 0)
		pvpScene(scene);
	if (counter == 1)
		blocksScene(scene);
	if (counter == 2)
		scene = menu;
}

// Phần PvP 

void printGoal(int x, int y, int goal)
{
	m.lock();
	gotoxy(x, y);
	SetColor(11, 0);
	cout << " ";
	SetColor(0, 14);
	cout << " GOAL ";
	SetColor(12, 0);
	cout << " ";
	gotoxy(x + 2, y + 2);
	SetColor(0, 14);
	cout << goal;
	m.unlock();
}

void print_ratio(int x, int y, int num1, int num2)
{
	m.lock();
	gotoxy(x - Count(num1) - 1, y);
	SetColor(0, 11);
	cout << num1;
	SetColor(0, 14);
	cout << " - ";
	SetColor(0, 12);
	cout << num2;
	gotoxy(x, y);
	gotoxy(x - 4, y + 10);
	SetColor(0, 11);
	cout << "[B]: ";
	SetColor(0, 14);
	cout << "Back";
	m.unlock();
}

void printInfoUser(int x, int y, User user, int size, int index)
{
	m.lock();
	if (index == 2)
	{
		gotoxy(x + 8 * size + 3, y);
		SetColor(0, 11);
		cout << char(219) << " PLAYER 2";
		gotoxy(x + 8 * size + 3, y + 1);
		cout << char(219) << " ";
		SetColor(0, 14);
		cout << user.name;
		SetColor(0, 11);
		gotoxy(x + 8 * size + 3, y + 2);
		cout << char(219) << " ";
		SetColor(0, 14);
		cout << user.score;
	}
	else
	{
		gotoxy(x - 11, y);
		SetColor(0, 12);
		cout << "PLAYER 1 " << char(219);
		gotoxy(x - user.name.size() - 3, y + 1);
		SetColor(0, 14);
		cout << user.name << " ";
		SetColor(0, 12);
		cout << char(219);
		SetColor(0, 14);
		gotoxy(x - Count(user.score) - 3, y + 2);
		cout << user.score << " ";
		SetColor(0, 12);
		cout << char(219);
	}
	m.unlock();
}

void printDetail(int x, int y, int space, int size, int mode, int goal, int time, int countdown, int counter)
{
	m.lock();
	gotoxy(x + space + 5, y + 3);
	if (counter == 2)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	if (size == 4)
		cout << "4x4";
	else
		cout << "5x5";
	gotoxy(x + space + 6, y + 8);
	if (counter == 3)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	if (countdown)
		cout << "On ";
	else
		cout << "Off";
	gotoxy(x + space * 2 + 5, y + 3);
	if (counter == 4)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	if (mode == 0)
		cout << "GOAL";
	else
		cout << "TIME";
	gotoxy(x + space * 2 + 5, y + 8);
	if (counter == 5)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	if (mode == 0)
		cout << goal << " ";
	else
		cout << " " << time << "m  ";
	m.unlock();
}

void reduce_second(Time& time)
{
	time.second--;
	if (time.second == -1)
	{
		time.second = 59;
		time.minute--;
		if (time.minute == -1)
		{
			time.minute = 59;
			time.hour--;
		}
	}
}

void TimeLeft(int x, int y, Time time, bool& pause, bool& clock_stopped, int mode)    // pause = true - hết thời gian 
{
	if (mode == 0)
		return;
	int x_next, y_next;
	m.lock();
	SetColor(7, 0);
	printNumber(x, y, time.minute / 10);
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
	while ((time.minute != 0 || time.second != 0) && !clock_stopped)
	{
		Sleep(1000);
		reduce_second(time);
		m.lock();
		SetColor(7, 0);
		printNumber(x, y, time.minute / 10);
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
	if (!clock_stopped)
		pause = false;
	pause = true;
}

int isWin(int** matrix, int size, int mode, int goal)	   // -1 loss, 1 win 
{
	if (mode == 0)
	{
		if (checkWin(matrix, size, goal))
			return 1;
	}
	if (checkGOV(matrix, size))
		return -1;
	return 0;
}

bool winScene(int winIndex)		// true - chơi tiếp , false - thoát 
{
	bool run = true;
	char key;
	int x1, y1, x2, y2, counter = 1, color0 = 14, color1, color2;
	string win = "WIN", loss = "LOSS", dawn = "DRAW", title1, title2;
	if (winIndex == 1)
	{
		x1 = 105; y1 = 10;
		x2 = 16; y2 = 10;
		title1 = win;
		title2 = loss;
	}
	else if (winIndex == 2)
	{
		x1 = 104; y1 = 10;
		x2 = 20; y2 = 10;
		title2 = win;
		title1 = loss;
	}
	else
	{
		x1 = 96; y1 = 10;
		x2 = 20; y2 = 10;
		title1 = dawn;
		title2 = dawn;
	}
	if (winIndex == 0)
	{
		color1 = color2 = 8;
		color0 = 15;
	}
	else if (winIndex == 1)
	{
		color1 = 11;
		color2 = 12;
	}
	else
	{
		color1 = 12;
		color2 = 11;
	}
	thread winthr(printTitle, x1, y1, title1, ref(run), color0, color1);
	thread lossthr(printTitle, x2, y2, title2, ref(run), color0, color2);
	while (start)
	{
		printButton(71, 20, "   BACK   ", false, 11, 14);
		printButton(69, 23, "  PLAY AGAIN  ", false, 11, 14);
		if (counter == 0)
			printButton(71, 20, "   BACK   ", true, 11, 14);
		else
			printButton(69, 23, "  PLAY AGAIN  ", true, 11, 14);
		key = _getch();
		if (key == Enter)
		{
			run = false;
			break;
		}
		if (key == Up)
			counter = 0;
		if (key == Down)
			counter = 1;
	}
	if (winthr.joinable())
		winthr.join();
	if (lossthr.joinable())
		lossthr.join();
	if (counter == 0)
		return false;
	else
		return true;
}

void getNameTwoPlayer(int x, int y, string& name1, string& name2)
{
	bool run = true;
	string title = "MINI GAME";
	thread thr(printTitle, 40, 8, title, ref(run), 11, 14);
	char key;
	warning();
	name1 = "";
	name2 = "";
	for (int i = 1; i <= 2; i++)
	{
		string name = "";
		if (i == 1)
			printNotice(x - 3, y, "ENTER PLAYER 1'S NAME");
		else
			printNotice(x - 3, y, "ENTER PLAYER 2'S NAME");
		m.lock();
		SetColor(0, 14);
		gotoxy(x, y + 3);
		cout << char(175);
		gotoxy(x + 18, y + 3);
		cout << char(174);
		m.unlock();
		while (start)
		{
			key = _getch();
			clearScreen(x - 24, y + 5, 1, 66);
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
				else if (name == name1)
				{
					m.lock();
					gotoxy(x - 24, y + 5);
					SetColor(0, 12);
					cout << "The name entered for Player 2 is the same as the previous player.";
					m.unlock();
					continue;
				}
				else
				{
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
					SetColor(0, 0);
					gotoxy(x + 8, y + 3);
					cout << "   ";
					m.unlock();
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
			if (name.size() > 0)
				cout << name[name.size() - 1];
			for (int i = name.size(); i < 15; i++)
				cout << " ";
			m.unlock();
		}
		if (i == 1)
			name1 = name;
		else
			name2 = name;
	}
	run = false;
	if (thr.joinable())
		thr.join();
	clearScreen(x - 3, y, 4, 25);
}

void processUpAndPrint(int** matrix, int size, int x, int y, User& user, int index)
{
	if (processUp(matrix, size, user.score))
	{
		randomItem(matrix, size);
		printMatrix(matrix, size, x, y);
		printInfoUser(x, y, user, size, index);
	}
}

void processDownAndPrint(int** matrix, int size, int x, int y, User& user, int index)
{
	if (processDown(matrix, size, user.score))
	{
		randomItem(matrix, size);
		printMatrix(matrix, size, x, y);
		printInfoUser(x, y, user, size, index);
	}
}

void processLeftAndPrint(int** matrix, int size, int x, int y, User& user, int index)
{
	if (processLeft(matrix, size, user.score))
	{
		randomItem(matrix, size);
		printMatrix(matrix, size, x, y);
		printInfoUser(x, y, user, size, index);
	}
}

void processRightAndPrint(int** matrix, int size, int x, int y, User& user, int index)
{
	if (processRight(matrix, size, user.score))
	{
		randomItem(matrix, size);
		printMatrix(matrix, size, x, y);
		printInfoUser(x, y, user, size, index);
	}
}

void pvpScene(Scene& scene)
{
	SetColor(0, 0);
	system("cls");
	makeFrames();
	warning();

	bool run = true;
	char key;
	int counter = 0, mode = 0, size = 4, countdown = 1, time = 1, goal = 512, x = 35, y = 23, space = 22;
	string title = "SETTING";
	thread thr(printTitle, 45, 8, title, ref(run), 11, 14);
	structionBoard(x + 8, y - 2, "SETTING", 3, 43);
	while (start)
	{
		printButton(x - space / 2, y, "  BACK  ", false, 11, 14);
		printButton(x - space / 2, y + 3, "  PLAY  ", false, 11, 14);
		printButton(x + space, y, "    SIZE    ", false, 11, 14);
		printButton(x + space, y + 5, " COUNT DOWN ", false, 11, 14);
		printButton(x + space * 2, y, "    MODE    ", false, 11, 14);
		if (mode == 0)
			printButton(x + space * 2, y + 5, "    GOAL    ", false, 11, 14);
		else
			printButton(x + space * 2, y + 5, "    TIME    ", false, 11, 14);

		if (counter == 0)
			printButton(x - space / 2, y, "  BACK  ", true, 11, 14);
		else if (counter == 1)
			printButton(x - space / 2, y + 3, "  PLAY  ", true, 11, 14);
		else if (counter == 2)
			printButton(x + space, y, "    SIZE    ", true, 11, 14);
		else if (counter == 3)
			printButton(x + space, y + 5, " COUNT DOWN ", true, 11, 14);
		else if (counter == 4)
			printButton(x + space * 2, y, "    MODE    ", true, 11, 14);
		else {
			if (mode == 0)
				printButton(x + space * 2, y + 5, "    GOAL    ", true, 11, 14);
			else
				printButton(x + space * 2, y + 5, "    TIME    ", true, 11, 14);
		}
		printDetail(x, y, space, size, mode, goal, time, countdown, counter);
		key = _getch();
		if (key == Enter)
		{
			if (counter == 0 || counter == 1)
			{
				break;
			}
			else if (counter == 2)
			{
				if (size == 4)
					size = 5;
				else
					size = 4;
			}
			else if (counter == 3)
			{
				countdown = 1 - countdown;
			}
			else if (counter == 4)
				mode = 1 - mode;
			else if (mode == 0)
			{
				goal *= 2;
				if (goal > 2048)
					goal = 512;
			}
			else
			{
				time += 2;
				if (time > 5)
					time = 1;
			}
		}
		if (key == prefix_value)
			key = _getch();
		if (key == Up)
		{
			if ((counter - 1) % 2 == 0 && (counter - 1 >= 0))
				counter -= 1;
		}
		else if (key == Down)
		{
			if ((counter + 1) % 2 == 1 && (counter + 1 <= 5))
				counter += 1;
		}
		else if (key == Left)
		{
			if (counter - 2 >= 0)
				counter -= 2;
		}
		else if (key == Right)
		{
			if (counter + 2 <= 5)
				counter += 2;
		}
	}
	run = false;
	if (thr.joinable())
		thr.join();
	if (counter == 1)
		playPvP(size, mode, goal, time, countdown);
	scene = minigame;
}

void playPvP(int size, int mode, int goal, int time, int countdown)
{
	SetColor(0, 0);
	system("cls");
	makeFrames();
	warning();

	User user1, user2;
	getNameTwoPlayer(65, 20, user1.name, user2.name);
	clearScreen(38, 8, 6, 76);

	char key;
	bool run = true, pause = false, playagain, clock_stopped = false;
	int** matrix1, ** matrix2, x1 = 103, y1 = 18, x2 = 8 + (5 - size) * 8, y2 = 18, win1 = 0, win2 = 0, player1_ratio = 0, player2_ratio = 0;
	Time Time;
	string title = "MINI GAME";
	thread thr(printTitle, 40, 2, title, ref(run), 11, 14);
	Time.minute = time;
	while (start)
	{
		run = true;
		if (countdown)
			countDown(65, 15);
		if (mode == 0)
			printGoal(73, 12, goal);
		thread clo(TimeLeft, 70, 12, Time, ref(pause), ref(clock_stopped), mode);
		print_ratio(76, 18, player2_ratio, player1_ratio);
		makeMatrix(matrix1, size);
		makeMatrix(matrix2, size);
		printFrameMatrix(x1, y1, size);
		printFrameMatrix(x2, y2, size);
		printMatrix(matrix1, size, x1, y1);
		printMatrix(matrix2, size, x2, y2);
		printInfoUser(x1, y1, user1, size, 1);
		printInfoUser(x2, y2, user2, size, 2);
		while (start)
		{
			if (mode == 1 && pause == true)
			{
				if (user1.score > user2.score)
				{
					player1_ratio++;
					print_ratio(76, 18, player2_ratio, player1_ratio);
					playagain = winScene(1);
				}
				else if (user1.score < user2.score)
				{
					player2_ratio++;
					print_ratio(76, 18, player2_ratio, player1_ratio);
					playagain = winScene(2);
				}
				else
				{
					player1_ratio++;
					player2_ratio++;
					print_ratio(76, 18, player2_ratio, player1_ratio);
					playagain = winScene(0);

				}
				break;
			}
			if (_kbhit())
			{
				key = _getch();
			}
			else
				continue;
			if (key == prefix_value)
			{
				key = _getch();
			}
			if (key == 'B' || key == 'b')
			{
				clock_stopped = true;
				playagain = false;
				break;
			}
			if (key == Up)
			{
				processUpAndPrint(matrix1, size, x1, y1, user1, 1);
			}
			else if (key == Down)
			{
				processDownAndPrint(matrix1, size, x1, y1, user1, 1);
			}
			else if (key == Left)
			{
				processLeftAndPrint(matrix1, size, x1, y1, user1, 1);
			}
			else if (key == Right)
			{
				processRightAndPrint(matrix1, size, x1, y1, user1, 1);
			}
			else if (key == W)
			{
				processUpAndPrint(matrix2, size, x2, y2, user2, 2);
			}
			else if (key == S)
			{
				processDownAndPrint(matrix2, size, x2, y2, user2, 2);
			}
			else if (key == A)
			{
				processLeftAndPrint(matrix2, size, x2, y2, user2, 2);
			}
			else if (key == D)
			{
				processRightAndPrint(matrix2, size, x2, y2, user2, 2);
			}
			win1 = isWin(matrix1, size, mode, goal);
			win2 = isWin(matrix2, size, mode, goal);
			if (win1 == 1 || win2 == -1)
			{
				player1_ratio++;
				clock_stopped = true;
				print_ratio(76, 18, player2_ratio, player1_ratio);
				playagain = winScene(1);
				break;
			}
			else if (win1 == -1 || win2 == 1)
			{
				clock_stopped = true;
				player2_ratio++;
				print_ratio(76, 18, player2_ratio, player1_ratio);
				playagain = winScene(2);
				break;
			}
		}
		freeMatrix(matrix1, size);
		freeMatrix(matrix2, size);
		if (playagain)
		{
			m.lock();
			SetColor(0, 0);
			system("cls");
			m.unlock();
			makeFrames();
			warning();
			Time.minute = time;
			user1.score = 0;
			user2.score = 0;
			pause = false;
			clock_stopped = false;
			win1 = 0, win2 = 0;
			if (clo.joinable())
				clo.join();
		}
		else
		{
			if (clo.joinable())
				clo.join();
			break;
		}
	}
	run = false;
	if (thr.joinable())
		thr.join();
}


// Phần Chướng Ngại Vật 

void printnBlocks(int x, int y, int nBlocks)
{
	m.lock();
	gotoxy(x, y);
	SetColor(0, 11);
	cout << char(219) << " REMAINING BLOCKS: ";
	SetColor(0, 14);
	cout << nBlocks << "   ";
	gotoxy(x, y + 2);
	SetColor(0, 11);
	cout << "[" << char(30) << "] : ";
	SetColor(0, 14);
	cout << "Up";
	gotoxy(x, y + 3);
	SetColor(0, 11);
	cout << "[" << char(31) << "] : ";
	SetColor(0, 14);
	cout << "Down";
	gotoxy(x, y + 4);
	SetColor(0, 11);
	cout << "[" << char(17) << "] : ";
	SetColor(0, 14);
	cout << "Left";
	gotoxy(x, y + 5);
	SetColor(0, 11);
	cout << "[" << char(16) << "] : ";
	SetColor(0, 14);
	cout << "Right";
	gotoxy(x, y + 6);
	SetColor(0, 11);
	cout << "[R] : ";
	SetColor(0, 14);
	cout << "Reset Map";
	gotoxy(x, y + 7);
	SetColor(0, 11);
	cout << "[B] : ";
	SetColor(0, 14);
	cout << "Back";
	gotoxy(x, y + 8);
	SetColor(0, 11);
	cout << "[ENTER] :";
	SetColor(0, 14);
	cout << "Drop";
	m.unlock();
}

void printInfoUserMiniGame(int x, int y, int goal, User user)
{
	int rank;
	m.lock();
	SetColor(0, 11);
	gotoxy(144, 19);
	cout << char(219);
	gotoxy(x + 5, y + 4);
	cout << "GOAL  : ";
	gotoxy(x + 5, y + 6);
	cout << "SCORE : ";
	SetColor(0, 14);
	gotoxy(x + 13, y + 4);
	if (goal != -2)
		cout << " 2048 ";
	else
		cout << " Infinite";
	gotoxy(x + 13, y + 6);
	cout << " " << user.score;
	for (int i = 1; i <= 11 - Count(user.score); i++)
		cout << " ";
	gotoxy(x, y + 11);
	SetColor(0, 11);
	cout << "[E] : ";
	SetColor(0, 14);
	cout << "Exit";
	m.unlock();
}

bool makeMap(int** matrix, int size, int x, int y)
{
	int nBlock = size * size / 4, xCounter = 0, yCounter = 0, flag = false;
	char key;
	clearScreen(1, 7, 30, 150);
	if (matrix != NULL)
	{
		while (start)
		{
			if(matrix[yCounter] != NULL)
				matrix[yCounter][xCounter] = -2;
			printnBlocks(x + 4 + size * 8, y, nBlock);
			printFrameMatrix(x, y, size);
			printMatrix(matrix, size, x, y);
			key = _getch();
			if (key == Enter)
			{
				if(matrix[yCounter] != NULL)
					matrix[yCounter][xCounter] = -1;
				nBlock--;
				if (nBlock == 0)
				{
					printMatrix(matrix, size, x, y);
					printnBlocks(x + 4 + size * 8, y, nBlock);
					if (Question(x - 5, y + 12, "ARE YOU READY TO START THE GAME WITH THIS MAP? (NO TO CREATE A NEW MAP.)"))
						return true;
					else
					{
						clearScreen(70, 15, 1, 73);
						flag = true;
					}
				}
				int bre = 0, success = false;
				for(int i = yCounter; i < size; i++)
				{
					int j = 0;
					if (i == yCounter)
						j = xCounter + 1;
					for (; j < size; j++)
						if (matrix[i] != NULL && matrix[i][j] == 0)
						{
							xCounter = j;
							yCounter = i;
							bre = 1;
							success = true;
							break;
						}
					if (bre)
						break;
				}
				if (!success)
				{
					for (int i = 0; i < size; i++)
					{
						for (int j = 0; j < size; j++)
							if (matrix[i] != NULL && matrix[i][j] == 0)
							{
								xCounter = j;
								yCounter = i;
								bre = 1;
								break;
							}
						if (bre)
							break;
					}
				}
			}
			if (key == 'r' || key == 'R' || flag)
			{
				flag = false;
				for (int i = 0; i < size; i++)
					for (int j = 0; j < size; j++)
						if (matrix[i] != NULL)
							matrix[i][j] = 0;
				xCounter = 0;
				yCounter = 0;
				nBlock = size * size / 4;
				continue;
			}
			if (key == 'b' || key == 'B')
			{
				for (int i = 0; i < size; i++)
					for (int j = 0; j < size; j++)
						matrix[i][j] = 0;
				return false;
			}
			if (key == Up)
			{
				if (yCounter - 1 >= 0 && matrix[yCounter - 1][xCounter] == -1 || yCounter - 1 < 0)
					continue;
				if (matrix[yCounter] != NULL)
					matrix[yCounter][xCounter] = 0;
				yCounter--;
				if (yCounter == -1)
					yCounter = size - 1;
			}
			else if (key == Down)
			{
				if (yCounter + 1 < size && matrix[yCounter + 1][xCounter] == -1 || yCounter + 1 >= size)
					continue;
				if (matrix[yCounter] != NULL)
					matrix[yCounter][xCounter] = 0;
				yCounter++;
				if (yCounter == size)
					yCounter = 0;
			} 
			else if (key == Left)
			{
				if (xCounter - 1 >= 0 && matrix[yCounter][xCounter - 1] == -1 || xCounter - 1 < 0)
					continue;
				if (matrix[yCounter] != NULL)
					matrix[yCounter][xCounter] = 0;
				xCounter--;
				if (xCounter == -1)
					xCounter = size - 1;
			}
			else if (key == Right)
			{
				if (xCounter + 1 < size && matrix[yCounter][xCounter + 1] == -1 || xCounter + 1 >= size)
					continue;
				if (matrix[yCounter] != NULL)
					matrix[yCounter][xCounter] = 0;
				xCounter++;
				if (xCounter == size)
					xCounter = 0;
			}
		}
	}
}

void WinGameSceneMiniGame(int x, int y, int& mode,int& goal,User user)
{
	string win = "WIN";
	bool run = true;
	thread thr(printTitle, x, y, win, ref(run), 11, 14);
	char key;
	mode = 1;
	m.lock();
	SetColor(0, 14);
	gotoxy(x + 2, y + 7);
	cout << char(219) << " " << user.name;
	gotoxy(x + 2, y + 8);
	cout << char(219);
	SetColor(0, 11);
	gotoxy(x + 4, y + 8);
	cout << "SCORE: " << user.score;
	m.unlock();
	while (start)
	{
		printButton(x - 4, y + 10, "   BACK   ", false, 11, 14);
		printButton(x + 16, y + 10, "  CONTINUE  ", false, 11, 14);
		if (mode == 0)
			printButton(x - 4, y + 10, "   BACK   ", true, 11, 14);
		else
			printButton(x + 16, y + 10, "  CONTINUE  ", true, 11, 14);
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
			goal = -2;
	}
	run = false;
	if (thr.joinable())
		thr.join();
}

void GOVSceneMiniGame(int x, int y, int goal, User user)
{
	string loss = "LOSS";
	bool run = true;
	thread thr(printTitle, x - 6, y, loss, ref(run), 11, 12);
	char key;
	m.lock();
	SetColor(0, 11);
	gotoxy(x + 2, y + 7);
	cout << char(219) << " " << user.name;
	gotoxy(x + 2, y + 8);
	cout << char(219);
	SetColor(0, 12);
	gotoxy(x + 4, y + 8);
	cout << "SCORE: " << user.score;
	m.unlock();
	printButton(x + 3, y + 10, "   BACK   ", true, 11, 12);
	while (start)
	{
		key = _getch();
		printButton(x + 3, y + 10, "   BACK   ", true, 11, 12);
		if (key == Enter)
			break;
	}
	run = false;
	if (thr.joinable())
		thr.join();
}

void copyMatrix(int** dest, int** scr, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			dest[i][j] = scr[i][j];
}

void playBlocks(int** map, int n)
{
	SetColor(0, 0);
	system("cls");
	makeFrames();

	char button;
	bool run = true, pause = false, change;
	int xo, yo, goal = 2048;
	string title = "MINI GAME";

	User user;
	Top20List list;
	list.top1 = NULL;
	getUserName(65, 20, user.name, list);
	clearScreen(38, 8, 6, 75);

	countDown(65, 15);
	thread thr(printTitle, 40, 2, title, ref(run), 11, 14);
	thread clo(Clock, 123, 11, ref(user.time), ref(run), ref(pause));
	xo = 6 + (10 - n) * 6, yo = 4 + (10 - n) * 2;
	randomItem(map, n);
	randomItem(map, n);
	printFrameMatrix(xo, yo, n);
	printMatrix(map, n, xo, yo);
	structionBoard(115, 21, user.name, 0, 15 - user.name.size());
	printInfoUserMiniGame(115, 21, goal, user);
	change = false;
	while (start)
	{
		button = _getch();
		clearScreen(124, 19, 1, 20);
		if (button == prefix_value)						// Loại bỏ giá trị tiền đề (prefix value) cho các phím mũi tên 
			button = _getch();		// Lưu matrix hiện tại qua pre_matrix	
		if (button == Up || button == W)
			change = processUpMiniGame(map, n, user.score);
		else if (button == Down || button == S)
			change = processDownMiniGame(map, n, user.score);
		else if (button == Left || button == A)
			change = processLeftMiniGame(map, n, user.score);
		else if (button == Right || button == D)
			change = processRightMiniGame(map, n, user.score);
		else if (button == Exit)
		{
			pause = true;
			if (Question(60, 15, "  Do you really want to exit the game?  "))
			{
				break;
			}
			else
			{
				clearScreen(1, 1, 41, 83);
				clearScreen(75, 2, 6, 75);
				countDown(40, 15);
				pause = false;
				printFrameMatrix(xo, yo, n);
				printMatrix(map, n, xo, yo);
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
		printMatrix(map, n, xo, yo);
		printInfoUserMiniGame(115, 21, goal,user);
		randomItem(map, n);
		Sleep(100);
		printMatrix(map, n, xo, yo);
		if (checkWin(map, n, goal))
		{
			int mode;
			pause = true;
			clearScreen(115, 21, 13, 30);
			WinGameSceneMiniGame(108, 21, mode, goal, user);
			if (mode == 0)
				break;
			else
			{
				pause = false;
				clearScreen(108 - 4, 21, 13, 34);
				printMatrix(map, n, xo, yo);
				structionBoard(115, 21, user.name, 0, 15 - user.name.size());
				printInfoUserMiniGame(115, 21, goal, user);
				continue;
			}
		}
		if (checkGOV(map, n))
		{
			pause = true;
			clearScreen(115, 21, 13, 30);
			GOVSceneMiniGame(108, 21, goal, user);
			break;
		}
	}
	run = false;
	if (thr.joinable())
		thr.join();
	if (clo.joinable())
		clo.join();
	freeMatrix(map, n);
}

void blocksScene(Scene& scene)
{
	SetColor(0, 0);
	system("cls");
	makeFrames();

	char key;
	bool run = true;
	int counter = 0, x = 20, y = 15, xMatrix = 44, yMatrix = 9;
	string title = "MINI GAME";
	thread thr(printTitle, 40, 2, title, ref(run), 11, 14);

	int** map0 = InitMatrix(8);
	int** map1 = InitMatrix(8);
	int** map2 = InitMatrix(8);
	int** map3 = InitMatrix(8);
	int** map4 = InitMatrix(8);

	map1[0][5] = map1[0][6] = map1[1][1] = map1[1][2] = map1[1][3] = map1[2][6] = map1[2][7] = map1[3][1] = map1[3][4] = map1[4][1] = map1[4][4] = map1[5][1] = map1[6][3] = map1[6][4] = map1[6][5] = map1[6][6] = -1;
	map2[1][1] = map2[1][2] = map2[2][1] = map2[1][5] = map2[1][6] = map2[3][3] = map2[3][4] = map2[3][5] = map2[5][2] = map2[6][2] = map2[7][3] = map2[7][4] = map2[5][6] = map2[5][7] = map2[6][7] = map2[6][6] = -1;
	map3[1][1] = map3[1][2] = map3[1][3] = map3[1][6] = map3[0][6] = map3[2][2] = map3[3][5] = map3[4][4] = map3[4][5] = map3[4][6] = map3[5][5] = map3[6][1] = map3[6][4] = map3[7][4] = map3[7][5] = map3[7][6] = -1;
	map4[0][4] = map4[1][1] = map4[1][5] = map4[1][6] = map4[2][6] = map4[3][2] = map4[3][3] = map4[4][2] = map4[4][5] = map4[4][6] = map4[4][7] = map4[6][1] = map4[6][2] = map4[6][3] = map4[6][6] = map4[7][6] = -1;

	while (start)
	{
		printButton(x, y,     "   MAP 1    ", false, 11, 14);
		printButton(x, y + 2, "   MAP 2    ", false, 11, 14);
		printButton(x, y + 4, "   MAP 3    ", false, 11, 14);
		printButton(x, y + 6, "   MAP 4    ", false, 11, 14);
		printButton(x, y + 8," CREATE MAP ", false, 11, 14);
		printButton(x, y + 10,"    BACK    ", false, 11, 14);
		if (counter == 0)
		{
			printButton(x, y, "   MAP 1    ", true, 11, 14);
			printFrameMatrix(xMatrix, yMatrix, 8);
			printMatrix(map1, 8, xMatrix, yMatrix );
		}
		else if (counter == 1)
		{
			printButton(x, y + 2, "   MAP 2    ", true, 11, 14);
			printFrameMatrix(xMatrix, yMatrix, 8);
			printMatrix(map2, 8, xMatrix, yMatrix);
		}
		else if (counter == 2)
		{
			printButton(x, y + 4, "   MAP 3    ", true, 11, 14);
			printFrameMatrix(xMatrix, yMatrix, 8);
			printMatrix(map3, 8, xMatrix, yMatrix);
		}
		else if (counter == 3)
		{
			printButton(x, y + 6, "   MAP 4    ", true, 11, 14);
			printFrameMatrix(xMatrix, yMatrix, 8);
			printMatrix(map4, 8, xMatrix, yMatrix);
		}
		else if (counter == 4)
		{
			printButton(x, y + 8, " CREATE MAP ", true, 11, 14);
			printFrameMatrix(xMatrix, yMatrix, 8);
			printMatrix(map0, 8, xMatrix, yMatrix);
		}
		else
		{
			printButton(x, y + 10, "    BACK    ", true, 11, 14);
			clearScreen(xMatrix, yMatrix, 33, 66);
		}
		key = _getch();
		if (key == prefix_value)
			key = _getch();
		if (key == Enter)
		{
			if (counter == 0)
			{
				copyMatrix(map0, map1, 8);
				break;
			}
			else if (counter == 1)
			{
				copyMatrix(map0, map2, 8);
				break;
			}
			else if (counter == 2)
			{
				copyMatrix(map0, map3, 8);
				break;
			}
			else if (counter == 3)
			{
				copyMatrix(map0, map4, 8);
				break;
			}
			else if (counter == 4)
			{
				if (makeMap(map0, 8, xMatrix, yMatrix))
					break;
				else
				{
					clearScreen(1, 7, 30, 150);
					counter = 0;
					continue;
				}
			}
			else
				break;
		}
		if (key == Up)
		{
			counter--;
			if (counter == -1)
				counter = 5;
		}
		else if (key == Down)
		{
			counter++;
			counter %= 6;
		}
	}
	run = false;
	if (thr.joinable())
		thr.join();
	freeMatrix(map1, 8);
	freeMatrix(map2, 8);
	freeMatrix(map3, 8);
	freeMatrix(map4, 8);
	if (counter == 5)
		freeMatrix(map0, 8);
	else
		playBlocks(map0, 8);
	scene = minigame;
}

bool processLeftOnLine(int* line, int n, unsigned int& score, int begin, int end)
{
	bool change = false;
	int idex = begin, i, j;									// idex là chỉ số sẽ di chuyển từ trái qua phải để dồn các phần tử về trái  
	for (i = begin; i <= end; i++)
	{
		if (line[i] == 0)								// Tìm phần tử khác 0 
			continue;
		for (j = i + 1; j <= end && line[j] == 0; j++);	// Tìm phần tử khác 0 kế tiếp 
		if (j == end + 1)										// Nếu không còn phần tử khác 0 thì dừng duyệt hàng 
			break;
		if (line[i] == line[j])							// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn về bên trái, tiếp tục duyệt mảng 
		{
			line[idex] = line[i] + line[j];
			score += line[idex];
			if (i != idex)
				line[i] = 0;
			line[j] = 0;
			idex++;
			change = true;								// đánh dấu mảng đã thay đổi
		}
		else
		{
			if (i != idex)								// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này về trái và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
			{
				line[idex] = line[i];
				line[i] = 0;
				change = true;
			}
			idex++;
			if (j != idex)
			{
				line[idex] = line[j];
				line[j] = 0;
				change = true;
			}
			i = idex - 1;
		}
	}
	if (i <= end && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn về trái 
	{
		line[idex] = line[i];
		line[i] = 0;
		change = true;
	}
	return change;
}

bool processRightOnLine(int* line, int n, unsigned int& score, int begin, int end)
{
	bool change = false;
	int idex = end, i, j;									// idex là chỉ số sẽ di chuyển từ phải qua trái để dồn các phần tử về phải   
	for (i = end; i >= begin; i--)
	{
		if (line[i] == 0)									// Tìm phần tử khác 0 
			continue;
		for (j = i - 1; j >= begin && line[j] == 0; j--);		// Tìm phần tử khác 0 kế tiếp 
		if (j == begin - 1)										// Nếu không còn phần tử khác 0 thì dừng duyệt hàng 
			break;
		if (line[i] == line[j])								// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn về bên phải, tiếp tục duyệt mảng 
		{
			line[idex] = line[i] + line[j];
			score += line[idex];
			if (i != idex)
				line[i] = 0;
			line[j] = 0;
			idex--;
			change = true;									// đánh dấu mảng đã thay đổi
		}
		else
		{
			if (i != idex)									// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này về phải và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
			{
				line[idex] = line[i];
				line[i] = 0;
				change = true;
			}
			idex--;
			if (j != idex)
			{
				line[idex] = line[j];
				line[j] = 0;
				change = true;
			}
			i = idex + 1;
		}
	}
	if (i >= begin && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn về phải  
	{
		line[idex] = line[i];
		line[i] = 0;
		change = true;
	}
	return change;
}

bool processUpOnColumn(int** matrix, int col, int n, unsigned int& score, int begin, int end)
{
	bool change = false;
	int idex = begin, i, j;									// idex là chỉ số sẽ di chuyển từ trên xuống dưới để dồn các phần tử lên trên   
	for (i = begin; i <= end; i++)
	{
		if (matrix[i][col] == 0)								// Tìm phần tử khác 0 
			continue;
		for (j = i + 1; j <= end && matrix[j][col] == 0; j++);	// Tìm phần tử khác 0 kế tiếp 
		if (j == end + 1)										    // Nếu không còn phần tử khác 0 thì dừng duyệt cột 
			break;
		if (matrix[i][col] == matrix[j][col])							// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn lên trên, tiếp tục duyệt mảng 
		{
			matrix[idex][col] = matrix[i][col] + matrix[j][col];
			score += matrix[idex][col];
			if (i != idex)
				matrix[i][col] = 0;
			matrix[j][col] = 0;
			idex++;
			change = true;								// đánh dấu mảng đã thay đổi
		}
		else
		{
			if (i != idex)								// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này lên trên và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
			{
				matrix[idex][col] = matrix[i][col];
				matrix[i][col] = 0;
				change = true;
			}
			idex++;
			if (j != idex)
			{
				matrix[idex][col] = matrix[j][col];
				matrix[j][col] = 0;
				change = true;
			}
			i = idex - 1;
		}
	}
	if (i <= end && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn lên trên  
	{
		matrix[idex][col] = matrix[i][col];
		matrix[i][col] = 0;
		change = true;
	}
	return change;
}

bool processDownOnColumn(int** matrix, int col, int n, unsigned int& score, int begin, int end)
{
	bool change = false;
	int idex = end, i, j;									// idex là chỉ số sẽ di chuyển từ dưới lên trên để dồn các phần tử xuống dưới   
	for (i = end; i >= begin; i--)
	{
		if (matrix[i][col] == 0)									// Tìm phần tử khác 0 
			continue;
		for (j = i - 1; j >= begin && matrix[j][col] == 0; j--);		// Tìm phần tử khác 0 kế tiếp 
		if (j == begin - 1)										// Nếu không còn phần tử khác 0 thì dừng duyệt cột  
			break;
		if (matrix[i][col] == matrix[j][col])								// Nếu hai phần tử khác 0 bằng nhau thì cộng hai phần tử và dồn xuống dưới, tiếp tục duyệt mảng 
		{
			matrix[idex][col] = matrix[i][col] + matrix[j][col];
			score += matrix[idex][col];
			if (i != idex)
				matrix[i][col] = 0;
			matrix[j][col] = 0;
			idex--;
			change = true;									// đánh dấu mảng đã thay đổi
		}
		else
		{
			if (i != idex)									// Nếu hai phần tử khác 0 khác nhau thì dồn hai phần tử này xuống dưới và tiếp tục duyệt mảng tại phần tử khác không thứ 2 
			{
				matrix[idex][col] = matrix[i][col];
				matrix[i][col] = 0;
				change = true;
			}
			idex--;
			if (j != idex)
			{
				matrix[idex][col] = matrix[j][col];
				matrix[j][col] = 0;
				change = true;
			}
			i = idex + 1;
		}
	}
	if (i >= begin && i != idex)								// Nếu phần tử khác 0 cuối cùng chưa được dồn thì dồn xuống dưới  
	{
		matrix[idex][col] = matrix[i][col];
		matrix[i][col] = 0;
		change = true;
	}
	return change;
}

bool processLeftMiniGame(int** matrix, int n, unsigned int& score)
{
	bool change = false;
	int begin, end;
	for (int i = 0; i < n; i++)		// Duyệt từng hàng 
	{
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == -1)
				continue;
			begin = j;
			for (int z = begin; ; z++)
			{
				if (((z + 1 == n) && matrix[i][z] != -1) || (matrix[i][z] != -1 && matrix[i][z + 1] == -1))
				{
					end = z;
					break;
				}
			}
			if (begin != end)
			{
				if (processLeftOnLine(matrix[i], n, score, begin, end))
					change = true;
			}
			j = end + 1;
		}
	}
	return change;
}

bool processRightMiniGame(int** matrix, int n, unsigned int& score)
{
	bool change = false;
	int begin, end;
	for (int i = 0; i < n; i++)		// Duyệt từng hàng 
	{
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == -1)
				continue;
			begin = j;
			for (int z = begin; ; z++)
			{
				if (((z + 1 == n) && matrix[i][z] != -1) || (matrix[i][z] != -1 && matrix[i][z + 1] == -1))
				{
					end = z;
					break;
				}
			}
			if (begin != end)
			{
				if (processRightOnLine(matrix[i], n, score, begin, end))
					change = true;
			}
			j = end + 1;
		}
	}
	return change;
}

bool processUpMiniGame(int** matrix, int n, unsigned int& score)
{
	bool change = false;
	int begin, end;
	for (int col = 0; col < n; col++)		// Duyệt từng cột 
	{
		for (int i = 0; i < n; i++)
		{
			if (matrix[i][col] == -1)
				continue;
			begin = i;
			for (int j = begin; ; j++)
			{
				if (((j + 1 == n) && matrix[j][col] != -1) || (matrix[j][col] != -1 && matrix[j + 1][col] == -1))
				{
					end = j;
					break;
				}
			}
			if (begin != end)
			{
				if (processUpOnColumn(matrix, col, n, score, begin, end))
					change = true;
			}
			i = end + 1;
		}
	}
	return change;
}

bool processDownMiniGame(int** matrix, int n, unsigned int& score)
{
	bool change = false;
	int begin, end;
	for (int col = 0; col < n; col++)		// Duyệt từng cột 
	{
		for (int i = 0; i < n; i++)
		{
			if (matrix[i][col] == -1)
				continue;
			begin = i;
			for (int j = begin; ; j++)
			{
				if (((j + 1 == n) && matrix[j][col] != -1) || (matrix[j][col] != -1 && matrix[j + 1][col] == -1))
				{
					end = j;
					break;
				}
			}
			if (begin != end)
			{
				if (processDownOnColumn(matrix, col, n, score, begin, end))
					change = true;
			}
			i = end + 1;
		}
	}
	return change;
}

