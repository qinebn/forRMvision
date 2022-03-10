#pragma once
#include"StoreDate.h"
#include"base.h"
#include<Eigen/Dense>

using namespace cv;
using namespace std;
using Eigen::MatrixXd;


class ArmorPredict
{
public:
    Point2f armordistance(Base base);
public:
    MatrixXd X_now;
    MatrixXd Y_now;
    MatrixXd Get1_now;
    MatrixXd Z_now;
    MatrixXd D_now;
    MatrixXd Get2_now;

    double a1=0;
    double b1=0;
    double c1=0;
    double a2=0;
    double b2=0;
    double c2=0;

    double dispre;
    double distance_x;
    double distance_y;


    Point2f armor_pre;

};

