#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <ctime>
#include <thread>



#define PART 15;

struct Point {
    int posX;
    int posY;
};

enum Direction {
    W = 'w',
    A = 'a',
    S = 's',
    D = 'd'
};

class GameSpace;

class Snake {
public:
    Snake(GameSpace& gameSpace);
    void ControlMove(GameSpace& gameSpace, char);
    int Moving(GameSpace& );
    void Eating(GameSpace& );

    friend GameSpace;

private:
    void OneMove(GameSpace &, int, int);

    Direction direct = D;
    Point head;
    Point tail;
    std::vector<Point> snakeBody;
    int eat = 0;
};

class GameSpace {
public:
    GameSpace();
    int Update(Snake &snake);
    void PrintScore();
    friend Snake::Snake(GameSpace &gameSpace);
    friend int Snake::Moving(GameSpace& gameSpace);
    friend void Snake::ControlMove(GameSpace& gameSpace, char);
    friend void Snake::OneMove(GameSpace&, int, int);
    friend void Snake::Eating (GameSpace&);


private:
    static const int leftBorder = 0, rightBorder = 15, upBorder = 15, downBorder = 0;
    unsigned char cellArray[upBorder][rightBorder];
    int difficulty = 600;  // time (in ms), before the snake takes a move
    int score = 0;
    time_t intervalCtrl;
    int trapTime = 10; // trap spawn time
};

GameSpace::GameSpace() {
    intervalCtrl = time(0);
                                                // fill borders
    for (int i = 0; i < upBorder; i++) {
        cellArray[i][0] = '|';
        cellArray[i][rightBorder - 1] = '|';
    }
    for (int i = 0; i < rightBorder; i++) {
        cellArray[upBorder - 1][i] = 'W';
        cellArray[0][i] = 'M';
    }
    
    for (int i = 1; i < rightBorder - 1; i++) { // fill main space
        for (int j = 1; j < upBorder - 1; j++) {
            cellArray[i][j] = ' ';
        }
    }
    
    srand(time(NULL));
    int randY = rand() % (upBorder - 2) + 1;
    int randX = rand() % (rightBorder - 2) + 1;
    do {
        if (cellArray[randY][randX] == ' ')
        {
            cellArray[randY][randX] = 'O';
            break;
        }
        randY = rand() % (upBorder - 2) + 1;
        randX = rand() % (rightBorder - 2) + 1;
    } while (true);

}

int GameSpace::Update(Snake &snake) {
    system("CLS");
    
    switch (cellArray[snake.head.posY][snake.head.posX]) {
    case ' ':
        cellArray[snake.head.posY][snake.head.posX] = PART;
        break;
    case 'O':
        cellArray[snake.head.posY][snake.head.posX] = PART;
        snake.Eating(*this);
        break;
    case 'M':
    case 'W':
    case '|':
    case 'X':
    case 15 :
        return 0;
        break;
    }
    
    time_t curTime = time(0);
    if (((curTime - intervalCtrl) % trapTime == 0) && (intervalCtrl < curTime)) {
        intervalCtrl += trapTime;
        int randY = rand() % (upBorder - 2) + 1;
            int randX = rand() % (rightBorder - 2) + 1;
            do {
                if (cellArray[randY][randX] == ' ')
                {
                    cellArray[randY][randX] = 'X';
                        break;
                }
                randY = rand() % (upBorder - 2) + 1;
                    randX = rand() % (rightBorder - 2) + 1;
            } while (true);
    }
    
    
    std::cout << "Score : " << score <<"\n\n";

    for (int i = 0; i < rightBorder; i++) {    // print gameSpace
        for (int j = 0; j < upBorder; j++) {
            std::cout << cellArray[i][j];
        }
        std::cout << '\n';
    }
    return 1;
}

