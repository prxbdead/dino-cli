#include <iostream>
#include <iomanip>
#include <fstream>
#include <termios.h>
#include <vector>
#include <deque>
#include <linux/input.h>
#include <fcntl.h>
#include <signal.h>
#include "aat.h"

using std::cout, std::setfill, std::setw, std::left, std::right, std::ofstream, std::ifstream, std::system, std::string, std::to_string, std::vector, std::deque;

#define gotoxy(x, y) cout << "\033[" << (x) << ";" << (y) << "H"
#define clear() cout << "\033[0m\033[H\033[J"
#define hideCursor() cout << "\e[?25l"
#define showCursor() cout << "\033[?25h"
#define HEIGHT int(SIZE.ws_row * 3 / 4)
#define WIDTH SIZE.ws_col

#define UP 65 
#define DOWN 66 
#define RIGHT 67 
#define LEFT 68
#define ENTER 10
#define SPACE 32
#define ESC 27
#define BACKSPACE 127

#define SLEEPVAL 12500.00
#define JMPLENGTH 20
#define NUMLEN 3

struct winsize SIZE;

const vector<string> MENUTITLE = {
	"███  ███ █  █  ██     ██   ██  █   █ ███",
	"█  █  █  ██ █ █  █   █    █  █ ██ ██ █",
	"█  █  █  ████ █  █   █ ██ █  █ █ █ █ ██",
	"█  █  █  █ ██ █  █   █  █ ████ █   █ █",
	"███  ███ █  █  ██     ██  █  █ █   █ ███"
};

const vector<string> MENU = {
	"    NEW GAME    ",
	"   SCOREBOARD   ",
	"      EXIT      "
};

const vector<string> ENTERUSERNAME = {
	"PLEASE ENTER A USERNAME",
	"OR PRESS ESC TO GO BACK TO THE MAIN MENU"
};

const vector<string> PLAYAGAIN = {
	"   PLAY AGAIN   ",
	"  BACK TO MENU  ",
	"      EXIT      "
};

const string NEWPB = "NEW PERSONAL BEST";

const vector<string> GAMEOVER = {
	" ██   ██  █   █ ███    ██  █   █ ████ ███ ",
	"█    █  █ ██ ██ █     █  █ █   █ █    █  █",
	"█ ██ █  █ █ █ █ ██    █  █ █   █ ███  ███",
	"█  █ ████ █   █ █     █  █  █ █  █    █  █",
	" ██  █  █ █   █ ███    ██    █   ████ █  █"
};

const vector<vector<string>> DINO = {
	{
		"           ▓▓██▓▓▓▓",
		"          ▓▓▓▓▓▓▓▓",
		"░░      ▒▒██▒▒",
		"▒▒░░  ██████▓▓██",
		"▒▒▓▓▓▓▓▓▓▓▓▓▒▒",
		"   ▓▓████▓▓▓▓",
		"     ▓▓▓▓▓▓",
		"      ▀▀  ██"
	}, {
		"           ▓▓██▓▓▓▓",
		"          ▓▓▓▓▓▓▓▓",
		"░░      ▒▒██▒▒",
		"▒▒░░  ██████▓▓██",
		"▒▒▓▓▓▓▓▓▓▓▓▓▒▒",
		"   ▓▓████▓▓▓▓",
		"     ▓▓▓▓▓▓",
		"      ██  ▀▀"
	},
	{
		"           ▓▓██▓▓▓▓",
		"          ▓▓▓▓▓▓▓▓",
		"░░      ▒▒██▒▒",
		"▒▒░░  ██████▓▓██",
		"▒▒▓▓▓▓▓▓▓▓▓▓▒▒",
		"   ▓▓████▓▓▓▓",
		"     ▓▓▓▓▓▓",
		"      ▀▀  ▀▀",
	},
	{
		"░░        ▒▒▓▓▓██▓▓▓▓",
		"▒▒░░  ██████▓▓▓▓▓▓▓▓",
		"▒▒▓▓▓▓▓▓▓▓▓▓▒▒",
		"   ▓▓████▓▓▓▓",
		"     ▓▓▓▓▓▓",
		"      ▀▀  ██",
	},
	{
		"░░        ▒▒▓▓▓██▓▓▓▓",
		"▒▒░░  ██████▓▓▓▓▓▓▓▓",
		"▒▒▓▓▓▓▓▓▓▓▓▓▒▒",
		"   ▓▓████▓▓▓▓",
		"     ▓▓▓▓▓▓",
		"      ██  ▀▀",
	}
};

