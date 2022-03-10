#include "findbuff.h"

void findbuff::getContours(Mat process_image, vector<RotatedRect>& maybe_armor)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Centers.clear();
	findContours(process_image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++) {
        size_t cont = hierarchy[i][2];
		//将可能中心保存
		if (cont == -1) { 
			Centers.push_back(contours[i]);
			continue;
		}
		if (hierarchy[cont][0] == -1 && hierarchy[cont][1] == -1) {
			RotatedRect rect = minAreaRect(contours[cont]);
			//cout <<"装甲比例"<< rect.size.height / rect.size.width << endl;
			//circle(process_image, rect.center, 2, Scalar(255, 0, 0), 2);
			if(judgeContours(rect))
			maybe_armor.push_back(rect);
		}
	}
	//imshow("armor", process_image);
}

bool findbuff::chooseContour(vector<RotatedRect> maybe_armor, RotatedRect& armor)
{
	size_t i = maybe_armor.size();
	if (i == 0) {
		cout << "识别到的总装甲数" << i << endl;
		return false;
	}
	else if (i == 1) {
		armor = maybe_armor[0];
		return true;
	}
	else {
		cout <<"识别到的总装甲数"<< i << endl;
		return false;
	}
	
}

bool findbuff::judgeContours(RotatedRect maybe_armor)
{
	float rate = maybe_armor.size.height / maybe_armor.size.width;
	if ((rate < 1.95 && rate>1.68)) return true; 
	else if ((rate < 0.60 && rate>0.5)) return true;
	else	return false;
}

bool findbuff::runfind(Mat process_image, RotatedRect& armor)
{
	vector<RotatedRect> maybe_armor;
	getContours(process_image, maybe_armor);
	if (!chooseContour(maybe_armor, armor)) {
		return false;
	}
	return true;
}
