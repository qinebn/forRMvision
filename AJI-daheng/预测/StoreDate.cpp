#include "StoreDate.h"
#include "base.h"

bool StoreDate::judgenum( RotatedRect & rect,Base *base)
{
    base->move_pre_rects.push_back(rect);
    cout<<base->move_pre_rects.size()<<endl;

    if (base->move_pre_rects.size() >= 4)
    {
        return true;
    }
  return false;
}
bool StoreDate::judgedisnum( double distance,Base *base){
     base->move_pre_dis.push_back(distance);
     if (base->move_pre_dis.size() >= 10)
     {
         return true;
     }
    return false;
}
bool StoreDate::judgeTLnum( RotatedRect & rect,Base *base){
    base->move_pre_TL.push_back(rect);
     if (base->move_pre_TL.size() >=25)
     {
         return true;
     }
    return false;
}



void StoreDate::cleardate(Base *base)
{
    base->move_pre_rects.clear();
}

void StoreDate::cleardisdate(Base *base)
{
    base->move_pre_dis.clear();
}

void StoreDate::clearTLdate(Base *base)
{
    base->move_pre_TL.clear();
}
