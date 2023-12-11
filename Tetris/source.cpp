#include "header.h"
#include "board.h"

//https://cachhoc.net/2013/08/22/cc-gotoxy-trong-dev-c-gotoxy-in-dev-c/
void gotoxy(int x, int y) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void gotoxyPiece(Coordinates co) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { co.x, co.y };
	SetConsoleCursorPosition(h, c);
}

Coordinates::Coordinates(int _x, int _y) {
	x = _x, y = _y;
}


Piece::~Piece() {
	delete[] c;
}
Coordinates* Piece::getCMove() {
	Coordinates* result = new Coordinates[4];
	for (int i = 0; i < 4; ++i) {
		result[i] = c[i];
	}
	return result;
}
bool Piece::BottomCheck(const Board& b) {
	for (int i = 0; i < 4; ++i) {
		if (b.getG(c[i].x, c[i].y + 1) != 0) {
			Sleep(1000);
			return true;
		}
	}
	return false;
}
void Piece::RotateRight(){
	state++;
	state %= 4;
}
void Piece::RotateLeft() {
	state += 3;
	state %= 4;
}
void Piece::MoveRight(const Board& b) {
	for (int i = 0; i < 4; ++i) {
		if (b.getG(c[i].x + 2, c[i].y) != 0)
			return;
	}
	this->UnShow();
	for (int i = 0; i < 4; ++i) {
		c[i].x += 2;
	}
}
void Piece::MoveLeft(const Board& b) {
	for (int i = 0; i < 4; ++i) {
		if (b.getG(c[i].x - 2, c[i].y) != 0)
			return;
	}
	this->UnShow();
	for (int i = 0; i < 4; ++i) {
		c[i].x -= 2;
	}
}
void Piece::MoveDownTime(const Board& b, time_t& originalTime){
	time_t nowTime = time(0);
	double timeLeft = difftime(nowTime, originalTime);
	if (timeLeft <= 1.0) {
		originalTime = time(0);
		for (int i = 0; i < 4; ++i) {
			if (b.getG(c[i].x, c[i].y + 1) != 0)
				return;
		}
		this->UnShow();
		for (int i = 0; i < 4; ++i) {
			c[i].y++;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
}
void Piece::UnShow() {
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top)
			cout << "  ";
	}
}


PieceI::PieceI() {
	c = new Coordinates[4];
	color = 11; //Bright cyan
	state = 0;
	c[0] = { 11, top + 1 };
	c[1] = { 11, top + 2 };
	c[2] = { 11, top + 3 };
	c[3] = { 11, top + 4 };
}
bool PieceI::RotateCheck(const Board& b, int rot){
	int s = state + rot;
	s %= 4;
	if (s % 2 == 0) {
		for (int i = 0; i < 4; ++i) {
			if (b.getG(c[0].x, c[0].y - i) != 0)
				return false;
		}
	}
	else {
		for (int i = 0; i < 4; ++i) {
			if (b.getG(c[0].x - 2 + 2 * i, c[0].y) != 0)
				return false;
		}
	}
	return true;
}
void PieceI::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	if (state % 2 == 0) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x, cMove.y - 2 };
		c[3] = { cMove.x, cMove.y - 3 };
	}
	else{
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x - 2, cMove.y};
		c[2] = { cMove.x + 2, cMove.y };
		c[3] = { cMove.x + 4, cMove.y };
	}
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


PieceO::PieceO() {
	c = new Coordinates[4];
	color = 14; //Yellow
	state = 0;
	c[0] = { 9, top + 1 };
	c[1] = { 11, top + 1 };
	c[2] = { 9, top };
	c[3] = { 11, top };
}
bool PieceO::RotateCheck(const Board& b, int rot) {
	return true;
}
void PieceO::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	c[0] = { cMove.x, cMove.y };
	c[1] = { cMove.x + 2, cMove.y };
	c[2] = { cMove.x, cMove.y - 1 };
	c[3] = { cMove.x + 2, cMove.y - 1 };
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


