#ifndef TUOLUO_H
#define TUOLUO_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string.h>
#include "base.h"
#include <iostream>

using namespace std;
using namespace cv;

class Tuoluo
{
public:
    Tuoluo();
    ~Tuoluo();
    bool start(size_t *time_count, size_t *tuoluo, size_t *tuoluo_count, bool *tuoluo_status, bool *tuoluo_start, double *sita_y, double *last_sita_y,
               deque<double> *sita);
    void iftuoluo();
    bool tuoluo_();

public:
    size_t time_count = 0;
    size_t tuoluo = 0;
    size_t tuoluo_count= 0;
    bool tuoluo_status = 0;
    bool tuoluo_start= 0;
    double sita_y= 0;
    double last_sita_y= 0;
    deque<double> sita ;
    double min= 99999;
    double max= 0;
    double avg_sita = 0;
};

#endif // TUOLUO_H
