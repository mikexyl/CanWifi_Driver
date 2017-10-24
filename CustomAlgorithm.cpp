#include "CustomAlgorithm.h"
#include <iostream>
using namespace std;

CustomAlgorithm::CustomAlgorithm()
{

}

CustomAlgorithm::~CustomAlgorithm()
{

}

void CustomAlgorithm::issueEncoderCnt(encoder_data_t _encoder_data)
{
    encoder_data=_encoder_data;
    cout<<encoder_data.encoder_cnt_l<<endl;
}

void CustomAlgorithm::issueBatteryInfo(battery_info_t _battery_info)
{
    battery_info=_battery_info;
}

void CustomAlgorithm::issueNaviData(navi_data_t _navi_data)
{
    navi_data=_navi_data;
}

void CustomAlgorithm::run()
{
    resetVelData();
    while(1)
    {
//        emit queryEncoderData();
        //////////user algo/////

        msleep(20);
        /////////user algo end////
        //emit updateAlgoData(vel_data);
    }
}

void CustomAlgorithm::resetVelData()
{
    vel_data.vel_l=0;
    vel_data.vel_r=0;
}