PieceJ::PieceJ() {
	c = new Coordinates[4];
	color = 1; //Blue
	state = 0;
	c[0] = { 9, top + 1 };
	c[1] = { 11, top + 1 };
	c[2] = { 11, top };
	c[3] = { 11, top - 1 };
}
bool PieceJ::RotateCheck(const Board& b, int rot) {
	int s = state + rot;
	s %= 4;
	if (s == 0) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x + 2, c[0].y) != 0 || b.getG(c[0].x + 2, c[0].y - 1) != 0 || b.getG(c[0].x + 2, c[0].y - 2) != 0)
			return false;
	}
	else if (s == 1) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x + 2, c[0].y) != 0 || b.getG(c[0].x + 4, c[0].y)     != 0 || b.getG(c[0].x, c[0].y - 1) != 0)
			return false;
	}
	else if (s == 2) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0 || b.getG(c[0].x, c[0].y - 2)     != 0 || b.getG(c[0].x + 2, c[0].y - 2) != 0)
			return false;
	}
	else if (s == 3) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0 || b.getG(c[0].x - 2, c[0].y - 1) != 0 || b.getG(c[0].x - 4, c[0].y - 1) != 0)
			return false;
	}
	return true;
}
void PieceJ::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	if (state == 0) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x + 2, cMove.y };
		c[2] = { cMove.x + 2, cMove.y - 1 };
		c[3] = { cMove.x + 2, cMove.y - 2 };
	}
	else if (state == 1) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x + 2, cMove.y };
		c[2] = { cMove.x + 4, cMove.y };
		c[3] = { cMove.x, cMove.y - 1 };
	}
	else if (state == 2) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x, cMove.y - 2 };
		c[3] = { cMove.x + 2, cMove.y - 2 };
	}
	else if (state == 3) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x - 2, cMove.y - 1 };
		c[3] = { cMove.x - 4, cMove.y - 1 };
	}
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


PieceL::PieceL() {
	c = new Coordinates[4];
	color = 12; //Bright red
	state = 0;
	c[0] = { 11, top + 1 };
	c[1] = { 9, top + 1 };
	c[2] = { 9, top };
	c[3] = { 9, top - 1 };
}
bool PieceL::RotateCheck(const Board& b, int rot) {
	int s = state + rot;
	s %= 4;
	if (s == 0) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x - 2, c[0].y) != 0 || b.getG(c[0].x - 2, c[0].y - 1) != 0 || b.getG(c[0].x - 2, c[0].y - 2) != 0)
			return false;
	}
	else if (s == 1) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0 || b.getG(c[0].x + 2, c[0].y - 1) != 0 || b.getG(c[0].x + 4, c[0].y - 1) != 0)
			return false;
	}
	else if (s == 2) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0 || b.getG(c[0].x, c[0].y - 2)     != 0 || b.getG(c[0].x - 2, c[0].y - 2) != 0)
			return false;
	}
	else if (s == 3) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x - 2, c[0].y) != 0 || b.getG(c[0].x - 4, c[0].y)     != 0 || b.getG(c[0].x, c[0].y - 1) != 0)
			return false;
	}
	return true;
}
void PieceL::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	if (state == 0) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x - 2, cMove.y };
		c[2] = { cMove.x - 2, cMove.y - 1 };
		c[3] = { cMove.x - 2, cMove.y - 2 };
	}
	else if (state == 1) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x + 2, cMove.y - 1 };
		c[3] = { cMove.x + 4, cMove.y - 1 };
	}
	else if (state == 2) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x, cMove.y - 2 };
		c[3] = { cMove.x - 2, cMove.y - 2 };
	}
	else if (state == 3) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x - 2, cMove.y };
		c[2] = { cMove.x - 4, cMove.y };
		c[3] = { cMove.x, cMove.y - 1 };
	}
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


