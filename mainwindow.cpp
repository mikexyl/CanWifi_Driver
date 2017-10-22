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

    ui->setupUi(this);

    CanConnector* sAGVControler=new CanConnector();
    sAGVControler->start();

    connect(this,SIGNAL(updateAlgoData(vel_data_t)),sAGVControler,SLOT(updateAlgoData(vel_data_t)));
}

MainWindow::~MainWindow()
{
    delete ui;
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

}


void MainWindow::on_backwardButton_clicked()
{

}


void MainWindow::on_rightwardButton_clicked()
{

}
