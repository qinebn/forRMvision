#ifndef STOREDATE_H
#define STOREDATE_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include "base.h"

using namespace cv;
using namespace std;

class StoreDate
{
public:
    bool judgenum(RotatedRect& rect, Base *base);
     bool judgedisnum(double distance,Base *base);
      bool judgeTLnum(RotatedRect& rect,Base *base);

    void cleardate(Base *base);
      void cleardisdate(Base *base);
      void clearTLdate(Base *base);

};

#endif // STOREDATE_H

