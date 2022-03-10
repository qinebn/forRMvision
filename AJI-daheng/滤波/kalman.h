#ifndef KALMAN_H
#define KALMAN_H
#include <iostream>
#include <Eigen/Dense>

using namespace std;
using Eigen::MatrixXd;

class kalman
{
public:
    kalman();
    ~kalman();
    void update();
    void predict();
    void getZ();
public:
    //更新参数
    MatrixXd E; //单位矩阵
    MatrixXd K; //卡尔曼增益
    MatrixXd P; //更新协方差
    MatrixXd X; //更新估计值
    MatrixXd R;

    //预测参数
    MatrixXd P_; //预测协方差
    MatrixXd X_; //预测估计值
    MatrixXd F;
    MatrixXd B;
    MatrixXd U; //输入X
    MatrixXd Q;

    //测量参数
    MatrixXd Z; //K时刻Z
    MatrixXd H;
    MatrixXd V;

};

#endif // KALMAN_H
