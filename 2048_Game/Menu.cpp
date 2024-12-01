#include "Menu.h"

void MenuScene(Scene &scene)
{
	SetColor(0, 15);
	system("cls");
	makeFrames();

	char key;
	int counter = 0, x = 67, y = 27;
	bool checkresume = checkResume(), run = true;

	thread thr(makeTitle2048, x - 34, y - 23, ref(run));
	while (start)
	{
		printButton(x, y,     "  NEW GAME   ", 0, 11, 14);
		printButton(x, y + 2, " TOP PLAYERS ", 0, 11, 14);
		printButton(x, y + 4, "    GUIDE    ", 0, 11, 14);
		printButton(x, y + 6, "   SETTING   ", 0, 11, 14);
		printButton(x, y + 8, "  MINI GAME  ", 0, 11, 14);
		if (checkresume)
			printButton(x, y + 10, "   RESUME    ", 0, 11, 14);
		else
			printButton(x, y + 10, "   RESUME    ", 0, 8, 8);
		if (counter == 0)
			printButton(x, y, "  NEW GAME   ", 1, 11, 14);
		else if (counter == 1)
			printButton(x, y + 2, " TOP PLAYERS ", 1, 11, 14);
		else if (counter == 2)
			printButton(x, y + 4, "    GUIDE    ", 1, 11, 14);
		else if (counter == 3)
			printButton(x, y + 6, "   SETTING   ", 1, 11, 14);
		else if(counter == 4)
			printButton(x, y + 8, "  MINI GAME  ", 1, 11, 14);
		else 
			printButton(x, y + 10, "   RESUME    ", 1, 11, 14);
		key = _getch();
		if (key == Enter)  
		{
			run = false;
			if (counter == 0)
			{
				scene = newgame;   
				break;
			}
			else if (counter == 1)
			{
				scene = topplayer;   
				break;
			}
			else if (counter == 2)
			{
				scene = guide;   
				break;
			}
			else if (counter == 3)
			{
				scene = setting;   
				break;
			}
			else if (counter == 4)
			{
				scene = minigame;  
				break;
			}
			else
			{
				scene = resume;    
				break;
			}
		}
		if (key == -32)
			key = _getch();

		if (key == Up)  
		{
			counter--;
			if (counter == 5 && !checkresume)
				counter--;
		}
		else if (key == Down) 
		{
			counter++;
			if (counter == 5 && !checkresume)
				counter++;
		}
		if (counter == -1)
		{
			if (checkresume)
				counter = 5;
			else
				counter = 4;
		}
		else
			counter = counter % 6;
	}
	if (thr.joinable())
		thr.join();
}

// Hàm kiểm tra chức năng resume có sử dụng được hay không 
bool checkResume()
{
	int check, empty = 0;
	string fileName[6] = { "Account_1.bin", "Account_2.bin" , "Account_3.bin" , "Account_4.bin" , "Account_5.bin" , "Account_6.bin" };
	fstream file;
	for (int i = 0; i < 6; i++)
	{
		file.open(fileName[i], ios::in | ios::binary);
		if (!file.is_open())
		{
			ofstream newFile(fileName[i], ios::out | ios::binary);		// Khởi tạo file cho lần đầu tiên chạy chương trình 
			newFile.write((char*)&empty, sizeof(int));
			newFile.close();
			check = 0;
		}
		else
		{
			file.read((char*)&check, sizeof(check));
			file.close();
		}
		if (check == 1)
			return true;
	}
	return false;  // Tất cả file đều rỗng thì return false;
}




