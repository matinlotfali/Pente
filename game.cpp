#include "game.h"
#include "QTime"

Game::Game(QObject *parent)
    : QThread(parent)
{
    timer = new QTime();
    depthDeep = 0;
    size = 15;        
    NewGame();
}

Game::~Game()
{
    delete timer;
}

void Game::run(void)
{        
    timer->restart();
    Piece board[19][19];
    for(int i=0; i<19; i++)
        for(int j=0; j<19; j++)
            board[i][j] = this->board[i][j];
    AlphaBetaPruning(INT_MIN, INT_MAX, 0,board);
    int t = timer->elapsed();

    if(t < 1000)
        msleep(1000 - t);

    this->board[bestX][bestY] = Black;
    lastX = bestX;
    lastY = bestY;
    turn = White;
    DoneSignal();
}

QRect Game::GetPentePieces()
{
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            if (board[i][j])
            {
                int z;
                for (z = 1; z < 5; z++)
                    if (j + z < size)
                    {
                        if (board[i][j] != board[i][j + z])
                            break;
                    }
                    else break;

                if (z == 5)
                {
                    QRect result(i, j, 0, 4);
                    return result;
                }


                /*for (z = 1; z < 5; z++)
                    if (j - z >= 0)
                    {
                        if (area[i, j] != area[i, j - z])
                            break;
                    }
                    else break;
                if (z == 5)
                    return new Rectangle(i, j, i, j - --z);*/

                for (z = 1; z < 5; z++)
                    if (i + z < size)
                    {
                        if (board[i][j] != board[i + z][j])
                            break;
                    }
                    else break;
                if (z == 5)
                {
                    QRect result(i, j, 4, 0);
                    return result;
                }

                /*for (z = 1; z < 5; z++)
                    if (i - z >= 0)
                    {
                        if (area[i, j] != area[i - z, j])
                            break;
                    }
                    else break;
                if (z == 5)
                    return new Rectangle(i, j, i - --z, j);*/

                for (z = 1; z < 5; z++)
                    if (j + z < size && i + z < size)
                    {
                        if (board[i][j] != board[i + z][j + z])
                            break;
                    }
                    else break;

                if (z == 5)
                {
                    QRect result(i, j, 4, 4);
                    return result;
                }

                for (z = 1; z < 5; z++)
                    if (j - z >= 0 && i - z >= 0)
                    {
                        if (board[i][j] != board[i - z][j - z])
                            break;
                    }
                    else break;
                if (z == 5)
                {
                    QRect result(i, j, -4, 4);
                    return result;
                }

                for (z = 1; z < 5; z++)
                    if (i + z < size && j - z >= 0)
                    {
                        if (board[i][j] != board[i + z][j - z])
                            break;
                    }
                    else break;
                if (z == 5)
                {
                    QRect result(i, j, 4, -4);
                    return result;
                }

                for (z = 1; z < 5; z++)
                    if (i - z >= 0 && j + z < size)
                    {
                        if (board[i][j] != board[i - z][j + z])
                            break;
                    }
                    else break;
                if (z == 5)
                {
                    QRect result(i, j, -4, 4);
                    return result;
                }

            }
        }

    QRect e(0,0,0,0);
    return e;
}

void Game::NewGame()
{
    turn = White;
    lastX = lastY = -1;
    gameOverTimer.start();
    for(int i=0; i<19; i++)
        for(int j=0; j<19; j++)
            board[i][j] = Empty;
}

Won Game::IsOver(Piece board[19][19])
{
    if (CostBlack(Black, White,board) >= 100000000)
    {
        if(gameOverTimer.elapsed() > 5000)
            gameOverTimer.restart();
        return BlackWon;
    }
    if (CostBlack(White, Black,board) >= 100000000)
    {
        if(gameOverTimer.elapsed() > 5000)
            gameOverTimer.restart();
        return WhiteWon;
    }

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(board[i][j] == Empty)
                return NoneWon;

    if(gameOverTimer.elapsed() > 5000)
        gameOverTimer.restart();
    return Draw;
}

