#include "lastarmor.h"

Lastarmor::Lastarmor()
{
}

Rect Lastarmor::lastarmor(Base& base )
{
    Rect roi;
    RotatedRect rect = base.lastarmor;

    float lx = rect.center.x - 2*rect.size.width;
    float ly = rect.center.y - 2*rect.size.height;
    float rx = rect.center.x + 3*rect.size.width;
    float ry = rect.center.y + 3*rect.size.height;
    if(lx <= 0)               lx = 1;
    if(ly <= 0)               ly = 1;
    if(rx > base.src.cols)    rx = base.src.cols;
    if(ry > base.src.rows)    ry = base.src.rows;
    roi = Rect(Point2f(lx  , ly ), Point2f(rx, ry));

    if (base.lost_cnt >= 15 || roi.x <= 0 || roi.y <= 0 || roi.width <=0 || roi.height <= 0)
    {
        base.lastarmor = RotatedRect(Point2f(0, 0), Size2f(base.src.cols, base.src.rows), 0);
        roi = Rect(0,0,base.src.cols, base.src.rows);
        //printf("lost:%d:\n", base.lost_cnt);
        printf("no lastarmor\n");
        return roi;
    }
    else
    {

        printf("lastarmor lu:%f %f rd:%f %f\n",lx,ly,rx,ry);


        return roi;
    }
}

/*Rect Lastarmor::lastarmor(Base& base )
{
    Rect roi;
    RotatedRect rect = base.lastarmor;
    if (rect.center.x < 200)
    {
        roi = Rect(0,0,200,base.src.rows);
        base.last_roi = 0;
    }
    else if(rect.center.x < 600)
    {
        roi = Rect(200,0,400,base.src.rows);
        base.last_roi = 1;
    }
    else
    {
        roi = Rect(600,0,base.src.cols - 600 ,base.src.rows);
        base.last_roi = 2;
    }
}*/
