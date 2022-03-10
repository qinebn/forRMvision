/*
	本类用于寻找中心R,之后用于判断方向才能预测位置
	中心识别率较低大概在70%
	仍需添加筛选方法
*/

#pragma once
#include"buffbase.h"
class findCenter
{
private:
	void CenterChoose(vector<vector<Point>> Centers,RotatedRect armor,RotatedRect& CenterR);
	bool judgeCenter(RotatedRect maybe_center);
public:
	void runCenter(vector<vector<Point>> Centers, RotatedRect armor, RotatedRect& CenterR);

};

