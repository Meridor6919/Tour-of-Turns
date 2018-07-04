#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <thread>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma warning(disable:4996)

void Choose(std::string text[], COORD coord[], int HowMany, int &pos, int color, int color2);
bool HorizontalChoose(std::string text[], COORD coord, int HowMany, int &pos, int color, int color2);
void Sleeping(int Time);
void Title(int width, int color, int color2);
std::string NewRaceStart(std::string ext, std::string text, std::string file, short width, int color, int color2);
float factorial(float number);
float Power(float number, float ToThePowerOf);
void infobox(std::string text1, std::string text2, int color, int color2);
template<typename T>
std::string NumberToString(T a)
{
	std::stringstream b;
	b << a;
	return b.str();
}
inline void Spaces(int i)
{
	for (int i2 = 0; i2 < i; i2++)
	{
		std::cout << " ";
	}
}
