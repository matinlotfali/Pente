#ifndef GAME_H
#define GAME_H

#include "QRect"
#include "QThread"
#include "QTime"

enum Piece
{
    Empty = 0, White, Black
};

enum Won
{
    NoneWon = 0, WhiteWon, BlackWon, Draw
};

class Game : public QThread
{
    Q_OBJECT

public:
    explicit Game(QObject *parent);
    ~Game();

    int lastX, lastY, size, depthDeep;
    Piece board[19][19];    
    Piece turn;
    QTime gameOverTimer;

    Won IsOver() {  return IsOver(this->board); }
    Won IsOver(Piece board[19][19]);
    void NewGame();
    QRect GetPentePieces();

signals:
    void DoneSignal();

protected:
    void run(void);

private:    
    QTime* timer;    
    int bestX, bestY;
    inline bool IsNear(int i, int j,int distance, Piece board[19][19]);
    inline int StateCost(Piece board[19][19]);
    inline int CostBlack(Piece black, Piece white, Piece board[19][19]);
    int AlphaBetaPruning(int alpha, int beta, int depth, Piece board[19][19]);
};

#endif // GAME_H
