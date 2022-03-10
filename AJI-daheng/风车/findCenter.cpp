#include "findCenter.h"

void findCenter::CenterChoose(vector<vector<Point>> Centers, RotatedRect armor, RotatedRect& CenterR)
{
	vector<RotatedRect> process_centers;
	for (size_t i = 0; i < Centers.size(); i++) {
		RotatedRect cent = minAreaRect(Centers[i]);
		float rate = cent.size.height / cent.size.width;
		if (rate > 1.07 || rate < 0.875) continue;//判断长宽比例合不合适，筛选掉一部分
		//判断角度合不合适
		float y = armor.center.y - cent.center.y;
		float x = armor.center.x - cent.center.x;
		float armor_angle = fabs(armor.angle);
        size_t buff2armor_angle = ((size_t)fastAtan2(y, x))%90;
        size_t sub = fabs(armor_angle - buff2armor_angle);
        size_t addsub = fabs(armor_angle + buff2armor_angle-90);
		if (sub > 5 && addsub > 5) continue;
		//if(judgeCenter(cent))
			process_centers.push_back(cent);
	}

	if (process_centers.size() == 1) {
		CenterR = process_centers[0];
	//cout << process_centers[0].size.height / process_centers[0].size.width << endl;
	}

}

bool findCenter::judgeCenter(RotatedRect maybe_center)
{
	float min_area = 40;
	float max_area = 200;
	float area = maybe_center.size.area();
	if (area > min_area && area < max_area) return true;
	return false;
}

void findCenter::runCenter(vector<vector<Point>> Centers, RotatedRect armor, RotatedRect& CenterR)
{	
	CenterChoose(Centers, armor, CenterR);
}
