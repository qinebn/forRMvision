#pragma once
#include <iostream>
#include"base.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class shootTuoLuo
{
	public:
        bool StateRecognition(Base *base);//�ж���С����״̬
        Point2f TuoLuoPredict(Base *base);//Ԥ��
		vector<RotatedRect> rects;

public:
        Point2f armor_pre;
};

