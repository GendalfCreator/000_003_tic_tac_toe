#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

#define POINT_ITEM(a, x, y) (*((*(a + y)) + x))
#define CHECK_DOT(x, sz) ((x) >= 0 && (x) < (sz))

enum PLAYER {HUMAN='X', AI='0', EMPTY='_'};

typedef struct {
  int szY;
  int szX;
  PLAYER** map;
  int towin;
} Field;

char getVal(PLAYER** array, const int x, const int y) {
  return POINT_ITEM(array, x, y);
}

void setVal(PLAYER** array, const int x, const int y, PLAYER value) {
  POINT_ITEM(array, x, y) = value;
}

bool isValid(Field &field, int x, int y) {
  return CHECK_DOT(x, field.szX) && CHECK_DOT(y, field.szY);
}

bool isEmpty(Field &field, int x, int y) {
  return getVal(field.map, x, y) == EMPTY;
}

//Функция проверки на корректность введённых данных
bool checkCin() {
  if (cin.fail()){
  cin.clear();
  cin.ignore(65535,'\n');
  return false;
  }
  return true;
}

void initGame(Field &field) {
  field.towin = 3;
  field.map = new PLAYER* [field.szY];
  for (int y = 0; y < field.szY; ++y) {
      *(field.map + y) = new PLAYER [field.szX];
      for (int x = 0; x < field.szX; ++x) {
          setVal(field.map, x, y, EMPTY);
        }
    }
}

void printField(Field &field) {
  system("clear");
  cout << "   ";
  for (int i = 1; i <= field.szX; i++) {
      cout << " "<< i;
    }
  cout << endl;
  for (int y = 0; y < field.szY; ++y) {
      cout << " " << y + 1 << " |";
      for (int x = 0; x < field.szX; ++x) {
          cout << getVal(field.map, x, y) << "|";
        }
      cout << endl;
    }
  cout << endl;
}
bool checkLine(Field &field, int x, int y, int vx, int vy, int len, PLAYER player) {
  const int endx = x + (len - 1) * vx;
  const int endy = y + (len - 1) * vy;

  if (!isValid(field, endx, endy))
    return false;
  for (int i = 0; i < len; i++)
    if (getVal(field.map, (x + i * vx), (y + i * vy)) != player)
      return false;
  return true;
}

bool checkWin(Field &field, PLAYER player) {
  for (int y = 0; y < field.szY; y++) {
      for (int x = 0; x < field.szX; x++) {
          if (checkLine(field, x, y, 1, 0, field.towin, player)) return true;
          if (checkLine(field, x, y, 1, 1, field.towin, player)) return true;
          if (checkLine(field, x, y, 0, 1, field.towin, player)) return true;
          if (checkLine(field, x, y, 1, -1, field.towin, player)) return true;
        }
    }
  return false;
}

bool checkDraw(Field &field) {
  for (int y = 0; y < field.szY; y++)
    for (int x = 0; x < field.szX; x++)
      if (isEmpty(field, x, y))
        return false;
  return true;
}

void turnHuman(Field &field) {
  int x, y;
  do {
      cout << "Введите номер строки и столбца через пробел: ";
      cin >> y >> x;
      x--;
      y--;

      cin.ignore(65535,'\n');//Чистка буфера для предотвращения ввода сразу нескольких ходов подряд
  }
  while (!checkCin() || (!isValid(field, x, y) || !isEmpty(field, x, y)));
  setVal(field.map, x, y, HUMAN);
}

int checkAiWin(Field &field){
  for (int y = 0; y < field.szY; y++) {
      for (int x = 0; x < field.szX; x++) {
          if (isEmpty(field, x, y)) {
              setVal(field.map, x, y, AI);
              if (checkWin(field, AI)) {
                  return 1;
                }
              else {
                setVal(field.map, x, y, EMPTY);
                }
            }
        }
    }
  return 0;
}

int checkHumanWin(Field &field) {
  for (int y = 0; y < field.szY; y++) {
      for (int x = 0; x < field.szX; x++) {
          if (isEmpty(field, x, y)) {
              setVal(field.map, x, y, HUMAN);
              if (checkWin(field, HUMAN)) {
                  setVal(field.map, x, y, AI);
                  return 1;
                }
              else {
                  setVal(field.map, x, y, EMPTY);
                }
            }
        }
    }
  return 0;
}

void turnAi(Field &field) {
    int a = 0, b = 0;
    a = checkAiWin(field);
    if (a == 0) {
      b = checkHumanWin(field);
      if (b == 0) {
          int x, y;

          random_device rd;
          mt19937 mt(rd());
          uniform_real_distribution <double> distx(0, field.szX);
          uniform_real_distribution <double> disty(0, field.szY);

          do {
              x = distx(mt);
              y = disty(mt);
            }
          while (!isEmpty(field, x, y));
          setVal(field.map, x, y, AI);
        }
    }
}

void tictactoe() {
  while (true) {
      Field field;

      system("clear");

      do {
          cout << "Новая игра\nВведите количество строк и столбцов (от 3 до 9): ";
          cin >> field.szY;
          cin.ignore(65535,'\n');
      }
      while (!checkCin() || (field.szY < 3 || field.szY > 9));
      do {
          cout << "Введите количество столбцов по ширине (от 3 до 9): ";
          cin >> field.szX;
          cin.ignore(65535,'\n');
      }
      while (!checkCin() || (field.szX < 3 || field.szX > 9));

      initGame(field);

      while (true) {
          printField(field);
          turnHuman(field);
          if (checkWin(field, HUMAN) || checkDraw(field)) {
            printField(field);
            if (checkWin(field, HUMAN)) {
                cout << "Human win" << endl;
              }
            else {
                cout << "Draw" << endl;
              }
            break;
          }
        turnAi(field);
        if (checkWin(field, AI) || checkDraw(field)) {
            printField(field);
            cout << "AI win" << endl;
            break;
          }
      }

      string answer;
      cout << "PLay again? (Y): ";
      cin >> answer;

      transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

      if(answer.find('y') != 0) {
          break;
        }
    }
}

int main()
{
  tictactoe();

  return 0;
}
