#include "opengl.h"
#include "QDebug"
#include "QTime"
#include "QTimer"
#include "QPainter"
#include "QMouseEvent"
#include "QMessageBox"
#include "QThread"
#include "qmath.h"
#include "mainform.h"

MainWindow::MainWindow(MainForm *parent) :
    QOpenGLWidget(parent)
{
    drawState = FirstMenu;
    afterFadeState = FirstMenu;
    fade = 0;
    multiplayer=false;
    myParent = parent;

    game = new Game(this);
    connect(game, &Game::DoneSignal, this, &MainWindow::AIDone);

    QList<QString> *difficultyItems = new QList<QString>();
    difficultyItems->append("Easy");
    difficultyItems->append("Medium");
    difficultyItems->append("Hard");
    difficultyItems->append("Expert");
    difficultyMenu = new Menu("Select Difficulty:", difficultyItems);

    QList<QString> *firstMenuItems = new QList<QString>();
    firstMenuItems->append("Single Player");
    firstMenuItems->append("Two Players");
    firstMenuItems->append("About");
    firstMenuItems->append("Exit");
    firstMenu = new Menu("", firstMenuItems);

    QList<QString> *sizeMenuItems = new QList<QString>();
    sizeMenuItems->append("10 x 10");
    sizeMenuItems->append("15 x 15");
    sizeMenuItems->append("19 x 19");
    sizeMenuItems->append("Back");
    sizeMenu = new Menu("Select Size:", sizeMenuItems);

    QList<QString> *aboutMenuItems = new QList<QString>();
    aboutMenuItems->append("Back");
    aboutMenu = new Menu("", aboutMenuItems, "\nProgrammer:\nMatin Lotfaliei");    

    fadeTimer.restart();
}

MainWindow::~MainWindow()
{    
    game->terminate();
    game->wait();
    delete game;
    delete difficultyMenu;
    delete sizeMenu;
    delete firstMenu;
}

void MainWindow::AIDone()
{    
    emit myParent->playSoundOff();
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    QOpenGLWidget::mousePressEvent(e);
    if(game->isRunning())
        return;    

    switch(drawState)
    {
    case AboutMenu:
        if(aboutMenu->SelectItem(e->x(),e->y(),this)==0)
        {
            afterFadeState = FirstMenu;
            fadeTimer.restart();
        }
        return;

    case FirstMenu:
        switch(sizeMenu->SelectItem(e->x(),e->y(),this))
        {
        case 0:
            afterFadeState = DifficultyMenu;
            multiplayer = false;
            p1 = "You";
            p2 = "CPU";
            fadeTimer.restart();
            break;
        case 1:
            afterFadeState = SizeMenu;            
            multiplayer = true;
            p1 = "Player 1";
            p2 = "Player 2";
            fadeTimer.restart();
            break;
        case 2:
            afterFadeState = AboutMenu;
            fadeTimer.restart();
            break;
        case 3:
            myParent->close();
            break;
        }
        return;

    case SizeMenu:
        switch(sizeMenu->SelectItem(e->x(),e->y(),this))
        {
        case 0:
            game->size = 10;
            cellSize = boardSize/(double)game->size;
            game->NewGame();
            afterFadeState = Play;
            fadeTimer.restart();
            break;
        case 1:
            game->size = 15;
            cellSize = boardSize/(double)game->size;
            game->NewGame();
            afterFadeState = Play;
            fadeTimer.restart();
            break;
        case 2:
            game->size = 19;
            cellSize = boardSize/(double)game->size;
            game->NewGame();
            afterFadeState = Play;
            fadeTimer.restart();
            break;
        case 3:
            afterFadeState = FirstMenu;
            fadeTimer.restart();
            break;
        }
        return;

    case DifficultyMenu:
        game->depthDeep = difficultyMenu->SelectItem(e->x(),e->y(),this) + 1;
        if(game->depthDeep > 0)
        {
            afterFadeState = SizeMenu;
            fadeTimer.restart();
        }
        return;

    case Play:
        int x = (int)((e->x() - boardLeft) / cellSize);
        int y = (int)((e->y() - boardTop) / cellSize);

        if(!(x >= 0 && x<game->size && y>=0 && y<game->size))
            return;

        if(game->board[x][y])
            return;

        game->lastX = x;
        game->lastY = y;

        game->board[game->lastX][game->lastY] = game->turn;
        if(game->turn == White)
            emit myParent->playSoundOn();
        else
            emit myParent->playSoundOff();
        game->turn = game->turn==White? Black: White;        

        if(!game->IsOver())
            if(!multiplayer)
                game->start(QThread::TimeCriticalPriority);
    }    
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    QOpenGLWidget::resizeEvent(e);
    int width = this->width();
    int height = this->height();
    if(width >= height)
    {
        boardLeft = (width - height)/2;
        boardTop = 0;
        boardSize = height;
    }
    else
    {
        boardLeft = 0;
        boardTop = (height - width)/2;
        boardSize = width;
    }

    cellSize = boardSize/(double)game->size;

    qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<" : Form resized. -> "<<width<<","<<height<<" CellSize: "<<cellSize;
}