const vector<vector<string>> CACTUS = {
	{
		"   ▅██",
		" ▅ ███  █▅",
		"██ ███  ██",
		"▀███████▀▀",
		"   ███",
	},
	{
		"   ██▅",
		" ▅ ███ █▅",
		"██ ███ ██",
		"██ ███▅██",
		"▀█████▀▀",
		"   ███",
	},
	{
		"  ▅▅",
		"  ██ ▅   ▅█",
		"█ ██ █ █ ██ ▅",
		"█ ██ █ █ ██ █",
		"▀███▀  ▀███▅▀",
		"  ██     ██",
	},
	{
		"  ▅█",
		"█ ██ ▅   █▅",
		"█ ██ █ █ ██ ▅",
		"█ ██▅█ █ ██ █",
		"▀███   ▀▀██▅█",
		"  ██     ██",
	},
	{
		"         ▅ ",
		"  █▅     ██ ▅",
		"  ██ ▅ █ ██ █",
		"█ ██ █ █ ██ █",
		"▀▀██▀▀ ▀▀██▀▀",
		"  ██     ██",
	},
	{
		"  ▅     █▅",
		"█ █ ▅ █ ██ █   █ ▅",
		"▀██▅▀ ▀█████ █▅█▅█",
		"  █     ██     █",
	},
	{
		"  ▅     █▅     ▅",
		"▅ █ █ █ ██ █ █ █ ▅",
		"▀▅█▅▀ ▀████  ▀▅█▅▀",
		"  █     ██     █",
	}
};

const vector<vector<string>> BIRD = {
	{
		"     █",
		"  ▅▅  ██",
		"▅█████████",
		"      ▓▓▓▓▓▓▀▀"
	},
	{
		"",
		"  ▅▅",
		"▅█████████",
		"    ███▓▓▓▓▓▀▀",
		"    ██",
		"    █"
	}
};

const vector<vector<string>> NUMBERS = {
	{
		"███",
		"█ █",
		"█ █",
		"█ █",
		"███"
	},
	{
		" █",
		"██",
		" █",
		" █",
		"███"
	}, 
	{
		"██",
		"  █",
		" █",
		"█",
		"███"
	},
	{
		"███",
		"  █",
		" ██",
		"  █",
		"███"
	},
	{
		"█ █",
		"█ █",
		"███",
		"  █",
		"  █"
	},
	{
		"███",
		"█",
		"███",
		"  █",
		"███"
	},
	{
		"███",
		"█",
		"███",
		"█ █",
		"███"
	},
	{
		"███",
		"  █",
		" █",
		"█",
		"█"
	},
	{
		"███",
		"█ █",
		"███",
		"█ █",
		"███"
	},
	{
		"███",
		"█ █",
		"███",
		"  █",
		"███"
	}
};

