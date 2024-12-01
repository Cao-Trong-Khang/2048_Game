#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <thread>
#include <mutex>
#define start true

using namespace std;

struct cell							// Định nghĩa kiểu dữ liệu ô trong ma trận 
{
	int x, y;
};

struct Time
{
	int hour = 0;
	int minute = 0;
	int second = 0;
};

struct User		// Thông tin người chơi 
{
	string name = "";
	unsigned int score = 0;
	Time time;
};

enum Scene
{
	newgame = 0,
	topplayer = 1,
	guide = 2,
	setting = 3,
	menu = 4,
	minigame = 5,
	resume = 6
};

enum Key
{
	Up = 72,
	Down = 80,
	Left = 75,
	Right = 77,
	W = 'w',
	S = 's',
	A = 'a',
	D = 'd',
	Exit = 'e',
	Undo = 'u',
	Redo = 'r',
	Enter = '\r',
	prefix_value = -32
};

int timetoint(Time time);

void SettingScreen();		// Một số định dạng màn hình console 

void gotoxy(int x, int y);  // Di chuyển con trỏ trên màng hình console đến tọa độ x y 
 
void printButton(int x, int y, string text, bool highlight, int baseColor, int highColor);   // In nút

void printNotice(int x, int y, string Text);	// Hàm in bản thông báo 

void makeFrames();  // Vẽ khung hình 

void clearScreen(int x, int y, int h, int w);   // Xóa màng hình ở tọa độ (x,y) chiều cao bằng h, dài bằng w 

void printText(int x, int y, string text, int colorTextCode, int colorBackGroundCode); // In văn bản lên màng hình 

void printFrameMatrix(int x, int y, int n);   // In khung hình cho ma trận cấp n tại vị trí x, y

int Count(int n);			// Hàm đếm chữ số 

int br_colorCode(int n);  // Hàm tìm mã màu cho background của 1 số 

void printGuide(int x, int y);

int selectAccount(int xo, int yo, int mode, Scene& scene, bool lock);		// Chọn account để chơi tiếp hoặc để xóa và lưu tài khoản mới, trả về index tài khoản được chọn - lock : khóa tính năng trở về menu và xóa 

bool Question(int x, int y, string text);	// Hàm hỏi người chơi để thực hiện chức năng gì đó true - đồng ý, false - không đồng ý  

void printAccount(int xo, int yo, bool highlight, int accIndex, User user, int mode);		// In Account ra màn hình để người dùng chọn

void ShowCur(bool CursorVisibility);  // Ẩn hiện con trỏ - Nguồn codelearn 

void SetColor(int backgound_color, int text_color);  // Hàm chuyển màu - Nguồn codelearn 

void DisableSelection();	// Hàm vô hiệu hóa bôi đen màn hình console - Nguồn codelearn 

void SetScreenBufferSize(SHORT width, SHORT height);  // Hàm chỉnh kích thước màn hình console - Nguồn codelearn 

// Hàm lấy tọa độ con trỏ hiện tại - Nguồn ChatGPT 
void getCursorPosition(int& x, int& y);

void printMatrix(int** matrix, int n, int x, int y);

void printLetter(int x, int y, char c, int textColor, int brColor);    // Hàm in kí tự kiểu To có màu

void printTitle(int x, int y, string title, bool& run, int color1, int color2); 

void printNumber(int x, int y, int n); // Hàm in số 

// In khung đồng hồ 
void clockFrame(int x, int y);

void Clock(int x, int y, Time& time, bool& run, bool& pause);  // Hàm tính thời gian người chơi 

void structionBoard(int x, int y, string title, int h, int w);	// Hàm in bảng hướng dẫn 

void makeTitle2048(int x, int y, bool& run);

void warning();  