void MainWindow::DrawBackground(QPainter *painter)
{
    int width = this->width();
    int height = this->height();    

    painter->fillRect(0,0,width,height,Qt::black);

    QPixmap background (":/images/others/background.jpg");
    painter->drawPixmap(boardLeft,boardTop,boardSize,boardSize, background);

    painter->setPen(Qt::white);
    double cell2 = cellSize/2.0;
    int locEnd = (int)((game->size-1)*cellSize + cell2);
    for(int i = 0; i<game->size; i++)
    {
        int loc = (int)(i*cellSize + cell2);        
        painter->drawLine(boardLeft + loc, boardTop + cell2, boardLeft + loc, boardTop + locEnd);
        painter->drawLine(boardLeft + cell2, boardTop + loc, boardLeft + locEnd, boardTop + loc);
    }
}

void MainWindow::DrawFadeOut(QPainter *painter)
{
    int elapsed = fadeTimer.elapsed();
    if(elapsed < 500)
         fade = 255-elapsed*255/500;
    else
    {
         fade = 0;
         drawState = afterFadeState;
         fadeTimer.restart();
    }
    painter->fillRect(rect(),QColor(0,0,0,255-fade));
}

void MainWindow::DrawFadeIn(QPainter *painter)
{
    int elapsed = fadeTimer.elapsed();
    if(elapsed < 500)
         fade = elapsed*255/500;
    else
    {
         fade = 255;
         fadeTimer.restart();
    }
    painter->fillRect(rect(),QColor(0,0,0,255-fade));
}

