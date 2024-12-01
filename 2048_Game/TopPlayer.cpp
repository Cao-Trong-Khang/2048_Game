#include "TopPlayer.h"

void printCrown(int x, int y)
{ 
	m.lock();
	SetColor(14, 0);
	gotoxy(x + 5, y);
	cout << "  ";
	gotoxy(x, y + 1);
	cout << " ";
	gotoxy(x + 4, y + 1);
	cout << "    ";
	gotoxy(x + 11, y + 1);
	cout << " ";
	gotoxy(x + 1, y + 2);
	cout << " ";
	gotoxy(x + 3, y + 2);
	cout << "  ";
	gotoxy(x + 7, y + 2);
	cout << "  ";
	gotoxy(x + 10, y + 2);
	cout << " ";
	gotoxy(x, y + 3);
	cout << "            ";
	SetColor(11, 14);
	gotoxy(x, y + 2);
	cout << " ";
	gotoxy(x + 5, y + 2);
	cout << "  ";
	gotoxy(x + 11, y + 2);
	cout << " ";
	m.unlock();
}

// Hàm giải phóng bộ nhớ 
void Top20List::free()
{
	node* del = NULL;
	while (top1 != NULL)
	{
		del = top1;
		top1 = top1->next;
		delete del;
	}
	top1 = NULL;
	del = NULL;
	nUser = 0;
}

node* makenode(User user)
{
	node* nod = new node;
	nod->next = NULL;
	nod->user = user;
	return nod;
}

// Thêm một user mà vẫn giữ nguyên tính sắp xếp và số lượng user vẫn đảm bảo <= 20 (hoặc cập nhật thông tin user cùng tên)
void Top20List::addUser(User user)
{
	node* nod = makenode(user), *tmp, *del;
	if (top1 == NULL)
	{
		top1 = nod;
		nUser++;
		return;
	}
	if (top1->user.score < user.score || (top1->user.score == user.score && timetoint(top1->user.time) > timetoint(user.time)))
	{
		nod->next = top1;
		top1 = nod;
	}
	else
	{
		tmp = top1;
		while (tmp->next != NULL)
		{
			if (tmp->next->user.score < user.score || (tmp->next->user.score == user.score && timetoint(tmp->next->user.time) > timetoint(user.time)))
			{
				nod->next = tmp->next;
				tmp->next = nod;
				break;
			}
			tmp = tmp->next;
		}
		if (tmp->next == NULL)
			tmp->next = nod;
	}
	nUser++;
	tmp = top1;
	while (tmp != NULL && tmp->user.name != user.name)
		tmp = tmp->next;
	while (tmp != NULL && tmp->next != NULL && tmp->next->user.name != user.name)
		tmp = tmp->next;
	if (tmp != NULL && tmp->next != NULL)
	{
		del = tmp->next;
		tmp->next = del->next;
		delete del;
		nUser--;
	}
	if (nUser > 20)
	{
		tmp = top1;
		while (tmp != NULL && tmp->next != NULL && tmp->next->next != NULL)
			tmp = tmp->next;
		if(tmp != NULL && tmp->next != NULL)
		{
			del = tmp->next;
			tmp->next = del->next;
			delete del;
			nUser--;
		}
	}
}

// In Top 20
void printTop20(int x, int y, Top20List list)
{
	int i = 0;
	node* cur = list.top1;
	m.lock();
	SetColor(0, 11);
	gotoxy(x - 1, y - 1);
	cout << "RANK";
	gotoxy(x + 9, y - 1);
	cout << "NAME";
	gotoxy(x + 24, y - 1);
	cout << "TIME";
	gotoxy(x + 35, y - 1);
	cout << "SCORE";
	gotoxy(x + 59, y - 1);
	cout << "PRESS [";
	SetColor(0, 14);
	cout << "ENTER";
	SetColor(0, 11);
	cout << "] TO BACK MENU";
	while (cur != NULL)
	{
		if (cur == list.top1)
			SetColor(0, 14);
		else
			SetColor(0, 15);
		gotoxy(x, y + i + 1);
		cout << i + 1;
		gotoxy(x + 4, y + i + 1);
		cout << cur->user.name;
		gotoxy(x + 22, y + i + 1);
		if (cur->user.time.hour < 10)
			cout << 0 << cur->user.time.hour;
		else
			cout << cur->user.time.hour;
		cout << ":";
		if (cur->user.time.minute < 10)
			cout << 0 << cur->user.time.minute;
		else
			cout << cur->user.time.minute;
		cout << ":";
		if (cur->user.time.second < 10)
			cout << 0 << cur->user.time.second;
		else
			cout << cur->user.time.second;
		gotoxy(x + 40 - Count(cur->user.score), y + i + 1);
		cout << cur->user.score;
		cur = cur->next;
		i++;
	}
	m.unlock();
}

// Ghi file topplayer.bin
void writeTopPlayerList(Top20List& list)
{
	fstream file;
	file.open("topplayer.bin", ios::out | ios::binary | ios::trunc);
	file.write((char*)&list.nUser, sizeof(int));
	while (list.top1 != NULL)
	{
		file.write((char*)list.top1->user.name.c_str(), sizeof(string));
		file.write((char*)&list.top1->user.score, sizeof(int));
		file.write((char*)&list.top1->user.time.hour, sizeof(Time));
		node* del = list.top1;
		list.top1 = del->next;
		delete del;
	}
	list.nUser = 0;
	file.close();
}

// Đọc file topplayer.bin - top 20 player cao điểm nhất
void readTopPlayerList(Top20List& list)
{
	int nUser = 0;
	User user;
	fstream file;
	file.open("topplayer.bin", ios::in | ios::binary);
	if (!file.is_open())
	{
		ofstream newFile("topplayer.bin", ios::out | ios::binary);		// Khởi tạo file cho lần đầu tiên chạy chương trình 
		newFile.write((char*)&nUser, sizeof(nUser));
		newFile.close();
	}
	else
	{
		file.read((char*)&nUser, sizeof(nUser));
		for (int i = 0; i < nUser; i++)
		{
			file.read((char*)user.name.c_str(), sizeof(string));
			file.read((char*)&user.score, sizeof(unsigned int));
			file.read((char*)&user.time.hour, sizeof(Time));
			list.addUser(user);
		}
		file.close();
	}
}

void TopPlayerScene(Scene& scene)
{
	string Title = "TOP PLAYERS";
	Top20List list;
	SetColor(0, 15);
	system("cls");
	warning();
	makeFrames();
	printCrown(68, 10); 
	bool run = true;
	thread thr(printTitle, 28, 2, Title, ref(run), 11, 14);
	structionBoard(52, 15, "TOP 20 LIST", 15, 20);
	readTopPlayerList(list);
	printTop20(55, 18, list);
	Key button;
	while (start)
	{
		button = (Key)_getch();
		if (button == Enter)
		{
			scene = menu;
			run = false;
			if (thr.joinable())
				thr.join();
			break;
		}
	}
	list.free();
}


