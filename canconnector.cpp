#include "canconnector.h"
#include <iostream>
#include <windows.h>
#include <QObject>

using namespace std;

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

#define SCANNER_CANID 0x08
#define BATTERY_CANID 0x10
#define SAGV_CANID 0x07
#define PC_CANID 0x09

#define ENCODER_CANID 0x110

typedef DWORD (__stdcall*  LPVCI_OpenDevice)(DWORD,DWORD,DWORD);
typedef DWORD (__stdcall*  LPVCI_CloseDevice)(DWORD,DWORD);
typedef DWORD (__stdcall*  LPVCI_InitCan)(DWORD,DWORD,DWORD,PVCI_INIT_CONFIG);

typedef DWORD (__stdcall*  LPVCI_ReadBoardInfo)(DWORD,DWORD,PVCI_BOARD_INFO);
typedef DWORD (__stdcall*  LPVCI_ReadErrInfo)(DWORD,DWORD,DWORD,PVCI_ERR_INFO);
typedef DWORD (__stdcall*  LPVCI_ReadCanStatus)(DWORD,DWORD,DWORD,PVCI_CAN_STATUS);

typedef DWORD (__stdcall*  LPVCI_GetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);
typedef DWORD (__stdcall*  LPVCI_SetReference)(DWORD,DWORD,DWORD,DWORD,PVOID);

typedef ULONG (__stdcall*  LPVCI_GetReceiveNum)(DWORD,DWORD,DWORD);
typedef DWORD (__stdcall*  LPVCI_ClearBuffer)(DWORD,DWORD,DWORD);

typedef DWORD (__stdcall*  LPVCI_StartCAN)(DWORD,DWORD,DWORD);
typedef DWORD (__stdcall*  LPVCI_ResetCAN)(DWORD,DWORD,DWORD);

typedef ULONG (__stdcall*  LPVCI_Transmit)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ,ULONG);
typedef ULONG (__stdcall*  LPVCI_Receive)(DWORD,DWORD,DWORD,PVCI_CAN_OBJ,ULONG,INT);

LPVCI_OpenDevice VCI_OpenDevice;
LPVCI_CloseDevice VCI_CloseDevice;
LPVCI_InitCan VCI_InitCAN;
LPVCI_ReadBoardInfo VCI_ReadBoardInfo;
LPVCI_ReadErrInfo VCI_ReadErrInfo;
LPVCI_ReadCanStatus VCI_ReadCanStatus;
LPVCI_GetReference VCI_GetReference;
LPVCI_SetReference VCI_SetReference;
LPVCI_GetReceiveNum VCI_GetReceiveNum;
LPVCI_ClearBuffer VCI_ClearBuffer;
LPVCI_StartCAN VCI_StartCAN;
LPVCI_ResetCAN VCI_ResetCAN;
LPVCI_Transmit VCI_Transmit;
LPVCI_Receive VCI_Receive;

CanConnector::CanConnector()
{
    infoParams.deviceType=VCI_WIFICAN_TCP;
    infoParams.deviceInd=0;
    infoParams.canInd=0;
    infoParams.canwifiPort=4001;
    infoParams.reserved=0;
    strcpy(infoParams.canwifiIp,"192.168.51.178");

    canTransmitThread=new CanTransmitThread();
    canReceiveThread=new CanReceiveThread();
}

CanConnector::~CanConnector()
{
    DWORD dwRel;
    dwRel=VCI_CloseDevice(infoParams.deviceType,
                          infoParams.deviceInd);
    if(dwRel=STATUS_OK) cout<<"disconnected"<<endl;
}