void MainWindow::DrawPlayGame(QPainter *painter)
{        
    DrawBackground(painter);

    QPixmap whitePiece (":/images/others/white.png");
    QPixmap blackPiece (":/images/others/black.png");
    QImage arrow (":/images/others/right.png");
    QPen pen(Qt::yellow,cellSize/10);
    QFont f ("Tahoma");
    f.setPixelSize(boardSize/8);
    painter->setPen(pen);
    painter->setFont(f);
    painter->setBrush(Qt::transparent);

    int board8 = boardSize/8;
    int board32 = boardSize/32;

    if(this->width() < this->height())
    {
        if(!game->IsOver())
        {
            int msec = QDateTime::currentDateTime().time().msec();
            int pos = (1+sin(msec*6.28/1000.0))*board8/2;
            if(game->turn == White && !game->isRunning())
                painter->drawImage(QRect(pos,boardTop+boardSize+boardSize/32,board8,board8),arrow);
            else
                painter->drawImage(QRect(pos,boardTop- board8-boardSize/32,board8,board8),arrow);
        }

        painter->drawPixmap(2*board8+board32,boardTop+boardSize+board32,board8,board8,whitePiece);
        painter->drawEllipse(2*board8+board32,boardTop+boardSize+board32,board8,board8);
        painter->drawText(3*board8+2*board32,boardTop+boardSize+board32,boardSize,board8,Qt::AlignVCenter,p1);

        painter->drawPixmap(2*board8+board32,boardTop-board8-board32,board8,board8,blackPiece);
        painter->drawEllipse(2*board8+board32,boardTop-board8-board32,board8,board8);
        painter->drawText(3*board8+2*board32,boardTop-board8-board32,boardSize,board8,Qt::AlignVCenter,p2);
    }
    else
    {
        if(!game->IsOver())
        {
            int msec = QDateTime::currentDateTime().time().msec();
            int pos = (1+sin(msec*6.28/1000.0))*board8/2;
            if(game->turn == White && !game->isRunning())
                painter->drawImage(QRect(pos+boardLeft+boardSize+board8+board8/2,boardSize-2*board8-2*board32,board8,board8),arrow.mirrored(true,false));
            else
                painter->drawImage(QRect(pos+boardLeft -3*board8-2*board32,board8+2*board32,board8,board8),arrow);
        }

        painter->drawPixmap(boardLeft + boardSize+board32,boardSize-2*board8-board8/2,board8,board8,whitePiece);
        painter->drawEllipse(boardLeft + boardSize+board32,boardSize-2*board8-board8/2,board8,board8);
        painter->drawText(boardLeft + boardSize+board32,boardSize-board8-board32,boardSize,board8,Qt::AlignVCenter,p1);

        painter->drawPixmap(boardLeft -board8-board32,board8+board8/2,board8,board8,blackPiece);
        painter->drawEllipse(boardLeft -board8-board32,board8+board8/2,board8,board8);
        painter->drawText(0,+board32,boardLeft-board32,board8,Qt::AlignVCenter|Qt::AlignRight,p2);
    }


    for(int i=0; i<game->size; i++)
        for(int j=0; j<game->size; j++)
            switch(game->board[i][j])
            {
            default: continue;
            case White:
                painter->drawPixmap(boardLeft + i*cellSize, boardTop + j*cellSize, cellSize, cellSize,whitePiece);
                break;
            case Black:
                painter->drawPixmap(boardLeft + i*cellSize, boardTop + j*cellSize, cellSize, cellSize,blackPiece);
                break;
            }

    if(game->lastX >= 0)
    painter->drawEllipse(boardLeft + game->lastX*cellSize, boardTop + game->lastY*cellSize, cellSize, cellSize);

    Won won = game->IsOver();
    if(won)
    {
        if(won == BlackWon || won == WhiteWon)
        {
            QRect rect = game->GetPentePieces();
            QPen pen(Qt::red,cellSize/5);
            painter->setPen(pen);
            int x1 = boardLeft + rect.left()*cellSize + cellSize/2;
            int y1 = boardTop + rect.top()*cellSize + cellSize/2;
            int w = (rect.width())*cellSize;
            int h = (rect.height())*cellSize;
            painter->drawLine(x1,y1,x1+w,y1+h);
        }

        if(game->gameOverTimer.elapsed() >= 2000)
        {            
            QMessageBox::StandardButton result;
            if(multiplayer || won == WhiteWon)
            {
                QString p = game->turn == White? p2: p1;
                result = QMessageBox::question(myParent,
                        p + " won!!",
                        p + " won!!\n\nMatin thanks you for playing this game!!! :-)\n\nPlay again?",
                        QMessageBox::Yes|QMessageBox::No);
            }
            else if(won == Draw)
            {                
                result = QMessageBox::question(myParent,
                        "Draw!!",
                        "No one won!!\nHe he he he.... (OMRAN!!)\nMatin thanks you for playing this game!!! :-)\nPlay again?",
                        QMessageBox::Yes|QMessageBox::No);                
            }
            else
            {                
                result = QMessageBox::question(myParent,
                        "You lost!!",
                        "You lost!!\nHe he he he.... (OMRAN!!)\nMatin thanks you for playing this game!!! :-)\nPlay again?",
                        QMessageBox::Yes|QMessageBox::No);                
            }

            if(result == QMessageBox::Yes)
                game->NewGame();
            else
                afterFadeState=FirstMenu;
        }
    }
}

bool MainWindow::IsFadeIn()
{
    return drawState == afterFadeState && fade != 255;
}

bool MainWindow::IsFadeOut()
{
    return drawState != afterFadeState && fade != 0;
}

void MainWindow::paintEvent(QPaintEvent*e)
{
    if(isDrawing)
        return;
    QOpenGLWidget::paintEvent(e);
    isDrawing = true;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch(drawState)
    {
    case AboutMenu:
        aboutMenu->Draw(&painter,this);
        break;
    case FirstMenu:
        firstMenu->Draw(&painter,this);
        break;
    case SizeMenu:
        sizeMenu->Draw(&painter,this);
        break;
    case DifficultyMenu:
        difficultyMenu->Draw(&painter,this);
        break;
    case Play:
        DrawPlayGame(&painter);
        break;
    }

    if(IsFadeIn())
        DrawFadeIn(&painter);
    else if(IsFadeOut())
        DrawFadeOut(&painter);
    isDrawing = false;
}
