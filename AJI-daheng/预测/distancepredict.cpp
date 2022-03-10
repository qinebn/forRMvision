#include "distancepredict.h"
#include "math.h"


double DistancePredict::distancepredict(double distance,Base *base)
{
    double distan[20]={0};
       double zh[20]={1,2,3,4,5,6,7,8,9,10,11};
       double z=0,dis=0;//平均值
       double z_ = 0,dis_ = 0;
       double a = 0, b = 0;//拟合直线的系数
       double a_ = 0, b_ = 0;
       double zs=0;//用于预测时用的变量
       double sum1=0,sum2=0,sum3=0,sum4=0;
       double sum1_=0,sum2_=0,sum3_=0,sum4_=0;


       //前五帧
          for (int i = 0; i < 5; i++) {
              distan[i] = base->move_pre_dis[i];
              sum1 += zh[i];
              sum2 += distan[i];
              z = sum1 / 5;
              dis = sum2 / 5;
          }
          for (int i = 0; i < 5; i++) {
              sum3 += distan[i] * zh[i];
              sum4 += zh[i] *  zh[i];
          }
          a = (sum3 - 5 * z*dis) / (sum4 - 5 * z*z);
          b = dis - a * z;
          //后五帧
              for (int i = 5; i < 10; i++) {
                  distan[i] = base->move_pre_dis[i];
                  sum1_ += zh[i];
                  sum2_ += distan[i];
                  z_ = sum1_ / 5;
                  dis_ = sum2_ / 5;
              }
              for (int i = 5; i < 10; i++) {
                  sum3_ += distan[i] * zh[i];
                  sum4_ += zh[i] * zh[i];
              }
              a_ = (sum3_ - 5 * z_*dis_) / (sum4_ - 5 * z_*z_);
              b_ = dis_ - a_ * z_;


              //开始拟合
               if(fabs(a-a_)<0.5){
                   zs=11;
                   distance_pre=(a+a_)/2*z+(b+b_);

               }else{
                   zs=11;
                   distance_pre=a_*z+b_;
               }
               return distance_pre;

}
