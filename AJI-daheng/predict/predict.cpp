
#include "predict.h"
#include "kalman.h"
#include "base.h"

Predict::Predict()
{
    E = MatrixXd(4,4);
    E <<1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1;
    H = MatrixXd(4,4);
    H << 1,0,0,0,
         0,1,0,0,
         0,0,1,0,
         0,0,0,1;
    Q = MatrixXd(4,4);
    /*Q << 1,0,0,0,
         0,1,0,0,
         0,0,1,0,
         0,0,0,1;*/
    Q << 1,0,0,0,
         0,1,0,0,
         0,0,1,0,
         0,0,0,1;
    V = MatrixXd(4,1);
    V << 0,
         0,
         0,
         0;
    R = MatrixXd(4,4);
    R << 100,0,0,0,
         0,100,0,0,
         0,0,800,0,
         0,0,0,800;
   /* R << 100,0,0,0,
         0,100,0,0,
         0,0,800,0,
         0,0,0,800;*/
    Z =  MatrixXd(4,1);
    Z << 0,
         0,
         0,
         0;
    F = MatrixXd(4,4);
    F << 1,0,t,0,
         0,1,0,t,
         0,0,1,0,
         0,0,0,1;
//    B = MatrixXd(4,2);
//    B << t*t/2,0,
//         0,t*t/2,
//         t,0,
//         0,t;
//    U = MatrixXd(2,1);
//    U << a_p,
//        a_y;
}

void Predict::pre_nov(Base *base, double last_pitch,double last_yaw)
{
    X = base->X;
    X(2,0) = 0;
    X(3,0) = 0;
    X_ = base->X_;
    X_(2,0) = 0;
    X_(3,0) = 0;
    K = base->K;
    P_ = base->P_;
    P = base->P;
    Z =  MatrixXd(4,1);
    Z << last_pitch,
         last_yaw,
         0,
         0;
    _predict_x = MatrixXd(4,1);
}

Predict::~Predict()
{

}

void Predict::get_matrixxd(Base *base)
{
    flag = base->pre_flag;
    if(0 == flag)
    {
        K = MatrixXd(4,4);
        K << 1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1;
        P = MatrixXd(4,4);
        P << 1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1;
        P_ = MatrixXd(4,4);
        P_ <<1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1;
        X = MatrixXd(4,1);
        X << sita_p,
             sita_y,
             v_p,
             v_y;

        X_ = MatrixXd(4,1);
        X_<< sita_p,
             sita_y,
             v_p,
             v_y;
        base->pre_flag = 1;
    }
    else
    {
        X = base->X;
        X_ = base->X_;
        K = base->K;
        P_ = base->P_;
        P = base->P;
    }
}

void Predict::get_number(double sita_p,double sita_y,double t)
{
    this->sita_p = sita_p;
    this->sita_y = sita_y;
    this->t = t;
    if(0 == last_sita_y) last_sita_y = this->sita_y;

    v_p = (this->sita_p - last_sita_p)/t;
    v_y = (this->sita_y - last_sita_y)/t;

#ifdef PRE_NUMBER
    cout << "t "<< this->t <<endl;
    cout << "sita_p" << this->sita_p << endl;
     cout << "last_sita_p" << last_sita_p<< endl;
    cout << "sita_y" << this->sita_y << endl;
     cout << "last_sita_y" << last_sita_y<< endl;
    cout << "v_p" << v_p << endl;
     cout << "v_y" << v_y<< endl;
#endif
}

void Predict::getZ()
{
    Z << sita_p,
         sita_y,
         v_p,
         v_y;
    F << 1,0,t,0,
         0,1,0,t,
         0,0,1,0,
         0,0,0,1;
}

void Predict::kpredict()
{
    X_ = F*X ;
    P_= F * P * F.transpose() +Q;
#ifdef PRE_KALMAN
//    cout <<"X_: "<< X_ << endl;
    cout <<"X: "<< X << endl;
    cout <<"X_: "<< X_ << endl;
    cout <<"F*X: "<< F*X << endl;
//    cout <<"P_: "<< P_ << endl;
#endif

}

