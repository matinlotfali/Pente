#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QTimer>
#include <opengl.h>
#include <QResizeEvent>
#include <QSound>

class MainForm : public QWidget
{    
    Q_OBJECT

    QTimer *drawTimer;
    MainWindow *openGLWidget;
    QSound *soundOff, *soundOn;


    void resizeEvent(QResizeEvent *e);
public:
    explicit MainForm(QWidget *parent = 0);

signals:

public slots:
    void playSoundOn();
    void playSoundOff();
};

#endif // MAINFORM_H