void CanConnector::registerAlgo(CustomAlgorithm *algo)
{

    connect(this->canReceiveThread,SIGNAL(issueEncoderCnt(encoder_data_t)),
            algo,SLOT(issueEncoderCnt(encoder_data_t)));
    connect(this->canReceiveThread,SIGNAL(issueBatteryInfo(battery_info_t)),
            algo,SLOT(issueBatteryInfo(battery_info_t)));
    connect(this->canReceiveThread,SIGNAL(issueNaviData(navi_data_t)),
            algo,SLOT(issueNaviData(navi_data_t)));
    connect(algo,SIGNAL(queryEncoderData()),
            this->canTransmitThread,SLOT(queryEncoderData()));
    QObject::connect(algo,SIGNAL(updateAlgoData(vel_data_t)),
            this->canTransmitThread,SLOT(updateAlgoData(vel_data_t)));

}

bool CanConnector::setDeviceType(int _deviceType)
{
    infoParams.deviceType=_deviceType;
}

bool CanConnector::setDeviceInd(int _deviceInd)
{
    infoParams.deviceInd=_deviceInd;
}

bool CanConnector::setCanInd(int _canInd)
{
    infoParams.canInd=_canInd;
}

bool CanConnector::setCanWifiPort(int _canwifiPort)
{
    infoParams.canwifiPort=_canwifiPort;
}

bool CanConnector::setCanWifiIp(char *_canwifiIp)
{
    strcpy(infoParams.canwifiIp,_canwifiIp);
}

bool CanConnector::initCanWifi()
{
    initDllFunc();

    DWORD dwRel;
    if(VCI_OpenDevice!=NULL){
        dwRel = VCI_OpenDevice(infoParams.deviceType, infoParams.deviceInd, infoParams.reserved);
    }
    else cout<<"cannot load VCI_OpenDevice"<<endl;

    if(VCI_SetReference!=NULL){
        dwRel=VCI_SetReference(infoParams.deviceType,
                               infoParams.deviceInd,
                               infoParams.canInd,
                               VCI_CMD_SET_IP_REMOTE,(void*)infoParams.canwifiIp);
        if(dwRel==STATUS_OK)
            cout<<"set ip ok"<<endl;
        else cout<<"set ip not ok"<<endl;

        dwRel=VCI_SetReference(infoParams.deviceType,
                               infoParams.deviceInd,
                               infoParams.canInd,
                               VCI_CMD_SET_PORT_REMOTE,(void*)&infoParams.canwifiPort);
        if(dwRel==STATUS_OK)
            cout<<"set port ok"<<endl;
        else cout<<"set port not ok"<<endl;
    }
    else cout<<"cannot load VCI_SetReference"<<endl;
}

bool CanConnector::startCAN()
{
    DWORD dwRel;
    if(VCI_StartCAN!=NULL){
        dwRel = VCI_StartCAN(infoParams.deviceType, infoParams.deviceInd, infoParams.canInd);
        if(dwRel==STATUS_OK)
            cout<<"start can0 ok"<<endl;
        else cout<<"start can0 not ok"<<endl;
    }
    else cout<<"cannot load VCI_StartCAN"<<endl;
}

void _reverse_copy(const void*src, void*dest, uint32_t len)
{
    int   i;
    for(i=0; i<len; i++)
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[len-1-i];
}

bool CanConnector::start()
{
    initCanWifi();
    startCAN();
   canTransmitThread->start();
   canReceiveThread->start();
}

bool CanTransmitThread::sendCmd(int CMD_TYPE, vel_data_t *param)
{
    VCI_CAN_OBJ transmitData;

    transmitData.ID=PC_CANID|CMD_TYPE;
    transmitData.SendType=0;
    transmitData.RemoteFlag=0;
    transmitData.ExternFlag=0;
    transmitData.DataLen=8;

    memcpy(&transmitData.Data[0],param,2*sizeof(int));
    DWORD dwRel=VCI_Transmit(infoParams.deviceType,
                             infoParams.deviceInd,
                             infoParams.canInd,
                             &transmitData,1);

    return (bool)dwRel;
}

bool CanTransmitThread::sendAlgoData()
{
    sendCmd(AGV_CMD_TYPE_VEL,&vel_data_algo);
}