PieceT::PieceT() {
	c = new Coordinates[4];
	color = 5; //Purple
	state = 0;
	c[0] = { 11, top + 1 };
	c[1] = { 9, top + 1 };
	c[2] = { 13, top + 1 };
	c[3] = { 11, top };
}
bool PieceT::RotateCheck(const Board& b, int rot) {
	int s = state + rot;
	s %= 4;
	if (s == 0) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x - 2, c[0].y) != 0     || b.getG(c[0].x + 2, c[0].y)     != 0 || b.getG(c[0].x, c[0].y - 1) != 0)
			return false;
	}
	else if (s == 1) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0     || b.getG(c[0].x, c[0].y - 2)     != 0 || b.getG(c[0].x + 2, c[0].y - 1) != 0)
			return false;
	}
	else if (s == 2) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x - 2, c[0].y - 1) != 0 || b.getG(c[0].x + 2, c[0].y - 1) != 0 || b.getG(c[0].x, c[0].y - 1) != 0)
			return false;
	}
	else if (s == 3) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0     || b.getG(c[0].x, c[0].y - 2)     != 0 || b.getG(c[0].x - 2, c[0].y - 1) != 0)
			return false;
	}
	return true;
}
void PieceT::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	if (state == 0) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x - 2, cMove.y };
		c[2] = { cMove.x + 2, cMove.y };
		c[3] = { cMove.x, cMove.y - 1 };
	}
	else if (state == 1) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x, cMove.y - 2 };
		c[3] = { cMove.x + 2, cMove.y - 1 };
	}
	else if (state == 2) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x - 2, cMove.y - 1 };
		c[2] = { cMove.x + 2, cMove.y - 1 };
		c[3] = { cMove.x, cMove.y - 1 };
	}
	else if (state == 3) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x, cMove.y - 2 };
		c[3] = { cMove.x - 2, cMove.y - 1 };
	}
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


PieceZ::PieceZ() {
	c = new Coordinates[4];
	color = 4; //Red
	state = 0;
	c[0] = { 11, top + 1 };
	c[1] = { 9, top + 1 };
	c[2] = { 11, top };
	c[3] = { 13, top };
}
bool PieceZ::RotateCheck(const Board& b, int rot) {
	int s = state + rot;
	s %= 4;
	if (s % 2 == 0) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x - 2, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0     || b.getG(c[0].x + 2, c[0].y - 1) != 0)
			return false;
	}
	else if (s % 2 == 1) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0 || b.getG(c[0].x - 2, c[0].y - 1) != 0 || b.getG(c[0].x - 2, c[0].y - 2) != 0)
			return false;
	}
	return true;
}
void PieceZ::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	if (state % 2 == 0) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x - 2, cMove.y };
		c[2] = { cMove.x, cMove.y - 1 };
		c[3] = { cMove.x + 2, cMove.y - 1 };
	}
	else if (state % 2 == 1) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x - 2, cMove.y - 1 };
		c[3] = { cMove.x - 2, cMove.y - 2 };
	}
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


PieceS::PieceS() {
	c = new Coordinates[4];
	color = 10; //Bright green
	state = 0;
	c[0] = { 9, top + 1 };
	c[1] = { 11, top + 1 };
	c[2] = { 9, top };
	c[3] = { 7, top };
}
bool PieceS::RotateCheck(const Board& b, int rot) {
	int s = state + rot;
	s %= 4;
	if (s % 2 == 0) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x + 2, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0     || b.getG(c[0].x - 2, c[0].y - 1) != 0)
			return false;
	}
	else if (s % 2 == 1) {
		if (b.getG(c[0].x, c[0].y) != 0 || b.getG(c[0].x, c[0].y - 1) != 0 || b.getG(c[0].x + 2, c[0].y - 1) != 0 || b.getG(c[0].x + 2, c[0].y - 2) != 0)
			return false;
	}
	return true;
}
void PieceS::Show() {
	UnShow();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	Coordinates cMove = c[0];
	if (state % 2 == 0) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x + 2, cMove.y };
		c[2] = { cMove.x, cMove.y - 1 };
		c[3] = { cMove.x - 2, cMove.y - 1 };
	}
	else if (state % 2 == 1) {
		c[0] = { cMove.x, cMove.y };
		c[1] = { cMove.x, cMove.y - 1 };
		c[2] = { cMove.x + 2, cMove.y - 1 };
		c[3] = { cMove.x + 2, cMove.y - 2 };
	}
	for (int i = 0; i < 4; ++i) {
		gotoxyPiece(c[i]);
		if (c[i].y > top && c[i].y < bottom)
			cout << block;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}


void clearScreen() {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
