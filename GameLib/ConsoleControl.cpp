#include "ConsoleControl.h"

#include <conio.h>
#include <windows.h>

void ConsoleSetColor(ConsoleColor ForeColor, ConsoleColor BackColor) {
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD color = (BackColor << 4) | ForeColor;
	SetConsoleTextAttribute(hstdout, color);
}

void ConsoleXY(short int x, short int y) {
	COORD pos = { x, y };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(console, pos);
}

void ConsoleClear() {
	ConsoleClearCharacter(' ', WHITE, BLACK);
}

void ConsoleClearCharacter(char character, ConsoleColor ForeColor, ConsoleColor BackColor) {
	COORD topLeft = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD color = (BackColor << 4) | ForeColor;
	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, character, screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, color,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void ConsoleWait(DWORD milisecons) {
	Sleep(milisecons);
}

int ConsoleInKey() {
	int KB_code = 0;

	if (_kbhit())
	{
		KB_code = _getch();
	}
	return KB_code;
}