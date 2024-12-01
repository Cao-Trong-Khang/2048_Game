#include "Setting.h"

void print(int x, int y, int space, int level, int goal, int undo_redo, int speed, int countdown, int counter)
{
	m.lock();
	if (counter == 1)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	gotoxy(x + space + 2, y + 3);
	if (level == 4)
		cout << "  4 x 4  ";
	else if (level == 5)
		cout << "  5 x 5  ";
	else if (level == 6)
		cout << "  6 x 6  ";
	else if (level == 7)
		cout << "  7 x 7  ";
	else if (level == 8)
		cout << "  8 x 8  ";
	else if (level == 9)
		cout << "  9 x 9  ";
	else
		cout << " 10 x 10 ";
	if (counter == 2)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	gotoxy(x + space * 2 + 2, y + 3);
	if (goal == 2048)
		cout << "  2048  ";
	else if (goal == 4096)
		cout << "  4096  ";
	else if (goal == 8192)
		cout << "  8192  ";
	else
		cout << "Infinite";
	if (counter == 3)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	gotoxy(x + space * 3 + 1, y + 3);
	if (undo_redo == 0)
		cout << "     Off    ";
	else if (undo_redo == 1)
		cout << "    Undo    ";
	else
		cout << " Undo & Redo ";
	if (counter == 4)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	gotoxy(x + space * 4 + 5, y + 3);
	if (speed == 0)
		cout << "  0 ms";
	else
		cout << speed << " ms";
	if (counter == 5)
		SetColor(0, 14);
	else
		SetColor(0, 11);
	gotoxy(x + space * 5 + 8, y + 3);
	if (countdown)
		cout << "On ";
	else
		cout << "Off";
	m.unlock();
}

void printGuideSetting()
{
	m.lock();
	printText(55, 28, "Use the arrow buttons   to move buttons.", 11, 0);
	printText(55, 30, "Press [     ] button to change mode.", 11, 0);
	SetColor(0, 14);
	gotoxy(55 + 22, 28);
	cout << char(26);
	gotoxy(55 + 7, 30);
	cout << "ENTER";
	m.unlock();
}

void SettingScene(Scene& scene)
{
	SetColor(0, 0);
	system("cls");
	int level, goal, undo_redo, speed, countdown, counter = 0, x = 20, y = 18, space = 20;
	char key;
	bool run = true;
	makeFrames();
	warning();
	structionBoard(52, 25, "BUTTON FUNCTIONS", -1, 15);
	printGuideSetting();
	thread thr(printTitle, 45, 8, "SETTING", ref(run), 11, 14);
	readSettingFile(level, goal, undo_redo, speed, countdown);
	while (start)
	{
		printButton(x , y, " BACK MENU ", false, 11, 14);
		printButton(x + space, y, "   LEVEL   ", false, 11, 14);
		printButton(x + space * 2, y, "   GOAL   ", false, 11, 14);
		printButton(x + space * 3, y, " UNDO & REDO ", false, 11, 14);
		printButton(x + space * 4, y, " RANDOM SPEED ", false, 11, 14);
		printButton(x + space * 5 + 2, y, " COUNT DOWN ", false, 11, 14);
		print(x, y, space, level, goal, undo_redo, speed, countdown,counter);
		if (counter == 0)
			printButton(x, y, " BACK MENU ", true, 11, 14);
		if (counter == 1)
			printButton(x + space, y, "   LEVEL   ", true, 11, 14);
		if (counter == 2)
			printButton(x + space * 2, y, "   GOAL   ", true, 11, 14);
		if (counter == 3)
			printButton(x + space * 3, y, " UNDO & REDO ", true, 11, 14);
		if (counter == 4)
			printButton(x + space * 4, y, " RANDOM SPEED ", true, 11, 14);
		if (counter == 5)
			printButton(x + space * 5 + 2, y, " COUNT DOWN ", true, 11, 14);
		key = _getch();
		if (key == Enter)
		{
			if (counter == 0)
			{
				run = false;
				if (thr.joinable())
					thr.join();
				writeSettingFile(level, goal, undo_redo, speed, countdown);
				scene = menu;
				return;
			}
			else if (counter == 1)
			{
				level++;
				if (level == 11)
					level = 4;
				continue;
			}
			else if (counter == 2)
			{
				if (goal == -1)
					goal = 2048;
				else
				{
					goal *= 2;
					if (goal > 10000)
						goal = -1;
				}
				continue;
			}
			else if(counter == 3)
			{
				undo_redo++;
				undo_redo %= 3;
				continue;
			}
			else if(counter == 4)
			{
				speed += 100;
				if (speed > 500)
					speed = 0;
				continue;
			}
			else
			{
				countdown = 1 - countdown;
			}
		}
		if (key == prefix_value)
			key = _getch();
		if (key == Right)
		{
			counter++;
			counter %= 6;
		}
		else if (key == Left)
		{
			counter--;
			if (counter == -1)
				counter = 5;
		}
	}
}

void readSettingFile(int& level, int& goal, int& undo_redo, int& speed, int& countdown)
{
	fstream input;
	input.open("Setting.bin", ios::in | ios::binary);
	if (!input.is_open())
	{
		level = 4;															// Khởi gán giá trị mặc định cho lần đầu chạy 
		goal = 2048;						
		undo_redo = 0;
		speed = 200;
		countdown = 1;
		fstream output;
		output.open("Setting.bin", ios::out | ios::binary | ios::trunc);
		output.write((char*)&level, sizeof(level));
		output.write((char*)&goal , sizeof(goal));
		output.write((char*)&undo_redo, sizeof(undo_redo));
		output.write((char*)&speed, sizeof(speed));
		output.write((char*)&countdown, sizeof(countdown));
		output.close();
	}
	else
	{
		input.read((char*)&level, sizeof(level));
		input.read((char*)&goal, sizeof(goal));
		input.read((char*)&undo_redo, sizeof(undo_redo));
		input.read((char*)&speed, sizeof(speed));
		input.read((char*)&countdown, sizeof(countdown));
		input.close();
	}
}

void writeSettingFile(int level, int goal, int undo_redo, int speed, int countdown)
{
	fstream output;
	output.open("Setting.bin", ios::out | ios::binary | ios::trunc);
	output.write((char*)&level, sizeof(level));
	output.write((char*)&goal, sizeof(goal));
	output.write((char*)&undo_redo, sizeof(undo_redo));
	output.write((char*)&speed, sizeof(speed));
	output.write((char*)&countdown, sizeof(countdown));
	output.close();
}



