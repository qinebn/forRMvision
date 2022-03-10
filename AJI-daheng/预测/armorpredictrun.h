#ifndef ARMORPREDICTRUN_H
#define ARMORPREDICTRUN_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include"ArmorPredict.h"
#include"shootTuoLuo.h"
#include"StoreDate.h"
#include "base.h"

using namespace cv;
using namespace std;

class yuc
{
public:
    bool run(Base *base, Point2f* center,double *distance);//, Point2f p);
    bool pd(int k);
public:
    int k = 0;
};

#endif // ARMORPREDICTRUN_H
