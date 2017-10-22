/* Driver Class ver 1.0 */
#ifndef CANCONNECTORC_H
#define CANCONNECTORC_H
#include "ControlCAN.h"
#include <QThread>
#include <QObject>

#define VCI_CMD_SET_IP_REMOTE 0
#define VCI_CMD_SET_PORT_REMOTE 1
#define VCI_CMD_SET_PORT_LOCAL 2
#define VCI_CMD_SET_MODE_LOCAL 4
#define VCI_MODE_LOCAL_CLIENT 0
#define VCI_MODE_LOCAL_SERVER 1

#define AGV_CMD_TYPE_VEL 0x220

typedef struct
{
    uint32_t vel_l;
    uint32_t vel_r;
}vel_data_t;

class CanConnector:public QThread
{
    Q_OBJECT
public:
    CanConnector();
    ~CanConnector();

    void run();

    ////////Info///////
    /// \brief info params
    struct InfoParam{
        int deviceType;
        int deviceInd;
        int canInd;
        int canwifiPort;
        char canwifiIp[50];
        int reserved;
    }infoParams;

    ///////input Msgs////////
    /// \brief msg vars needed for algrithoms
    typedef struct
    {
        int     state;

        float   voltage;
        float   current;
        uint8_t power;          //0-100
        float   temperature;
    }battery_info_t;
    battery_info_t s_battery_info;

    typedef struct
    {
        float       x;
        float       y;
        float       angle;
        uint32_t    tag;

        uint16_t    warn;
    }navi_data_t;
    navi_data_t navi_data;

    ////////output Msgs/////////
    vel_data_t vel_data_algo;
    vel_data_t vel_data_real;

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
    bool startReceive(void);
    bool sendCmd(int CMD_TYPE, uint32_t param0, uint32_t param1);
    bool sendAlgoData();

signals:

public slots:
    bool resetAlgoData();
    bool updateAlgoData(vel_data_t new_vel_data_algo);

private:
    /////////init Dll//////////
    bool initDllFunc(void);
};

#endif // CANCONNECTOR_H
