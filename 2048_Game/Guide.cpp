#include "Guide.h"

void printGameRules(int x, int y)
{
	structionBoard(x, y, "GAME RULES", 10, 53);
	m.lock();
	printText(x + 5, y + 5, "       In 2048 , you start with two tiles on a 4x4 grid , each tile", 14, 0);
	printText(x + 5, y + 6, "bearing a number , either 2 or 4 . Your goal is to combine tiles of", 14, 0);
	printText(x + 5, y + 7, "the same number to reach the coveted 2048 tile . You can move tiles", 14, 0);
	printText(x + 5, y + 8, "in four directions: up, down, left, and right . When two tiles with", 14, 0);
	printText(x + 5, y + 9, "the same number touch, they merge into 1 tile with the sum of their", 14, 0);
	printText(x + 5, y + 10, "numbers. The game ends when the grid is full, and no more moves are", 14, 0);
	printText(x + 5, y + 11, "possible . To win , strategize to keep higher-value tiles together, ", 14, 0);
	printText(x + 5, y + 12, "plan your moves ahead , and aim for the highest score possible.", 14, 0);
	printText(x + 5, y + 14, "                                  Enjoy the challenge!", 14, 0);
	m.unlock();
}

void printControlGuide(int x, int y)
{
	structionBoard(x, y, "BUTTON FUNCTIONS", 10, 47);
	clearScreen(x + 5, y + 5, 10, 68);
	m.lock();
	SetColor(0, 11);
	gotoxy(x + 15, y + 6);
	cout << "[" << char(30) << "],[W] : ";
	gotoxy(x + 15, y + 8);
	cout << "[" << char(31) << "],[S] : ";
	gotoxy(x + 15, y + 10);
	cout << "[" << char(16) << "],[D] : ";
	gotoxy(x + 15, y + 12);
	cout << "[" << char(17) << "],[A] : ";
	gotoxy(x + 45, y + 6);
	cout << "[U] : ";
	gotoxy(x + 45, y + 8);
	cout << "[R] : ";
	gotoxy(x + 45, y + 10);
	cout << "[E] : ";
	gotoxy(x + 45, y + 12);
	cout << "[ENTER] : ";
	SetColor(0, 14);
	gotoxy(x + 25, y + 6);
	cout << "MOVE UP";
	gotoxy(x + 25, y + 8);
	cout << "MOVE DOWN";
	gotoxy(x + 25, y + 10);
	cout << "MOVE RIGHT";
	gotoxy(x + 25, y + 12);
	cout << "MOVE LEFT";
	gotoxy(x + 51, y + 6);
	cout << "UNDO";
	gotoxy(x + 51, y + 8);
	cout << "REDO";
	gotoxy(x + 51, y + 10);
	cout << "EXIT GAME";
	gotoxy(x + 55, y + 12);
	cout << "CONFIRM";
	m.unlock();
}

void GuideScene(Scene& scene)		// Giao diện hướng dẫn chơi 
{
	SetColor(0, 0);
	system("cls");
	makeFrames();
	warning();
	bool run = true;
	string title = "GUIDE";
	thread thr(printTitle,55, 4, title, ref(run), 11, 14);
	int counter = 0;
	char key;
	int x = 45, y = 12;
	while (start)
	{
		printButton(x, y, " BACK MENU ", false, 11, 14);
		printButton(x + 20, y, " GAME RULES ", false, 11, 14);
		printButton(x + 40, y, " BUTTON FUNCTIONS ", false, 11, 14);
		if (counter == 0)
		{
			printButton(x, y, " BACK MENU ", true, 11, 14);
			clearScreen(x - 8, y + 5, 20, 80);
		}
		if (counter == 1)
		{
			printButton(x + 20, y, " GAME RULES ", true, 11, 14);
			/*clearScreen(x - 8, y + 5, 20, 80);*/
			printGameRules(x - 8, y + 5);
		}
		if (counter == 2)
		{
			printButton(x + 40, y, " BUTTON FUNCTIONS ", true, 11, 14);
			/*clearScreen(x - 8, y + 5, 20, 80);*/
			printControlGuide(x - 8, y + 5);
		}
		key = _getch();
		if (key == Enter && counter == 0)
		{
			scene = menu;
			break;
		}
		if (key == prefix_value)
			continue;
		if (key == Right)
		{
			counter++;
			if (counter == 3)
				counter = 0;
		}
		else if (key == Left)
		{
			counter--;
			if (counter == -1)
				counter = 2;
		}
	}
	run = false;
	if (thr.joinable())
		thr.join();
}

