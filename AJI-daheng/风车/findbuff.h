/*
	��������Ѱ�Ҵ�����װ�װ�
	ͨ����糵װ�װ�Ψһ�����ֵ�����ɸѡ
	������
*/
#pragma once
#include"buffbase.h"
class findbuff
{
private:
	void getContours(Mat process_image,vector<RotatedRect>& maybe_armor);//��ȡ���п���Ϊ�����װ�װ������
	bool chooseContour(vector<RotatedRect> maybe_armor,RotatedRect& armor);//ɸѡΨһװ�װ�
	bool judgeContours(RotatedRect maybe_armor);//�ж����������Ƿ�������Ϊһ��װ�װ�
public:
	bool runfind(Mat process_image, RotatedRect& armor);//����
	vector<vector<Point>> Centers;//���ж����Ŀ�������
};

