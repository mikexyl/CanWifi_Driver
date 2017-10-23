#ifndef VARIABLES_H
#define VARIABLES_H
#include <QThread>
#include <QObject>
typedef struct
{
    uint32_t vel_l;
    uint32_t vel_r;
}vel_data_t;

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

typedef struct
{
    float       x;
    float       y;
    float       angle;
    uint32_t    tag;

    uint16_t    warn;
}navi_data_t;

typedef struct
{
    int encoder_cnt_l;
    int encoder_cnt_r;
}encoder_data_t;

#endif // VARIABLES_H
