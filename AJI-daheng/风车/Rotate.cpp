/*
	会不会出现在0和360度分界的地方导致顺时针被判断为逆时针的问题
*/
#include "Rotate.h"

void Rotate::judgeRotate(float angle1, float angle2)
{
	
	if (angle1 < angle2) {
        buffbase.anticlockwise++;
	}
	else if (angle1 > angle2) {
        buffbase.clockwise++;
	}

    if (buffbase.anticlockwise > 15) {
        buffbase.direction = 0;
        buffbase.is_direction = 1;
	}

    if (buffbase.clockwise > 15) {
        buffbase.direction = 1;
        buffbase.is_direction = 1;
	}
}


void Rotate::runRotate(RotatedRect armor1, RotatedRect armor2, RotatedRect CenterR)
{	
	float angle1 =fastAtan2(CenterR.center.y - armor1.center.y, armor1.center.x - CenterR.center.x);
	float angle2 =fastAtan2(CenterR.center.y - armor2.center.y, armor2.center.x - CenterR.center.x) ;
	cout << "角度1   " << angle1 << endl;
	cout << "角度2   " << angle2 << endl;
	judgeRotate(angle1, angle2);
}