const vector<vector<string>> NUMBERS2 = {
	{
		"▒▒▒",
		"▒ ▒",
		"▒ ▒",
		"▒ ▒",
		"▒▒▒"
	},
	{
		" ▒",
		"▒▒",
		" ▒",
		" ▒",
		"▒▒▒"
	}, 
	{
		"▒▒",
		"  ▒",
		" ▒",
		"▒",
		"▒▒▒"
	},
	{
		"▒▒▒",
		"  ▒",
		" ▒▒",
		"  ▒",
		"▒▒▒"
	},
	{
		"▒ ▒",
		"▒ ▒",
		"▒▒▒",
		"  ▒",
		"  ▒"
	},
	{
		"▒▒▒",
		"▒",
		"▒▒▒",
		"  ▒",
		"▒▒▒"
	},
	{
		"▒▒▒",
		"▒",
		"▒▒▒",
		"▒ ▒",
		"▒▒▒"
	},
	{
		"▒▒▒",
		"  ▒",
		" ▒",
		"▒",
		"▒"
	},
	{
		"▒▒▒",
		"▒ ▒",
		"▒▒▒",
		"▒ ▒",
		"▒▒▒"
	},
	{
		"▒▒▒",
		"▒ ▒",
		"▒▒▒",
		"  ▒",
		"▒▒▒"
	}
};

const vector<string> HI = {
	"▒ ▒ ▒▒▒",
	"▒ ▒  ▒",
	"▒▒▒  ▒",
	"▒ ▒  ▒",
	"▒ ▒ ▒▒▒"
};

vector<vector<bool>> unsafe;

deque<int> cactuses;
deque<int> birds;

bool alive = true;
bool crouch = false;
int score = 0;
int lastcactus = 0;
int height = 0;
double speed = 1.0;
string username = "";
int scoreBoardIndex = -1;
int highScore = 0;

class ScoreBoardEntry {
	string name;
	int score;

	public:
		ScoreBoardEntry() {
			name = "";
			score = 0;
		}

		ScoreBoardEntry(const string a, const int s) {
			name = a;
			score = s;
		}

		bool operator>(ScoreBoardEntry const &a) const {
			return a.score > score;
		}

		bool operator<(ScoreBoardEntry const &a) const {
			return a.score < score;
		}

		bool operator==(ScoreBoardEntry const &a) const {
			return a.name == name;
		}

		string getName() const {
			return name;
		}

		int getScore() const {
			return score;
		}
};

SortedList<ScoreBoardEntry> scoreboard;

struct entry_t {
	ScoreBoardEntry val;
	entry_t* next;
};

void init();
void cleanup();
void signalHandler(int);
int unicodeLength(const string);
int log10(int);
void drawChar(int, const int, const vector<string> &, bool);
void drawDino();
void drawFloor();
void drawBox();
void clearUnsafe();
void drawBg();
void kbAction();
void jumpHandle();
int startGame();
int insertScoreBoard(const string, const int);
void saveScoreBoard();
void readScoreBoard();
void drawScoreBoard();
bool getUserName();
void menu();

void init() {
	srand(time(NULL));

	// Disable canonical mode and echo input characters
	termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	// Enable non-blocking mode
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

	// Get size of current console window
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &SIZE);
	
	//hide blinking cursor
	hideCursor();

	cactuses.resize(WIDTH, -1);
	birds.resize(WIDTH, -1);
	unsafe.resize(HEIGHT);
	for(auto &it : unsafe) {
		it.resize(WIDTH, 0);
	}
}

void cleanup() {
	clear();

	// Enable canonical mode and echo input characters
	termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);

	// Disable non-blocking mode
	fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & O_NONBLOCK);

	// show blinking cursor
	showCursor();
}

void signalHandler(int signal) {
	cleanup();
	exit(signal);
}

int unicodeLength(const string a) {
	int cnt = 0;
	for(const char &c : a) {
		if((c & 0xC0)!=0x80) {
			++cnt;
		}
	}
	return cnt;
}

int log10(int n) {
	int res = 0;
	
	while(n > 9) {
		n /= 10;
		res++;
	}

	return res;
}

void drawChar(int x, const int y, const vector<string> &c, bool dino = false) {
	for(const string &it : c) {
		gotoxy(x, y);
		int cnt = y;
		int spacecnt = 0;
		for(const char &c : it) {
			if((c & 0xC0)!=0x80) {
				++cnt;
			}

			if(cnt > WIDTH + 1 || c == '\0') {
				break;
			}

			if(c == SPACE) {
				++spacecnt;
			} else {
				if(spacecnt) {
					cout << "\033[" << spacecnt << "C";
					spacecnt = 0;
				}
				cout << c;
				
				if(!dino) {
					unsafe[x][cnt] = true;
				} else if(unsafe[x][cnt]) {
					alive = false;
				}
			}
		}
		x++;
	}
}

