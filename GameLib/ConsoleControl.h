#pragma once

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_SPACE 32
#define KB_ESCAPE 27

enum ConsoleColor {
	BLACK, DARKBLUE, DARKGREEN, DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW, LIGHTGREY,
	DARKGREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
};

void ConsoleSetColor(ConsoleColor ForeColor, ConsoleColor BackColor);
void ConsoleXY(short int x, short int y);
void ConsoleClear();
void ConsoleClearCharacter(char character, ConsoleColor ForeColor, ConsoleColor BackColor);

void ConsoleWait(unsigned long milisecons);

int ConsoleInKey();