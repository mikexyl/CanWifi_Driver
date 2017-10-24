#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canconnector.h"
#include "CustomAlgorithm.h"
#include "vardef.h"

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

    void registerControler(CanConnector* controller);
private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event);

signals:
    bool updateAlgoData(vel_data_t new_vel_data_algo);
    bool queryEncoderCnt();
    void startRecordLog(QString _logName);
    void endRecordLog(void);
private slots:

    void on_forwardButton_clicked();
    void on_leftwardButton_clicked();
    void on_backwardButton_clicked();
    void on_rightwardButton_clicked();

    void showTagCode(uint32_t code);
    void showTagX(float x);
    void showTagY(float y);
    void showTagAngle(float angle);

    void showBattery(float battery);
    void showEncoderL(int encoderL);
    void showEncoderR(int encoderR);
    void on_tagCodeLabel_linkActivated(const QString &link);
    void on_stopButton_clicked();
    void on_encoderButton_clicked();

    void on_startRecordButton_clicked();
    void on_endButton_clicked();
};

#endif // MAINWINDOW_H
