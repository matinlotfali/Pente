#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWidget>
#include <game.h>
#include "menu.h"
#include "QTime"
#include "QPixmap"

class MainForm;

namespace Ui {
class MainWindow;
}

class MainWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit MainWindow(MainForm *parent = 0);
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
    bool multiplayer, isDrawing;
    QString p1,p2;    
    QTime fadeTimer;
    MainForm* myParent;

    void mousePressEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*e);
    void DrawBackground(QPainter *painter);
    void DrawFadeOut(QPainter *painter);
    void DrawFadeIn(QPainter *painter);
    void DrawPlayGame(QPainter *painter);
    bool IsFadeIn();
    bool IsFadeOut();
};

#endif // MAINWINDOW_H
