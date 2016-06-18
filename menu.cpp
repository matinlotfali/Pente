#include "menu.h"
#include "opengl.h"

QImage* Menu::icon = NULL;
QFont* Menu::font = NULL;
QBrush* Menu::orange = NULL;

Menu::Menu(QString title, QList<QString> *menuList, QString text)
{
    this->menuList = menuList;
    this->title = title;
    this->text = text;

    if(!icon)
        icon = new QImage(":/images/others/icon512.png");
    if(!font)
        font = new QFont("Tahoma");
    if(!orange)
        orange = new QBrush("orange");
}

void Menu::Draw(QPainter *painter, MainWindow *window)
{
    int w = window->width();
    int h = window->height();
    QRect rect = window->rect();            
    font->setPixelSize(window->boardSize/16);

    painter->setFont(*font);
    painter->setPen(Qt::white);
    painter->setBrush(*orange);
    painter->fillRect(rect,Qt::black);

    if(w<h)
    {
        int iw = w/2;
        int ih = h/4;
        int is = iw<ih? iw: ih;
        painter->drawImage(QRect(w/4+ (w/2-is)/2, h/8 + (h/4-is)/2, is, is), *icon);

        painter->drawText(
                    0,
                    h/2,
                    w,
                    h/16,
                    Qt::AlignVCenter | Qt::AlignHCenter,title);

        painter->drawText(
                    0,
                    h/2,
                    w,
                    h/2,
                    Qt::AlignVCenter | Qt::AlignHCenter,text);

        painter->setPen(Qt::black);
        for(int i=0; i<menuList->length(); i++)
        {
            painter->drawEllipse(
                        w/8,
                        h/2 + h*(i+1)/16 + h*(i+1)/32,
                        h/16,h/16);

            painter->drawEllipse(
                        w - w/8 - h/16,
                        h/2 + h*(i+1)/16 + h*(i+1)/32,
                        h/16,h/16);

            painter->fillRect(
                        w/8+h/32,
                        h/2 + h*(i+1)/16 + h*(i+1)/32,
                        w - 2*w/8 - h/16,
                        h/16,
                        *orange);

            painter->drawText(
                        w/8,
                        h/2 + h*(i+1)/16 + h*(i+1)/32,
                        w - 2*w/8,
                        h/16,
                        Qt::AlignVCenter | Qt::AlignHCenter,menuList->at(i));
        }
    }
    else
    {
        int iw = w/4;
        int ih = h/2;
        int is = iw<ih? iw: ih;
        painter->drawImage(QRect(w/8+ (w/4-is)/2, h/4 + (h/2-is)/2, is, is), *icon);

        painter->drawText(
                    w/2,
                    h/16,
                    w/2 - w/16,
                    h/16,
                    Qt::AlignVCenter | Qt::AlignHCenter,title);

        painter->drawText(
                    w/2,
                    0,
                    w/2,
                    h,
                    Qt::AlignVCenter | Qt::AlignHCenter,text);

        painter->setPen(Qt::black);
        for(int i=0; i<menuList->length(); i++)
        {
            painter->drawEllipse(
                        w/2,
                        h*(i+2)/16 + h*(i+1)/32,
                        h/16,h/16);

            painter->drawEllipse(
                        w -w/16- h/16,
                        h*(i+2)/16 + h*(i+1)/32,
                        h/16,h/16);

            painter->fillRect(
                        w/2 + h/32,
                        h*(i+2)/16 + h*(i+1)/32,
                        w/2 - w/16 - h/16,
                        h/16,
                        *orange);

            painter->drawText(
                        w/2,
                        h*(i+2)/16 + h*(i+1)/32,
                        w/2 - w/16,
                        h/16,
                        Qt::AlignVCenter | Qt::AlignHCenter,menuList->at(i));
        }
    }
}

int Menu::SelectItem(int x, int y, MainWindow *window)
{
    int w = window->width();
    int h = window->height();

    if(w<h)
    {
        if(x>w/8 && x<w-w/8)
        {
            for(int i=0; i<menuList->length(); i++)
                if(y>h/2 + h*(i+1)/16 + h*(i+1)/32 && y<h/2 + h*(i+1)/16 + h*(i+1)/32+h/16)
                    return i;
        }
    }
    else
        if(x>w/2 && x<w-w/16)
        {
            for(int i=0; i<menuList->length(); i++)
                if(y>h*(i+2)/16 + h*(i+1)/32 && y<h*(i+2)/16 + h*(i+1)/32+h/16)
                    return i;
        }

    return -1;
}
