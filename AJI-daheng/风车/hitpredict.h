/*
	预测打击点
	1.判断大小风车（通过转速
	2.若为大风车，拟合时间，进行预测
	3.若为小风车，直接预测
*/
#pragma once
#include"buffbase.h"
class hitpredict
{

public:
    hitpredict():tht(-1){}
	//void runSmall();
    void predict_smallbuff(Point2f CenterR, Point2f armor, Point2f& hit, int Rot,Base base1);//小能量机关预测
    void predict_bigbuff(Point2f CenterR, Point2f armor, double now_t, Point2f& hit, int Rot,Base base1);//大能量机关预测
	void predictTime();//拟合大能量机关时间
	void SmallorBig(Point2f armor1, Point2f armor2, double t1, double t2, Point2f CenterR);//判断大小能量机关
	void judgetht();
	double tht;

};

