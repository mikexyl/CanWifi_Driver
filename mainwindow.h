#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canconnector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    vel_data_t vel_data_algo;

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event);

signals:
    bool updateAlgoData(vel_data_t new_vel_data_algo);
private slots:

    void on_forwardButton_clicked();
    void on_leftwardButton_clicked();
    void on_backwardButton_clicked();
    void on_rightwardButton_clicked();
};

#endif // MAINWINDOW_H
