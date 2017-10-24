#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEvent>
#include <QKeyEvent>
#include "canconnector.h"
#include <QMetaType>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<vel_data_t>("vel_data_t");
    qRegisterMetaType<uint32_t>("uint32_t");
    qRegisterMetaType<battery_info_t>("battery_info_t");
    qRegisterMetaType<navi_data_t>("navi_data_t");
    qRegisterMetaType<encoder_data_t>("encoder_cnt_t");

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerControler(CanConnector *controller)
{
    connect(this,SIGNAL(updateAlgoData(vel_data_t)),controller->canTransmitThread,SLOT(updateAlgoData(vel_data_t)));
    connect(controller->canReceiveThread,SIGNAL(showBattery(float)),this,SLOT(showBattery(float)));
    connect(controller->canReceiveThread,SIGNAL(showTagCode(uint32_t)),this,SLOT(showTagCode(uint32_t)));
    connect(controller->canReceiveThread,SIGNAL(showTagX(float)),this,SLOT(showTagX(float)));
    connect(controller->canReceiveThread,SIGNAL(showTagY(float)),this,SLOT(showTagY(float)));
    connect(controller->canReceiveThread,SIGNAL(showTagAngle(float)),this,SLOT(showTagAngle(float)));
    connect(controller->canReceiveThread,SIGNAL(showEncoderL(int)),this,SLOT(showEncoderL(int)));
    connect(controller->canReceiveThread,SIGNAL(showEncoderR(int)),this,SLOT(showEncoderR(int)));
    connect(this,SIGNAL(startRecordLog(QString)),controller->canReceiveThread,SLOT(startRecordLog(QString)));
    connect(this,SIGNAL(endRecordLog()),controller->canReceiveThread,SLOT(endRecordLog()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    ////move forward
    case Qt::Key_W:
        vel_data_algo.vel_l=1000;
        vel_data_algo.vel_r=1000;
        emit updateAlgoData(vel_data_algo);
        break;
    ////move backward
    case Qt::Key_S:
        //emit sendCmd(AGV_CMD_TYPE_VEL,-1000,-1000);
        break;
    ////turn left
    case Qt::Key_A:

        break;
    case Qt::Key_D:

        break;
    case Qt::Key_X:
        vel_data_algo.vel_l=0;
        vel_data_algo.vel_r=0;
        emit updateAlgoData(vel_data_algo);
        break;
    }
}

void MainWindow::on_forwardButton_clicked()
{
    vel_data_algo.vel_l=1000;
    vel_data_algo.vel_r=1000;
    emit updateAlgoData(vel_data_algo);
}

void MainWindow::on_leftwardButton_clicked()
{
    vel_data_algo.vel_l-=500;
    vel_data_algo.vel_r+=500;
    emit updateAlgoData(vel_data_algo);
}


void MainWindow::on_backwardButton_clicked()
{
    vel_data_algo.vel_l=-1000;
    vel_data_algo.vel_r=-1000;
    emit updateAlgoData(vel_data_algo);
}


void MainWindow::on_rightwardButton_clicked()
{
    vel_data_algo.vel_l+=500;
    vel_data_algo.vel_r-=500;
    emit updateAlgoData(vel_data_algo);
}

void MainWindow::showTagCode(uint32_t code)
{
    char text[50];
    sprintf(text,"TagCode:%d",code);

    ui->tagCodeEdit->setText(text);
    ui->tagCodeEdit->repaint();
}

void MainWindow::showTagX(float x)
{
    char text[50];
    sprintf(text,"TagX:%f",x);

    ui->tagXEdit->setText(text);
    ui->tagXEdit->repaint();
}

void MainWindow::showTagY(float y)
{
    char text[50];
    sprintf(text,"TagY:%f",y);

    ui->tagYEdit->setText(text);
    ui->tagYEdit->repaint();
}

void MainWindow::showTagAngle(float angle)
{
    char text[50];
    sprintf(text,"TagAngle:%f",angle);

    ui->tagAngleEdit->setText(text);
    ui->tagAngleEdit->repaint();
}

void MainWindow::showBattery(float battery)
{
    char text[50];
    sprintf(text,"Battery:%f",battery);

    ui->battreyEdit->setText(text);
    ui->battreyEdit->repaint();
}

void MainWindow::showEncoderL(int encoderL)
{
    char text[50];
    sprintf(text,"encoderL:%d",encoderL);

    ui->encoderLEdit->setText(text);
    ui->encoderLEdit->repaint();
}

void MainWindow::showEncoderR(int encoderR)
{
    char text[50];
    sprintf(text,"encoderR:%d",encoderR);

    ui->encoderREdit->setText(text);
    ui->encoderREdit->repaint();
}

void MainWindow::on_tagCodeLabel_linkActivated(const QString &link)
{

}

void MainWindow::on_stopButton_clicked()
{
    vel_data_algo.vel_l=0;
    vel_data_algo.vel_r=0;
    emit updateAlgoData(vel_data_algo);
}

void MainWindow::on_encoderButton_clicked()
{
    emit queryEncoderCnt();
}

void MainWindow::on_startRecordButton_clicked()
{
    emit startRecordLog(ui->xmlNameEdit->text());
}


void MainWindow::on_endButton_clicked()
{
    emit endRecordLog();
}
