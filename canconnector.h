#ifndef CANCONNECTOR_H
#define CANCONNECTOR_H
#include "ControlCAN.h"
#include <QThread>
#include <QObject>
#include "CustomAlgorithm.h"
#include "vardef.h"
#include <QFile>
#include <QString>
#include <QTime>
#include <QDateTime>
#include "time.h"

#define VCI_CMD_SET_IP_REMOTE 0
#define VCI_CMD_SET_PORT_REMOTE 1
#define VCI_CMD_SET_PORT_LOCAL 2
#define VCI_CMD_SET_MODE_LOCAL 4
#define VCI_MODE_LOCAL_CLIENT 0
#define VCI_MODE_LOCAL_SERVER 1

#define AGV_CMD_TYPE_VEL 0x220
#define AGV_CMD_TYPE_ENC 0x230

class CustomAlgorithm;

class CanTransmitThread:public QThread
{
    Q_OBJECT
public:
    CanTransmitThread();
    ~CanTransmitThread();

    vel_data_t vel_data_algo;

    bool sendCmd(int CMD_TYPE,vel_data_t* param);
    bool sendAlgoData();
    bool resetAlgoData();
    void run();

public slots:
    bool updateAlgoData(vel_data_t new_vel_data_algo);
    bool queryEncoderData(void);
};

class CanReceiveThread:public QThread
{
    Q_OBJECT
public:
    CanReceiveThread();
    ~CanReceiveThread();

    battery_info_t s_battery_info;
    navi_data_t navi_data;
    encoder_data_t encoder_cnt;
    QString logName="default";
    QFile* log;
    bool writeLog(char* text);
public slots:
    void startRecordLog(QString _logName);
    void endRecordLog(void);
    void run();
signals:
    void showTagCode(uint32_t code);
    void showTagX(float x);
    void showTagY(float y);
    void showTagAngle(float angle);

    void showBattery(float battery);
    void showEncoderL(int encoderL);
    void showEncoderR(int encoderR);
    void issueEncoderCnt(encoder_data_t encoder_data);
    void issueBatteryInfo(battery_info_t _battery_info);
    void issueNaviData(navi_data_t _navi_data);

public slots:
};

class CanConnector:public QObject
{
    Q_OBJECT
public:
    CanConnector();
    ~CanConnector();

//    ////////output Msgs/////////
//    vel_data_t vel_data_real;

    void registerAlgo(CustomAlgorithm* algo);

    ////////functions to set info//////
    /// \brief infoSet funcs
    bool setDeviceType(int _deviceType);
    bool setDeviceInd(int _deviceInd);
    bool setCanInd(int _canInd);
    bool setCanWifiPort(int _canwifiPort);
    bool setCanWifiIp(char* _canwifiIp);

    ////////functions to operate CAN////
    /// \brief functions to operate CAN
    bool initCanWifi(void);
    bool startCAN(void);
    bool start(void);



    CanTransmitThread* canTransmitThread;

    CanReceiveThread* canReceiveThread;

private:
    /////////init Dll//////////
    bool initDllFunc(void);

};

#endif // CANCONNECTOR_H