void Predict::update()
{
    MatrixXd a=P_*H.transpose();
    MatrixXd b=H*P_*H.transpose() + R;
    K = a*b.inverse();//没更新
    P = (E - K*H)*P_;
    X = X_ + K*(Z - H*X_);
}

void Predict::start(double predict_t,double sita_p_,double sita_y_, double* last_pitch,
                    double* last_yaw, double* last_t, double *pre_count,Base *base,double *last_vp,double *last_vy, double *maoqiaqia)
{
    if(sita_y_ - 360 <= 0 && sita_y_ - 180 >= 0)
    {
        sita_y_ -= 360;
    }
//    if(PTZ_Pitch - 360 <= 0 && PTZ_Pitch - 180 >= 0)
//    {
//        PTZ_Pitch -= 360;
//    }
//    double PTZ_omiga_p = omiga_Pitch;
//    double PTZ_omiga_y = omiga_Yaw;
    double t = *last_t / 1000.0;
    count = *pre_count;
    last_sita_p = *last_pitch;
    last_sita_y = *last_yaw;
    last_v_p = *last_vp;
    last_v_y = *last_vy;

    if(sita_y_ > 360) sita_y_-=360;
    get_number(sita_p_, sita_y_, t);
    get_matrixxd(base);
    getZ();
    if(count > 2)
    {
        kpredict();
        predict_x(predict_t);
        update();
    }
    if(count  > 2)
   // if(count  > 10)
    {
        count = 80;
    }
    else
    {
        _pre_pitch = sita_p_;
       //_pre_yaw = Solver_sita_y;
        //  _pre_pitch = sita_p_;
          _pre_yaw = sita_y_ ;
    }
    count++;
    *pre_count = count ;
#ifdef PRE_NUMBER
    cout<<"all_sita_p_"<<sita_p_<<endl;
    cout<<"all_sita_y_"<<sita_y_<<endl;
    cout<<"_pre_pitch"<<_pre_pitch<<endl;
    cout<<"_pre_yaw "<<_pre_yaw <<endl;
#endif
    *last_pitch = sita_p_;
    *last_yaw = sita_y_;
    *last_vp = v_p;
    *last_vy = v_y;
    //*last_pitch = omiga_p;
    //*last_yaw = omiga_y;
    //last_t = t;
    base->X = X;
    base->X_ = X_;
    base->K = K;
    base->P_ = P_;
    base->P = P;
}

void Predict::predict_x(double predict_t)
{
    t = get_predict_time(predict_t);
#ifdef PRE_NUMBER
        cout<<"predict time: "<<predict_t<<endl;
#endif
    F << 1,0,t,0,
         0,1,0,t,
         0,0,1,0,
         0,0,0,1;
//    B << t*t/2,0,
//         0,t*t/2,
//         t,0,
//         0,t;


    //_pre_pitch = _predict_x(0,0)-PTZ_Pitch;
    //_pre_yaw = _predict_x(1,0)-PTZ_Yaw;
      //_pre_pitch = (PTZ_Pitch - X_(0,0));
      //_pre_yaw = (X_(1,0) -PTZ_Yaw) + X_(3,0)*t;
     // _pre_yaw = (X_(1,0)) + X_(3,0)*3*t;
    _pre_yaw = (X_(1,0)) + X_(3,0)*t*1.5;
   // _pre_yaw = (X_(1,0)) + v_y*t*10;
      if(_pre_yaw > 180) _pre_yaw-=360;
      if(_pre_yaw < -180) _pre_yaw+=360;
      if(abs(_pre_yaw) > 180) _pre_yaw = _pre_yaw = (X_(1,0));
     // cout<<"_pre_pitch"<<_pre_pitch<<endl;
     // cout<<"_pre_yaw"<<_pre_yaw<<endl;
}

double Predict::get_predict_time(double predict_t)
{
    return predict_t;
}

void Predict::ifmaoqiaqia(double sita_y_,double *maoqiaqia)
{
    if(abs(sita_y_ - last_sita_y) < 0.7) maoqiaqia++;
    else maoqiaqia = 0;
}

