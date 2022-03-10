#pragma once
#include <iostream>
#include"base.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class shootTuoLuo
{
	public:
        bool StateRecognition(Base *base);//ÅÐ¶ÏÊÇÐ¡ÍÓÂÝ×´Ì¬
        Point2f TuoLuoPredict(Base *base);//Ô¤²â
		vector<RotatedRect> rects;

public:
        Point2f armor_pre;
};

