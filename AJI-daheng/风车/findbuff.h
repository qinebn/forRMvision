/*
	本类用于寻找待击打装甲板
	通过大风车装甲板唯一且无兄弟轮廓筛选
	仅初期
*/
#pragma once
#include"buffbase.h"
class findbuff
{
private:
	void getContours(Mat process_image,vector<RotatedRect>& maybe_armor);//获取所有可能为须击打装甲板的轮廓
	bool chooseContour(vector<RotatedRect> maybe_armor,RotatedRect& armor);//筛选唯一装甲板
	bool judgeContours(RotatedRect maybe_armor);//判断所得轮廓是否满足作为一个装甲板
public:
	bool runfind(Mat process_image, RotatedRect& armor);//运行
	vector<vector<Point>> Centers;//待判断中心可能区域
};