Snake::Snake(GameSpace &gameSpace) {
    Point startPoint;
    startPoint.posX = gameSpace.rightBorder / 2;
    startPoint.posY = gameSpace.upBorder / 2;
    gameSpace.cellArray[gameSpace.rightBorder/2][gameSpace.upBorder/2] = PART;
    
    tail = startPoint;
    snakeBody.push_back(startPoint);
    startPoint.posX++;
    snakeBody.push_back(startPoint);
    head = startPoint;
}

void Snake::ControlMove(GameSpace& gameSpace, char c) {
    switch (c) {
    case 'w':       
        if (direct != S) {
            OneMove(gameSpace, 0, -1);
            direct = W;
        }
        else {
            OneMove(gameSpace, 0, 1);
        }
        break;
    case 's':   
        if (direct != W) {
            OneMove(gameSpace, 0, 1);
            direct = S;
        }
        else {
            OneMove(gameSpace, 0, -1);
        }           
        break;
    case 'a':   
        if (direct != D) {
            OneMove(gameSpace, -1, 0);
            direct = A;
        }
        else {
            OneMove(gameSpace, 1, 0);
        }           
        break;
    case 'd':    
        if (direct != A) {
            OneMove(gameSpace, 1, 0);
            direct = D;
        }
        else {
            OneMove(gameSpace, -1, 0);
        }           
        break;
    default:
        ;
    }
}

void Snake::OneMove(GameSpace &gameSpace, int x, int y) {
    head.posX += x;
    head.posY += y;
    snakeBody.push_back(head);
    if (eat) {
        eat = 0;
    }
    else {
        gameSpace.cellArray[tail.posY][tail.posX] = ' ';
        tail = snakeBody[1];
        snakeBody.erase(snakeBody.begin());
    }
}

int Snake::Moving(GameSpace &gameSpace) {
    char c = 11;
    std::thread t([&c] {
        c = _getch();
        });
    t.detach();
    Sleep(gameSpace.difficulty);
    if (c != 11) {
        ControlMove(gameSpace, c);
    }
    else {
        switch (direct) {
        case W:
            OneMove(gameSpace, 0, -1);
            break;
        case S:
            OneMove(gameSpace, 0, 1);
            break;
        case A:
            OneMove(gameSpace, -1, 0);
            break;
        case D:
            OneMove(gameSpace, 1, 0);
            break;
        }
    } 
    return gameSpace.Update(*this);
}

void Snake::Eating(GameSpace& gameSpace) {
    eat = 1;
    int randY = rand() % (gameSpace.upBorder - 2) + 1;
    int randX = rand() % (gameSpace.rightBorder - 2) + 1;
    do {
        if (gameSpace.cellArray[randY][randX] == ' ')
        {
            gameSpace.cellArray[randY][randX] = 'O';
            break;
        }
        randY = rand() % (gameSpace.upBorder - 2) + 1;
        randX = rand() % (gameSpace.rightBorder - 2) + 1;
    } while (true);

    gameSpace.score += 10;
    if (gameSpace.score % 50 == 0) gameSpace.difficulty -= 100;
}

void gameOver() {
    std::cout << '\n';
    std::cout << "  Y     Y      O O       U      U           L         O O       S S S     E E E   \n";
    std::cout << "   Y   Y      O   O      U      U           L        O   O     S          E       \n";
    std::cout << "    Y Y      O     O     U      U           L       O     O     S S S     E E     \n";
    std::cout << "     Y        O   O      U      U           L        O   O           S    E       \n";
    std::cout << "     Y         O O         U U U            L L L     O O       S S S     E E E \n\n";
}

void GameSpace::PrintScore () {
    std::cout << "SCORE : " << score<<'\n';
}

int main()
{
    GameSpace gameSpace;
    Snake snake(gameSpace);
    gameSpace.Update(snake);

    while (true) {
        if (!snake.Moving(gameSpace)) {
            gameSpace.PrintScore();
            gameOver();
            return 0;
        }       
    }
    return 0;
}

                                                                                                                            