bool CanTransmitThread::resetAlgoData()
{
    vel_data_algo.vel_l=0;
    vel_data_algo.vel_r=0;
}

bool CanTransmitThread::updateAlgoData(vel_data_t new_vel_data_algo)
{
    vel_data_algo=new_vel_data_algo;
    cout<<"vel_algo:"<<vel_data_algo.vel_l<<","<<vel_data_algo.vel_r<<endl;
    sendAlgoData();
}

bool CanTransmitThread::queryEncoderData()
{
    vel_data_t tmp;
    sendCmd(AGV_CMD_TYPE_ENC,&tmp);
}

bool CanConnector::initDllFunc()
{
    HMODULE m_hDLL;
    LPCWSTR lib=TEXT("ControlCAN.dll");
    m_hDLL = LoadLibrary(lib);
    if(m_hDLL==NULL){
        cout<<"cannot load ControlCAN.dll"<<endl;
    }
    VCI_OpenDevice=(LPVCI_OpenDevice)GetProcAddress(m_hDLL,"VCI_OpenDevice");
    VCI_CloseDevice=(LPVCI_CloseDevice)GetProcAddress(m_hDLL,"VCI_CloseDevice");
    VCI_InitCAN=(LPVCI_InitCan)GetProcAddress(m_hDLL,"VCI_InitCAN");
    VCI_ReadBoardInfo=(LPVCI_ReadBoardInfo)GetProcAddress(m_hDLL,"VCI_ReadBoardInfo");
    VCI_ReadErrInfo=(LPVCI_ReadErrInfo)GetProcAddress(m_hDLL,"VCI_ReadErrInfo");
    VCI_ReadCanStatus=(LPVCI_ReadCanStatus)GetProcAddress(m_hDLL,"VCI_ReadCANStatus");
    VCI_GetReference=(LPVCI_GetReference)GetProcAddress(m_hDLL,"VCI_GetReference");
    VCI_SetReference=(LPVCI_SetReference)GetProcAddress(m_hDLL,"VCI_SetReference");
    VCI_GetReceiveNum=(LPVCI_GetReceiveNum)GetProcAddress(m_hDLL,"VCI_GetReceiveNum");
    VCI_ClearBuffer=(LPVCI_ClearBuffer)GetProcAddress(m_hDLL,"VCI_ClearBuffer");
    VCI_StartCAN=(LPVCI_StartCAN)GetProcAddress(m_hDLL,"VCI_StartCAN");
    VCI_ResetCAN=(LPVCI_ResetCAN)GetProcAddress(m_hDLL,"VCI_ResetCAN");
    VCI_Transmit=(LPVCI_Transmit)GetProcAddress(m_hDLL,"VCI_Transmit");
    VCI_Receive=(LPVCI_Receive)GetProcAddress(m_hDLL,"VCI_Receive");
}

CanTransmitThread::CanTransmitThread()
{

}

CanTransmitThread::~CanTransmitThread()
{

}

void CanTransmitThread::run()
{
    while(1)
    {
//        vel_data_algo=new_vel_data_algo;
//        cout<<"vel_algo:"<<vel_data_algo.vel_l<<","<<vel_data_algo.vel_r<<endl;
        sendAlgoData();
        msleep(20);
    }
}

CanReceiveThread::CanReceiveThread()
{

}

CanReceiveThread::~CanReceiveThread()
{

}

