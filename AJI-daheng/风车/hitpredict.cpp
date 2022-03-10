#include "hitpredict.h"



void hitpredict::predict_smallbuff(Point2f CenterR, Point2f armor, Point2f& hit,int Rot,Base base1)
{
	float x1 = armor.x - CenterR.x;
	float y1 = armor.y - CenterR.y;
    double theta = CV_PI * base1.delay / 3;
	if (Rot) {
		hit.x = x1 * cos(-theta) + y1 * sin(-theta) + CenterR.x;
		hit.y = y1 * cos(-theta) - x1 * sin(-theta) + CenterR.y;
	}
	else {
		hit.x = x1 * cos(theta) + y1 * sin(theta)+CenterR.x;
		hit.y = y1 * cos(theta) - x1 * sin(theta)+CenterR.y;
	}
	
}

void hitpredict::predict_bigbuff(Point2f CenterR, Point2f armor, double now_t, Point2f& hit, int Rot,Base base1)
{
	now_t = fmod(now_t + tht,3.335);
	float x1 = armor.x - CenterR.x;
	float y1 = armor.y - CenterR.y;
    double theta = 0.41667*(cos(1.884*now_t)-cos(1.884*(now_t+base1.delay)))+1.305*base1.delay;
	//大风车0.5秒转的角度从15-60
	if (Rot) {
		hit.x = x1 * cos(-theta) + y1 * sin(-theta) + CenterR.x;
		hit.y = y1 * cos(-theta) - x1 * sin(-theta) + CenterR.y;
	}
	else {
		hit.x = x1 * cos(theta) + y1 * sin(theta) + CenterR.x;
		hit.y = y1 * cos(theta) - x1 * sin(theta) + CenterR.y;
	}
}

void hitpredict::predictTime()
{
    for (size_t i = 0; i < buffbase.anglespeeds.size()-1; i++) {
        double t1= (asin((buffbase.anglespeeds[i] - 1.305) / 0.785)) / 1.884;
        double t2 = (asin((buffbase.anglespeeds[i+1] - 1.305) / 0.785)) / 1.884;
		if ((t1 < 1.667 && t2 < 1.667)) {
			if (t1 < t2) {
                double time_sub = fmod(((buffbase.times[i] + buffbase.times[i + 1]) / 2 ),3.335)   - ((t1 + t2) / 2);
                buffbase.thts.push_back(time_sub);
			}
			else {
                double time_sub = fmod(((buffbase.times[i] + buffbase.times[i + 1]) / 2), 3.335) - 1.66667 + ((t1 + t2) / 2);
                buffbase.thts.push_back(time_sub);
			}
		}
		if (t1 > 1.667 && t2 > 1.667) {
			if (t1 < t2) {
                double time_sub = fmod(((buffbase.times[i] + buffbase.times[i + 1]) / 2), 3.335) - ((t1 + t2) / 2);
                buffbase.thts.push_back(time_sub);
			}
			else {
                double time_sub = fmod(((buffbase.times[i] + buffbase.times[i + 1]) / 2), 3.335) - 4.99928 + ((t1 + t2) / 2);
                buffbase.thts.push_back(time_sub);
			}
		}
		
	}


}

void hitpredict::SmallorBig(Point2f armor1, Point2f armor2, double t1, double t2, Point2f CenterR)
{
	//cout << "center" << CenterR.x << "," << CenterR.y << endl;//中心坐标
	float angle1 = fastAtan2(CenterR.y - armor1.y, armor1.x - CenterR.x);
	float angle2 = fastAtan2(CenterR.y - armor2.y, armor2.x - CenterR.x);
	float sub_angle = fabsf(angle2 - angle1);
	if (!sub_angle) return;
	if (sub_angle > 10) return;
	float anglespeed = sub_angle * CV_PI / 180 / (t2 - t1);
	if (anglespeed == 0) return;
	std::cout << "转速" << anglespeed/CV_PI*180 << endl;
	std::cout << "时长" << t2 - t1 << endl;
	
    buffbase.anglespeeds.push_back(anglespeed);
	if (anglespeed - 1.04719 < 0.07 && anglespeed - 1.04719 > -0.07)
        buffbase.smallbuff++;
    std::cout << "小风车数" << buffbase.smallbuff << endl;
    if (buffbase.smallbuff > 15) buffbase.smallbuff = -1;
}

void hitpredict::judgetht()
{
	double add=0;
    size_t max = buffbase.thts.size();
	for (size_t i = 0; i < max; i++) {
        add += buffbase.thts[i];
	}
	tht = add / max;
	
}


