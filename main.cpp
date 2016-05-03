#include "mainwindow.h"
#include <QApplication>

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_WIN32)
#include <qdesktopwidget.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_WIN32)
    w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
#endif

    w.show();

    return a.exec();
}
