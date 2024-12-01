#include "Resume.h"
#include "NewGame.h"

void printGuideResume()
{
	m.lock();
	SetColor(0, 11);
	gotoxy(113, 21);
	cout << "B";
	SetColor(0, 14);
	cout << " : Back Menu";
	SetColor(0, 11);
	gotoxy(113, 23);
	cout << "C";
	SetColor(0, 14);
	cout << " : Change Mode ";
	SetColor(0, 11);
	cout << "PLAY ";
	SetColor(0, 14);
	cout << char(29);
	SetColor(0, 12);
	cout << " DELETE";
	SetColor(0, 11);
	gotoxy(113, 25);
	cout << "Enter";
	SetColor(0, 14);
	cout << " : Choose Account";
	m.unlock();
}

void ResumeScene(Scene& scene)
{
	int accIndex;
	SetColor(0, 15);
	system("cls");
	makeFrames();
	warning();
	bool run = true;
	string title = "RESUME";
	thread thr(printTitle, 47, 5, title, ref(run), 11, 14);
	structionBoard(110, 18, "BUTTON FUNCTIONS", 1, 4);
	printGuideResume();
	accIndex = selectAccount(7, 15, 1, scene, false);
	run = false;
	if(thr.joinable())
		thr.join();
	if(scene == menu)
		return;
	NewGameScene(scene, accIndex);
}
