#include "staticpredict.h"

StaticPredict::StaticPredict()
{
    sum1 = 0;
    sum2 = 0;
}

StaticPredict::~StaticPredict()
{

}

bool StaticPredict::armorstaticstate(Base base) {
    float rectx[20]={0};
    float recty[20]={0};
    cout << "rectx[i]"<< "\n"<< endl;

    for (int i = 0; i < 3; i++) {
        rectx[i] = base.move_pre_rects[i].center.x;
        recty[i] = base.move_pre_rects[i].center.y;
        sum1 += rectx[i];
        sum2 += recty[i];
#ifdef yuce1
        cout << rectx[i]<< endl;
#endif
   }
#ifdef yuce1
    cout << "sum1:" << sum1 << "\n"<< endl;
#endif
    //只要有一张不符合就判断为不是静止
    for(int i=0;i<3;i++){
        if(fabs( sum1 /3-base.move_pre_rects[i].center.x)>2||(sum2 / 3-base.move_pre_rects[i].center.y)>4) return false;
    }
     return true;
}


Point2f StaticPredict::armorstatic(Base base) {
        armor_pre.x = sum1 / 3;
        armor_pre.y = sum2 / 3;
        armor_pre.x = fabs(armor_pre.x);
        armor_pre.y = fabs(armor_pre.y);
#ifdef yuce1
        cout << " staticx:" << armor_pre.x;
        cout << " staticy:" << armor_pre.y << endl;

        Mat src;
        base.src.copyTo(src);
        circle(src,Point(armor_pre.x ,armor_pre.y),5,Scalar(0,0,255));
        imshow("yuce1",src);
#endif
        return Point2f(armor_pre.x, armor_pre.y);

}
