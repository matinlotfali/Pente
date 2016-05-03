#ifndef MENU_H
#define MENU_H

#include <qlist.h>
#include <qpainter.h>

class MainWindow;

enum States
{
    FirstMenu, SizeMenu, DifficultyMenu, Play, AboutMenu
};

class Menu
{
private:
    QString title, text;
    QList<QString> *menuList;

public:
    Menu(QString title, QList<QString> *menuList, QString text = "");
    void Draw(QPainter *painter, MainWindow *window);
    int SelectItem(int x, int y, MainWindow *window);
};

#endif // MENU_H
