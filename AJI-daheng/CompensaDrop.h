#ifndef COMPENSADROP_H
#define COMPENSADROP_H

#endif // COMPENSADROP_H


using namespace std;
using namespace cv;

class CompensaDrop
{
public:

    bool launch(double distance，, Base* base);//是否启动补偿
    void CompensaDrop(double distance,double bulletspeed,double angle);
public:
    double dropheight;//子弹飞行过程下坠的高度
    double bulletspeed;//子弹速度
    double flytime;//子弹打出到击中目标的时间
    double angle;//敌方位置距离pitch的水平角度

}
