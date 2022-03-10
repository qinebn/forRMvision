#ifndef THREAD_H
#define THREAD_H

#include "cstdio"
#include "base.h"

class Threads
{
public:
    Threads();
    ~Threads();
    void camera_thread();
    void image_thread();
    void change_exposure(Base &base);
    void reset_kalman(Base *base);
    void get_prev();
    void avgsita(double* yaw, double* pitch);

public:
    size_t camera_count;
    size_t image_count;
    Base base;
    double dExposureValue = 2000.0;
    size_t exposure_low = 0;
    size_t exposure_high = 0;
    size_t exposure_type1_count = 0;
    bool pre_flag = 0;
    Point2f center;
    size_t port_count = 0;
    size_t video_flag = 1;
    size_t shaobing = 0;
    VideoWriter video;
    RotatedRect prev_armor;

    //kalman
    double last_pitch = 0;
    double last_yaw = 0;
    double last_t = 0;
    double pre_count = 0;
    double pre_pitch = 0;
    double pre_yaw = 0;
    size_t v_y_count = 0;
    size_t back = 0;
    double last_vp = 0;
    double last_vy = 0;
    double maoqiaqia = 0;
    double pre_t = 0;
    //kalman

    //tuoluo
    size_t time_count = 0;
    size_t tuoluo= 0;
    size_t tuoluo_count= 0;
    bool tuoluo_status= 0;
    bool tuoluo_start= 0;
    double last_sita_y = 0;
//    double armor_no = 0;

    //avg
    size_t avg_count = 0;
    deque<double> sita_yaw_avg;
    deque<double> sita_pitch_avg;
    double avg_time = 0;
    double move_yaw = 0;
    double move_pitch = 0;
    double all_yaw = 0;
    double all_pitch = 0;
};

#endif // THREAD_H
