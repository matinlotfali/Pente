#include "mainform.h"
#include <QApplication>

#if !defined(Q_OS_ANDROID)
#include <qdesktopwidget.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainForm w;

#if !defined(Q_OS_ANDROID)
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
#endif

    w.show();

    return a.exec();
}
