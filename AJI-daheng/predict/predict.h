#ifndef PREDICT_H
#define PREDICT_H

#include<iostream>
#include <Eigen/Dense>
#include"kalman.h"
#include"base.h"
using namespace std;

class Predict : public kalman
{
public:
    Predict();
    ~Predict();
    void pre_nov(Base *base, double last_pitch,double last_yaw);
    void get_number(double sita_p,double sita_y,double t);
    void kpredict();
    void getZ();
    void update();
    void start(double predict_t,double sita_p_,double sita_y_,double* last_pitch,
               double* last_yaw, double* last_t, double *pre_count,Base *base,double *last_vp,double *last_vy, double *maoqiaqia);
    void get_matrixxd(Base *base);
    void predict_x(double predict_t);
    double get_predict_time(double predict_t);
    void ifmaoqiaqia(double sita_y_,double *maoqiaqia);

private:
    double t = 10;
    double sita_p = 0;
    double sita_y = 0;
//    double omiga_p = 0;
//    double omiga_y = 0;
    double a_p = 0;
    double a_y = 0;
    double last_sita_p = 0;
    double last_sita_y = 0;
    double last_v_p = 0;
    double last_v_y = 0;
    double last_t = 0;
    int count = 0;

public:
    MatrixXd _predict_x;
    double _pre_pitch = 0;
    double _pre_yaw = 0;
    bool flag = 0;
    double v_p = 0;
    double v_y = 0;

};






#endif // PREDICT_H
