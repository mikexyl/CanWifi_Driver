#ifndef CUSTOMALGORITHM_H
#define CUSTOMALGORITHM_H

#include <QObject>
#include "canconnector.h"
#include "vardef.h"


class CustomAlgorithm:public QThread
{
    Q_OBJECT
public:
    CustomAlgorithm();
    ~CustomAlgorithm();
    void run();

    vel_data_t vel_data;
    navi_data_t navi_data;
    battery_info_t battery_info;
    encoder_data_t encoder_data;

    void resetVelData(void);
signals:
    void queryEncoderData();
    void updateAlgoData(vel_data_t _vel_data);
private slots:
    void issueEncoderCnt(encoder_data_t _encoder_data);
    void issueBatteryInfo(battery_info_t _battery_info);
    void issueNaviData(navi_data_t _navi_data);
};

#endif // CUSTOMALGORITHM_H
