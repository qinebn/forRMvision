#ifndef STATICPREDICT_H
#define STATICPREDICT_H
#include"base.h"

using namespace cv;
using namespace std;

class StaticPredict
{
public:
    StaticPredict();
    ~StaticPredict();
    bool armorstaticstate(Base base);
    Point2f armorstatic(Base base);

public:
    Point2f armor_pre;
    float sum1, sum2;
};

#endif // STATICPREDICT_H