void drawDino() {
	if(height > 0) {
		drawChar(HEIGHT - DINO[2].size() - height, WIDTH / 4, DINO[2], true);
		return;
	}

	int anim = crouch * ( crouch + 2 ) + (score % 20 < 10);
	drawChar(HEIGHT - DINO[anim].size(), WIDTH / 4, DINO[anim], true);
}

void drawFloor() {
	gotoxy(HEIGHT, 0);
	for(int i = 0; i < WIDTH; ++i) {
		cout << "▓";
	}
}

void drawBox() {
	gotoxy(0, 0);
	for(int i = 0; i < WIDTH; ++i) {
		cout << "▓";
	}

	for(int i = 0; i < HEIGHT; ++i) {
		gotoxy(i, 0);
		cout << "▓▓";
		gotoxy(i, WIDTH - 1);
		cout << "▓▓";
	}

	gotoxy(HEIGHT, 0);
	for(int i = 0; i < WIDTH; ++i) {
		cout << "▓";
	}
}

void clearUnsafe() {
	for(auto &it : unsafe) {
		fill(it.begin(), it.end(), 0);
	}
}

void drawBg() {
	clear();
	clearUnsafe();
	++score;
	if(score % 100 == 0 && speed < 5.0) {
		speed += 0.01;
	}
	
	cactuses.pop_front();
	birds.pop_front();
	
	if(lastcactus > 50 && rand() % (200 - lastcactus) == 0) {
		if(score < 4000 || rand() % 3 != 0) {
			cactuses.push_back(rand() % CACTUS.size());
			birds.push_back(-1);
		} else {
			switch(random() % 3) {
				case 0:
					birds.push_back(BIRD[0].size() + 2);
					break;
				case 1:
					birds.push_back(DINO[0].size() + BIRD[0].size());
					break;
				default:
					birds.push_back(DINO[0].size() + BIRD[0].size() * 1.5);
					break;
			}

			cactuses.push_back(-1);
		}

		lastcactus = 0;
	} else {
		lastcactus++;
		cactuses.push_back(-1);
		birds.push_back(-1);
	}

	for(int i = 0; i < WIDTH; ++i) {
		if(cactuses[i] != -1) {
			drawChar(HEIGHT - CACTUS[cactuses[i]].size(), i, CACTUS[cactuses[i]]);
			i += 50;
		} else if(birds[i] != -1) {
			drawChar(HEIGHT - birds[i], i, BIRD[score % 20 < 10]);
			i += 50;
		}
	}

	drawFloor();

	gotoxy(0, 0);

	string scr = to_string(score / 10);
	scr = string(5 - scr.length(), '0') + scr;

	int y;
	if(highScore) {
		string scr2 = to_string(highScore);
		scr2 = string(5 - scr2.length(), '0') + scr2;

		y = WIDTH - NUMLEN * 19 + 1;
		drawChar(1, y, HI);
		y += 4 * NUMLEN;
		
		for(const int &it : scr2) {
			drawChar(1, y, NUMBERS2[it - '0']);
			y += NUMLEN + 1;

		}

		y += 2 * NUMLEN;
	} else {
		y = WIDTH - NUMLEN * 6;
	}

	for(const int &it : scr) {
		drawChar(1, y, NUMBERS[it - '0']);
		y += NUMLEN + 1;
	}

	int ind = -1;
	entry_t* elem = static_cast<entry_t*>(scoreboard.ref());
	while(elem->next != NULL && elem->next->val.getScore() > score / 10) {
		elem = elem->next;
		++ind;
	}

	int offset = log10(ind + 1);

	for(int i = ind; i >= 0 && i > ind - 5; --i) {
		ScoreBoardEntry temp = scoreboard[i];
		gotoxy(ind - i + 1, 0);
		cout << i + 1 << "." << setfill(' ') << setw(size_t(offset - log10(i + 1)) + 1) << " " << temp.getName();
		gotoxy(ind - i + 1, 32),
		cout << setfill('0') << setw(5) << temp.getScore();
	}
}

