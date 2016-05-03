#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <game.h>
#include "menu.h"
#include "QTime"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();    
    int boardSize, boardTop, boardLeft;

public slots:
    void AIDone();    

private:    
    Game* game;    
    double cellSize;    
    Menu *difficultyMenu, *firstMenu, *sizeMenu, *aboutMenu;
    States drawState, afterFadeState;
    int fade;
    bool multiplayer;
    QString p1,p2;
    QTimer *drawTimer;
    QTime fadeTimer;

    void mousePressEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void DrawBackground(QPainter *painter);
    void DrawFadeOut(QPainter *painter);
    void DrawFadeIn(QPainter *painter);
    void DrawPlayGame(QPainter *painter);
};

#endif // MAINWINDOW_H