void CanReceiveThread::run()
{
    VCI_CAN_OBJ receiveData[50];
    VCI_ERR_INFO errInfo;
    int len,i;
    int n=0;
    while(1)
    {
//        Sleep(1);
//        cout<<"receiving"<<n++<<endl;


        len=VCI_Receive(infoParams.deviceType,
                        infoParams.deviceInd,
                        infoParams.canInd,
                        receiveData,1,10);
        if(len<=0){
            VCI_ReadErrInfo(infoParams.deviceType,
                            infoParams.deviceInd,
                            infoParams.canInd,
                            &errInfo);
        }
        else {
//            Sleep(1);
            for(i=0;i<len;i++){
                VCI_CAN_OBJ msg=receiveData[i];
                if( msg.ExternFlag!=0
                        ||msg.RemoteFlag!=0)
                    break;

                ///////bms msgs////////
                if(msg.ID==0x10)
                {
                    uint16_t tmp;
                    tmp = ((uint16_t)msg.Data[0])<<8 | msg.Data[1];
                    //cout<<"battery: "<<tmp/10.0<<"v"<<endl;

                    tmp = ((uint16_t)msg.Data[2])<<8 | msg.Data[3];
                    s_battery_info.current = tmp/10.0;

                    s_battery_info.power = msg.Data[4];
                    s_battery_info.temperature= msg.Data[5]-40;
                    s_battery_info.state = msg.Data[6];

                    emit showBattery(s_battery_info.power);
                }


                ///////navigation msgs///
                int ret=0;
                int32_t temp32=0;
                uint16_t temp16=0;
                uint16_t twoD_TAG=0;

                switch(msg.ID)
                {
                  //收到二维码的TxPDO1
                  //case 0x188:
                  case 0x180|SCANNER_CANID:
                      _reverse_copy(&msg.Data[0], &temp32, 4);
                      navi_data.y = temp32 * 0.1f;
                      //cout<<"y:"<<navi_data.y<<endl;
                      emit showTagY(navi_data.y);
                      break;

                  //收到二维码的TxPDO2
                  //case 0x288:
                  case 0x280|SCANNER_CANID:
                      _reverse_copy(&msg.Data[0], &temp32, 4);
                      _reverse_copy(&msg.Data[4], &temp16, 2);
                      navi_data.x = temp32*0.1f;
                      navi_data.angle = temp16*0.1f;
                      //cout<<"x:"<<navi_data.x<<endl;
                      //cout<<"angle:"<<navi_data.angle<<endl;
                      emit showTagX(navi_data.x);
                      emit showTagAngle(navi_data.angle);
                      break;

                  //收到二维码的TxPDO3
                  //case 0x388:
                  case 0x380|SCANNER_CANID:
                      twoD_TAG = (msg.Data[0]>>3)&0x01;
                      if(twoD_TAG == 0x1)  // ?twoD_TAG
                      {
                          //APP_TRACE("new tag\r\n");
                          _reverse_copy(&msg.Data[2], &navi_data.warn, 2);
                          _reverse_copy(&msg.Data[4], &navi_data.tag, 4);
                      }
                      //cout<<"tag:"<<navi_data.tag<<endl;
                      emit showTagCode(navi_data.tag);
                      break;

                  //case 0x488:
                  case 0x480|SCANNER_CANID:
                    break;

                  ///case 0x588:
                  case 0x580|SCANNER_CANID:
                    break;

                }


                ////////test msgs///////
                switch(msg.ID)
                {
                case SAGV_CANID|ENCODER_CANID:
                    for(int i=0;i<8;i++){
                        cout<<hex<<(unsigned short)msg.Data[i]<<ends;
                    }
                    cout<<endl;
                    memcpy(&encoder_cnt.encoder_cnt_l,&msg.Data[0],2*sizeof(int));
                    cout<<hex<<(int)encoder_cnt.encoder_cnt_l<<endl;
                    cout<<encoder_cnt.encoder_cnt_l<<ends<<encoder_cnt.encoder_cnt_r<<endl;
                    for(int i=0;i<8;i++){
                        cout<<hex<<(unsigned short)msg.Data[i]<<ends;
                    }
                    cout<<endl;
                    emit showEncoderL(encoder_cnt.encoder_cnt_l);
                    emit showEncoderR(encoder_cnt.encoder_cnt_r);
                    break;
                }
            }

        }
    }
}
