/*
	��������Ѱ������R,֮�������жϷ������Ԥ��λ��
	����ʶ���ʽϵʹ����70%
	�������ɸѡ����
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