void kbAction() {
	char c;
	while((c = getchar()) != EOF) {
		if((c == 'w' || c == SPACE || c == UP) && height == 0) {
			crouch = false;
			jumpHandle();
		}

		if(c == 's' || c == DOWN && height == 0) {
			crouch = !crouch;
		}

		if(c == 'p') {
			while((c = getchar()) != 'p') {
				usleep(SLEEPVAL);
			}
		}
	}
}

void jumpHandle() {
	for(int i = -JMPLENGTH; i <= JMPLENGTH; ++i) {
		height = 10 - (i*i) / int(double(JMPLENGTH * JMPLENGTH) / 10.00);
		kbAction();
		drawBg();
		drawDino();

		if(!alive) {
			return;
		}

		fflush(stdout);
		usleep(int(SLEEPVAL / speed));
	}
}

int startGame() {
	lastcactus = 100;
	fill(cactuses.begin(), cactuses.end(), -1);
	fill(birds.begin(), birds.end(), -1);
	height = 0;
	score = 0;
	speed = 1.0;
	alive = true;
	crouch = false;

	while(true) {
		kbAction();
		drawBg();
		drawDino();

		if(!alive) {
			break;
		}

		fflush(stdout);
		usleep(int(SLEEPVAL / speed));
	}

	int selected = 0;
	int x = HEIGHT / 4; 
	drawChar(x, (WIDTH - unicodeLength(GAMEOVER[0])) / 2, GAMEOVER);
	x += GAMEOVER.size();

	int index = insertScoreBoard(username, score / 10);
	if(index != -1) {
		saveScoreBoard();
		scoreBoardIndex = index;
		highScore = score / 10;

		gotoxy(x + 2, (WIDTH - NEWPB.length()) / 2);
		cout << NEWPB;
	}

	while(char c = getchar()) {
		for(int i = 0; i < PLAYAGAIN.size(); ++i) {
			gotoxy(int(HEIGHT - PLAYAGAIN.size() + x) / 2 + i * 2, (WIDTH - PLAYAGAIN[i].size()) / 2);

			if(selected == i)
				cout << "\033[44m" << PLAYAGAIN[i] << "\033[0m";
			else 
				cout << PLAYAGAIN[i];
		}

		switch(c) {
			case -1:
				usleep(SLEEPVAL);
				continue;

			case UP:
				if(selected > 0)
					--selected;
				continue;

			case DOWN:
				if(selected < PLAYAGAIN.size() - 1)
					++selected;
				continue;

			case ENTER:
				switch(selected) {
					case 0:
						return ENTER;

					case 1:
						return BACKSPACE;

					case 2:
						return ESC;
			}
		}
	}

	return ESC;
}

int insertScoreBoard(const string username, const int score) {
	int index = scoreboard.contains({ username, 0 });
	if(index != -1) {
		if(scoreboard[index].getScore() >= score)
			return -1;

		scoreboard.erase(index);
	}

	return scoreboard.insert({ username, score });
}

void saveScoreBoard() {
	entry_t* elem = static_cast<entry_t*>(scoreboard.ref(0));
	ofstream out("scoreboard.txt");
	for(int i = 0; i < scoreboard.size(); ++i) {
		out << elem->val.getName() << " " << elem->val.getScore() << "\n";
		elem = elem->next;
	}
}

void readScoreBoard() {
	ifstream in("scoreboard.txt");
	scoreboard.empty();
	while(!in.eof()) {
		string name;
		int score;
		in >> name >> score;
		
		if(in.eof()) {
			return;
		}

		scoreboard.insert({ name, score });
	}
}

