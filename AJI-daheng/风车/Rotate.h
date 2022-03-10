/*
	在找到中心R的基础上判断旋转方向
	这个要修改
*/

#pragma once
#include"buffbase.h"
class Rotate
{
private:

	void judgeRotate(float angle1, float angle2);
public:

	void runRotate(RotatedRect armor1, RotatedRect armor2, RotatedRect CenterR);
};