int Game::CostBlack(Piece black, Piece white, Piece board[19][19])
{
    int c = 0;
    for (int k = 0; k < size; k++)
    {
        int countBlack = 0;
        int n = 0;
        for (int z = 0; z < size; z++)
        {
            n++;
            if (board[k][z] == black)
                countBlack++;
            else if (board[k][z] == white)
            {
                n = 0;
                countBlack = 0;
            }

            if (n == 5)
            {
                n--;
                switch (countBlack)
                {
                    case 5: c += 100000000; break;
                    case 4: c += 1000000; break;
                    case 3: c += 10000; break;
                    case 2: c += 100; break;
                    case 1: c += 1; break;
                    default: break;
                }

                if (board[k][z - 4] == black)
                    countBlack--;
            }
        }
    }

    for (int k = 0; k < size; k++)
    {
        int countBlack = 0;
        int n = 0;
        for (int z = 0; z < size; z++)
        {
            n++;
            if (board[z][k] == black)
                countBlack++;
            else if (board[z][k] == white)
            {
                n = 0;
                countBlack = 0;
            }

            if (n == 5)
            {
                n--;
                switch (countBlack)
                {
                    case 5: c += 100000000; break;
                    case 4: c += 1000000; break;
                    case 3: c += 10000; break;
                    case 2: c += 100; break;
                    case 1: c += 1; break;
                    default: break;
                }

                if (board[z - 4][k] == black)
                    countBlack--;
            }

        }
    }

    for (int k = 0; k < 38; k++)
    {
        int countBlack = 0;
        int n = 0;
        for (int z = 0; z < size; z++)
        {
            if (k - z < size && k - z >= 0)
            {
                n++;
                if (board[k - z][z] == black)
                    countBlack++;
                else if (board[k - z][z] == white)
                {
                    n = 0;
                    countBlack = 0;
                }

                if (n == 5)
                {
                    n--;
                    switch (countBlack)
                    {
                        case 5: c += 100000000; break;
                        case 4: c += 1000000; break;
                        case 3: c += 10000; break;
                        case 2: c += 100; break;
                        case 1: c += 1; break;
                        default: break;
                    }

                    if (board[k - z + 4][z - 4] == black)
                        countBlack--;
                }
            }
        }
    }

    for (int k = -size; k < size; k++)
    {
        int countBlack = 0;
        int n = 0;
        for (int z = 0; z < size; z++)
        {
            if (k + z < size && k + z >= 0)
            {
                n++;
                if (board[k + z][z] == black)
                    countBlack++;
                else if (board[k + z][z] == white)
                {
                    n = 0;
                    countBlack = 0;
                }

                if (n == 5)
                {
                    n--;
                    switch (countBlack)
                    {
                        case 5: c += 100000000; break;
                        case 4: c += 1000000; break;
                        case 3: c += 10000; break;
                        case 2: c += 100; break;
                        case 1: c += 1; break;
                    default: break;
                    }

                    if (board[k + z - 4][z - 4] == black)
                        countBlack--;
                }
            }
        }
    }
    return c;
}

int Game::StateCost(Piece board[19][19])
{
    return CostBlack(Black, White,board) - CostBlack(White, Black,board);
}

bool Game::IsNear(int i, int j,int distance, Piece board[19][19])
{
    if (i >= distance)
    {
        if (board[i - distance][j])
            return true;

        if (j >= distance)
            if (board[i - distance][j - distance])
                return true;

        if (j < size - distance)
            if (board[i - distance][j + distance])
                return true;
    }

    if (i < size - distance)
    {
        if (board[i + distance][j])
            return true;

        if (j >= distance)
            if (board[i + distance][j - distance])
                return true;

        if (j < size - distance)
            if (board[i + distance][j + distance])
                return true;
    }

    if (j < size - distance)
        if (board[i][j + distance])
            return true;

    if (j >= distance)
        if (board[i][j - distance])
            return true;

    return false;
}

int Game::AlphaBetaPruning(int alpha, int beta, int depth, Piece board[19][19])
{    
    if (depth < depthDeep && !IsOver())
    {
        int near[size][size];
        for(int i=0; i<size; i++)
            for(int j=0; j<size; j++)
            {
                if(!board[i][j])
                {
                    int k;
                    for(k=1; k<5; k++)
                    {
                        if(IsNear(i,j,k,board))
                        {
                            near[i][j] = k;
                            break;
                        }
                    }
                    if(k==5)
                        near[i][j] = 100;
                }
                else
                    near[i][j] = 0;
            }

        for(int k = 1; k<5; k++)
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    if (near[i][j] == k)
                        {
                            board[i][j] = (depth % 2 == 0)? Black: White;

                            int newScore = AlphaBetaPruning(alpha, beta, depth + 1,board);

                            if(depth % 2 == 0)
                            {
                                if (alpha < newScore)
                                {
                                    alpha = newScore;
                                    if (depth == 0)
                                    {
                                        bestX = i;
                                        bestY = j;
                                    }
                                }
                            }
                            else
                                if (beta > newScore)
                                    beta = newScore;

                            board[i][j] = Empty;

                            if (alpha >= beta)
                                return (depth % 2 == 0)? alpha: beta;
                        }
    }
    else
    {
        if(depth % 2 == 0)
            alpha = StateCost(board);
        else
            beta = StateCost(board);
    }
    return (depth % 2 == 0)? alpha: beta;
}
