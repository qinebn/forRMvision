/*
	pretreat.h为图像预处理类，目的处理成适合寻找风车轮廓的图片
*/

#include"buffbase.h"
#pragma once
class pretreat
{
	
private:
    void SubtractChannels(Mat raw_image, Mat& process_image,Base base1);//通道相减并二值化
	void ImageOptimize(Mat raw_image, Mat& process_image);//图像优化
public:
    void run(Mat raw_image, Mat& process_image,Base base1);
};

