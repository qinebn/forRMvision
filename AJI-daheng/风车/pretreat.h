/*
	pretreat.hΪͼ��Ԥ�����࣬Ŀ�Ĵ�����ʺ�Ѱ�ҷ糵������ͼƬ
*/

#include"buffbase.h"
#pragma once
class pretreat
{
	
private:
    void SubtractChannels(Mat raw_image, Mat& process_image,Base base1);//ͨ���������ֵ��
	void ImageOptimize(Mat raw_image, Mat& process_image);//ͼ���Ż�
public:
    void run(Mat raw_image, Mat& process_image,Base base1);
};

