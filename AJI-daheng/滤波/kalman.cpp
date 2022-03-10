#include "kalman.h"

kalman::kalman()
{

}

kalman::~kalman()
{

}

void kalman::update()
{
    MatrixXd a=P_*H.transpose();
    MatrixXd b=H*P_*H.transpose() + R;
    K = a*b.inverse();
    P = (E - K*H)*P_;
    X = X_ + K*(Z - H*X_);
  //  cout <<"X: "<< X << endl;
}

void kalman::predict()
{
    X_ = F*X + B*U;
    P_= F * P * F.transpose() + Q;
    cout <<"X_: "<< X_ << endl;
    cout <<"P_: "<< P_ << endl;
}

void kalman::getZ()
{

}