void redrawScoreBoard(int offset) {
	if(scoreboard.size()) {
		int x = 3;
		const int height = HEIGHT / 2 - 2 + offset;
		entry_t* elem = static_cast<entry_t*>(scoreboard.ref(offset));

		for(int i = offset + 1; i <= height && i <= scoreboard.size(); ++i) {
			gotoxy(x, 5);
			cout << i << ". " << setfill(' ') << setw(32 - log10(i)) << left << elem->val.getName() << setfill('0') << setw(5) << right << elem->val.getScore();
			elem = elem->next;
			x += 2;
		}
	}
}

void drawScoreBoard() {
	int offset = 0;

	clear();
	drawBox();

	redrawScoreBoard(offset);

	while(char c = getchar()) {
		switch(c) {
			case DOWN: 
				if(HEIGHT / 2 - 2 < scoreboard.size() - offset) 
					++offset;
					redrawScoreBoard(offset);
				break;
			
			case UP:
				if(offset > 0)
					--offset;
					redrawScoreBoard(offset);
				break;

			case ENTER:
			case BACKSPACE:
				return;

			default:
				usleep(SLEEPVAL);
				continue;
		}
	}
}

bool getUserName() {
	int x = HEIGHT / 4; 
	drawChar(x, (WIDTH - unicodeLength(MENUTITLE[0])) / 2, MENUTITLE);
	x = (x + MENUTITLE.size() + HEIGHT - ENTERUSERNAME.size()) / 2;

	for(const auto &it : ENTERUSERNAME) {
		gotoxy(x, (WIDTH - it.size() - 1) / 2);
		cout << it;	
		x++;
	}

	x += 4;
	gotoxy(x, (WIDTH - 21) / 2);
	cout << "\033[44m" << setfill(' ') << setw(21) << "";
	
	char c;
	while((c = getchar()) && c != ENTER || username.empty()) {
		gotoxy(x, (WIDTH - username.length() - 1) / 2);
		cout << " " << username << " ";
		if(c == -1) {
			usleep(SLEEPVAL);
			continue;
		}

		if(c == ESC) {
			username = "";
			break;
		}

		if(c == BACKSPACE) {
			if(!username.empty())
				username.pop_back();
			continue;
		}

		if(((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') && username.length() <= 16) {
			if((c >= 'a' && c <= 'z')) c -= 32;
			username.push_back(c);
		}
	}

	cout << "\033[0m";
	return !username.empty();
}

void menu() {
	int selected = 0;
	username = "";
	highScore = 0;
	clear();
	drawBox();
	readScoreBoard();
	
	int x = HEIGHT / 4;
	drawChar(x, (WIDTH - unicodeLength(MENUTITLE[0])) / 2, MENUTITLE);
	x += MENUTITLE.size();

	while(char c = getchar()) {
		for(int i = 0; i < MENU.size(); ++i) {
			gotoxy(int(HEIGHT - MENU.size() + x) / 2 + i * 2, (WIDTH - MENU[i].size()) / 2);

			if(selected == i)
				cout << "\033[44m" << MENU[i] <<"\033[0m";
			else 
				cout << MENU[i];
		}

		switch(c) {
			case -1:
				usleep(SLEEPVAL);
				continue;

			case UP:
				if(selected > 0)
					--selected;
				continue;

			case DOWN:
				if(selected < PLAYAGAIN.size() - 1)
					++selected;
				continue;

			case ENTER:
				switch(selected) {
					case 0:
						clear();
						drawBox();
						
						if(!getUserName()) {
							menu();
							return;
						}

						scoreBoardIndex = scoreboard.contains({ username, 0 });
						
						if(scoreBoardIndex != -1) {
							highScore = scoreboard[scoreBoardIndex].getScore();
						}

						switch(startGame()) {
							case ESC:
								return;

							case ENTER:
								startGame(); 
						}

						menu();
						return;
					
					case 1:
						drawScoreBoard();
						menu();
						return;
					
					case 2:
						clear();
						return;
				}
		}
	}
}

int main() {
	signal(SIGINT, signalHandler);
	init();
	menu();
	cleanup();
	return 0;
}