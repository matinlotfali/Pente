#include "mainform.h"
#include "QtMultimedia/QSound"
#define DRAW_INTERVAL 15

MainForm::MainForm(QWidget *parent) : QWidget(parent)
{
#if !defined(Q_OS_ANDROID)
    resize(400,600);
#endif

    openGLWidget = new MainWindow(this);
    openGLWidget->setFixedSize(size());

    drawTimer = new QTimer(this);
    connect(drawTimer,SIGNAL(timeout()),openGLWidget,SLOT(update()));
    drawTimer->start(DRAW_INTERVAL);

    soundOff = new QSound(":/sounds/Speech Off.wav",this);
    soundOn = new QSound(":/sounds/Speech On.wav",this);
}

void MainForm::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    openGLWidget->setFixedSize(size());
}

void MainForm::playSoundOff()
{
    soundOff->play();
}

void MainForm::playSoundOn()
{    
    soundOn->play();
}
