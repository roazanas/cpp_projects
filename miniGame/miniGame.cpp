#include <iostream>
#include <cmath>
#include <windows.h>
#include <string>
using namespace std;

int CHANCE = 25;

void colorPrint(int color, string text = "") {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	cout << text;
	if (text != "") {
		colorPrint(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
}

class Table {
private:
	static const int HEIGHT = 10, WIDTH = 10;
	int p_table[HEIGHT][WIDTH];
	int chr_i, chr_j;
	int coinsCounter;
	int xtoj(int x) {return x-1;}
	int jtox(int j) {return j+1;}
	const int ytoi(int y) {return HEIGHT-y;}
	const int itoy(int i) {return HEIGHT-i;}

	bool cellisAvailable(int i, int j) { return (p_table[i][j] == 0); }

	bool generateItem(bool isCoin = true) {
		int attempts = 0;
		int ri = rand() % HEIGHT;
		int rj = rand() % WIDTH;
		while (!cellisAvailable(ri, rj)) {
			if (attempts > (HEIGHT * WIDTH)) return false;
			ri = rand() % HEIGHT;
			rj = rand() % WIDTH;
			attempts++;
		}
		if (isCoin) {
			p_table[ri][rj] = rand() % 9 + 1;
		}
		else {
			p_table[ri][rj] = -2;
		}
		return true;
	}
public:
	Table(int x=WIDTH/2, int y=HEIGHT/2) {
		coinsCounter = 0;
		chr_i = ytoi(y);
		chr_j = xtoj(x);
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if (i == chr_i && j == chr_j) {
					p_table[i][j] = -1;
				} 
				else {
					p_table[i][j] = 0;
				}
			}
		}
		generateItem(true);
		showTable();
	}

	bool moveChrTo(int x, int y) {
		if ((1 <= x && x <= WIDTH) && (1 <= y && y <= HEIGHT)) {
			int j = xtoj(x);
			int i = ytoi(y);
			p_table[chr_i][chr_j] = 0;
			if (p_table[i][j] > 0) {
				coinsCounter += p_table[i][j];
				generateItem(true);
				if (rand() % (100/CHANCE) == 0) generateItem(true);
				if (CHANCE != 100) {
					for (int k = 1; k <= p_table[i][j]; k++) {
						if (rand() % (100 / (100 - CHANCE)) == 0) generateItem(false);
					}
				}
			}
			else if (p_table[i][j] == -2) {
				chr_i = i;
				chr_j = j;
				system("cls");
				showTable();
				colorPrint(FOREGROUND_RED, "GAME OVER!\n");
				system("pause");
				exit(0);
			}
			p_table[i][j] = -1;
			chr_i = i;
			chr_j = j;
			return true;
		}
		return false;
	}

	bool moveChrTo(char side) {
		int x = getX(), y = getY();
		switch (side) {
			case 'w': case 'ц': y++; break;
			case 'a': case 'ф': x--; break;
			case 's': case 'ы': y--; break;
			case 'd': case 'в': x++; break;
			default: break;
		}
		return moveChrTo(x, y);
	}

	const void showTable() {
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				switch (p_table[i][j]) {
					case  0: cout << '.'; break;
					case -1: colorPrint(FOREGROUND_GREEN, "o"); break;
					case -2: colorPrint(FOREGROUND_RED, "x"); break;
					default: 
						colorPrint(FOREGROUND_RED | FOREGROUND_GREEN, to_string(p_table[i][j]));
						break;
				}
				cout << ' ';
			}
			cout << endl;
		}
		cout << "x = "; colorPrint(FOREGROUND_BLUE, to_string(getX())); cout << " | ";
		cout << "y = "; colorPrint(FOREGROUND_BLUE, to_string(getY())); cout << endl;
		cout << "Количество монет: ";
		colorPrint(FOREGROUND_RED | FOREGROUND_GREEN, to_string(coinsCounter));
		cout << endl;
	}

	int getX() { return jtox(chr_j); }
	int getY() { return itoy(chr_i); }
	void setX(int x) { chr_j = xtoj(x); }
	void setY(int y) { chr_i = ytoi(y); }
	int getH() { return HEIGHT; }
	int getW() { return WIDTH; }
};

void updateScreen(Table& tbl) {
	system("cls");
	tbl.showTable();
}

void greetings() {
	system("chcp 1251 && cls");
	cout << "Выберите сложность (в процентах) < 100, 75% - нормальная: "; cin >> CHANCE; CHANCE = 100-(CHANCE%100);
	cout << "Правила: \n";
	cout << "• Это персонаж: "; colorPrint(FOREGROUND_GREEN, "o\n\n");
	cout << "• Перемещение осуществляется при помощи ввода символов ";
		colorPrint(FOREGROUND_BLUE, "WASD / ЦФЫВ");
		cout << " (можно последовательностью) и кнопки "; colorPrint(FOREGROUND_BLUE, "Enter\n\n");
	cout << "• Цель - собрать как можно больше "; colorPrint(FOREGROUND_RED | FOREGROUND_GREEN, "монет\n\n");
	cout << "• Каждая собранная "; colorPrint(FOREGROUND_RED | FOREGROUND_GREEN, "монета");
		cout << " = "<<CHANCE<<"% шанс появления дополнительной "; 
		colorPrint(FOREGROUND_RED | FOREGROUND_GREEN, "монеты");
		cout << " и "<<100-CHANCE<<"% шанс появления каждого "; colorPrint(FOREGROUND_RED, "крестика");
		cout << " из того кол-ва, каким номиналом была "; colorPrint(FOREGROUND_RED | FOREGROUND_GREEN, "монета\n\n");
	colorPrint(FOREGROUND_RED, "\nИЗБЕГАЙТЕ КРЕСТИКОВ!\n");
	system("pause && cls");
}

int main() {
	srand(time(NULL));
	greetings();
	Table table(1, 1);
	char side;
	for (int i = 0; cin >> side; i++) {
		table.moveChrTo(side);
		updateScreen(table);
	}
	return 0;
}
