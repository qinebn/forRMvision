/*
	���ҵ�����R�Ļ������ж���ת����
	���Ҫ�޸�
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

