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
    friend Snake::Snake(GameSpace &gameSpace);
    friend int Snake::Moving(GameSpace& gameSpace);
    friend void Snake::ControlMove(GameSpace& gameSpace, char);
    friend void Snake::OneMove(GameSpace&, int, int);
    friend void Snake::Eating (GameSpace&);


private:
    static const int leftBorder = 0, rightBorder = 12, upBorder = 12, downBorder = 0;
    unsigned char cellArray[upBorder][rightBorder];

};

GameSpace::GameSpace() {
                                                // fill borders
    for (int i = 0; i < upBorder; i++) {
        cellArray[i][0] = '|';
        cellArray[i][rightBorder - 1] = '|';
    }
    for (int i = 0; i < rightBorder; i++) {
        //cellArray[0][i] = '_';
        cellArray[upBorder - 1][i] = '-';
        cellArray[0][i] = '-';
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
    //cellArray[8][8] = 'O';

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
    case '-':
    case '|':
    case 15 :
        return 0;
        break;
    }
    
    
    /*for (auto it = snake.snakeBody.begin(); it != snake.snakeBody.end(); it++) {
        switch (cellArray[it->posY][it->posX]) {
        case ' ': 
            cellArray[it->posY][it->posX] = PART;
            break;
        case 'O':
            cellArray[it->posY][it->posX] = PART;
            snake.Eating(*this);
            break;
        case '-':
        case '|': 
        //case 15 :
            return 0;
            break;
        }
       




        //std::cout << it->posY << '\n';
        //std::cout << it->posX << '\n';
        
        //std::cout << "ep" << '\n';
    }*/
        
    
    for (int i = 0; i < rightBorder; i++) {    // print gameSpace
        for (int j = 0; j < upBorder; j++) {
            std::cout << cellArray[i][j];
        }
        std::cout << '\n';
    }
    return 1;
}

Snake::Snake(GameSpace &gameSpace) {
    //int rBord = gameSpace.rightBorder;
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
    //char c = 0;
    //std::cout << "MOVEEEEsssssssssssss\n";
    //c = _getch();
    //std::cout << "MOVEEEEppppppppp\n";
    //auto it = snakeBody.begin();
    //it = std::next(it, 1);
    switch (c) {
    case 'w':   // head.posY--;  
        //tail = *it;       
        OneMove(gameSpace, 0, -1);
        direct = W;
        //std::cout << snakeBody.begin()->posY << "\n";
        break;
    case 's':   //head.posY++;
        OneMove(gameSpace, 0, 1);
        direct = S;
        break;
    case 'a':   // head.posX--;
        OneMove(gameSpace, -1, 0);
        direct = A;
        break;
    case 'd':    //head.posX++;
        OneMove(gameSpace, 1, 0);
        direct = D;
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
    Sleep(1000);
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
}

void gameOver() {
    std::cout << "\n\n";
    std::cout << "  Y     Y      O O       U      U           L         O O       S S S     E E E   \n";
    std::cout << "   Y   Y      O   O      U      U           L        O   O     S          E       \n";
    std::cout << "    Y Y      O     O     U      U           L       O     O     S S S     E E     \n";
    std::cout << "     Y        O   O      U      U           L        O   O           S    E       \n";
    std::cout << "     Y         O O         U U U            L L L     O O       S S S     E E E \n\n";
}

int main()
{

    //std::cout << "\033[5;35;46m Hello World!fjwo";

    GameSpace gameSpace;
    Snake snake(gameSpace);
    gameSpace.Update(snake);

    time_t workTime = time(0) + 60;
    //time_t moveTime = time(0) + 1;

    while (time_t curTime = time(0) < workTime) {
        
        /*if (curTime == moveTime) {
            
        }*/
        //snake.Moving(gameSpace);
        //snake.ControlMove(gameSpace);
        //snake.IndependentMoving(gameSpace);
        if (!snake.Moving(gameSpace)) {
            gameOver();
            return 0;
        }
        //std::cout << _getch() << '\n';
    }
    //char c = _getch();
    //std::cout << c << '\n';
    //system("pause");

    return 0;
}

                                                                                                                            