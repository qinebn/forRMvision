/*
	Ԥ������
	1.�жϴ�С�糵��ͨ��ת��
	2.��Ϊ��糵�����ʱ�䣬����Ԥ��
	3.��ΪС�糵��ֱ��Ԥ��
*/
#pragma once
#include"buffbase.h"
class hitpredict
{

public:
    hitpredict():tht(-1){}
	//void runSmall();
    void predict_smallbuff(Point2f CenterR, Point2f armor, Point2f& hit, int Rot,Base base1);//С��������Ԥ��
    void predict_bigbuff(Point2f CenterR, Point2f armor, double now_t, Point2f& hit, int Rot,Base base1);//����������Ԥ��
	void predictTime();//��ϴ���������ʱ��
	void SmallorBig(Point2f armor1, Point2f armor2, double t1, double t2, Point2f CenterR);//�жϴ�С��������
	void judgetht();
	double tht;

};

