#include"armorpredictrun.h"
#include"base.h"
#include "StoreDate.h"
#include"shootTuoLuo.h"
#include"ArmorPredict.h"
#include"staticpredict.h"
#include"distancepredict.h"


using namespace std;
using namespace cv;

bool yuc::run(Base *base1, Point2f *center,double *distance)//, Point2f p)
{
    Base base=*base1;
    StoreDate   data;
    ArmorPredict armorPredict;
    shootTuoLuo  shoottuoluo;
    RotatedRect rect = base.lastarmor;
    StaticPredict zxarmor;
    DistancePredict dispre;
    RotatedRect rect1;
    double distance2 = *distance;

    k = 0;

    //静止预测
    //运动预测
    if(data.judgenum(rect,&base))
    {
        if(zxarmor.armorstaticstate(base)){
            *center = zxarmor.armorstatic(base);
            data.cleardate(&base);
        }else{
             *center = armorPredict.armordistance(base);
             data.cleardate(&base);
        }
	return true;
    }

    //距离预测
    if(data.judgedisnum(distance2,&base)){
        distance2 = dispre.distancepredict(distance2,&base);
        data.cleardisdate(&base);
    }

    //陀螺预测
    if(!data.judgeTLnum(rect,&base)){
        if(shoottuoluo.StateRecognition(&base)){
        *center = shoottuoluo.TuoLuoPredict(&base);
        data.clearTLdate(&base);
        }
    }

        k = 1;
    *distance = distance2;
    base1->move_pre_rects = base.move_pre_rects;
    return false;      
}






