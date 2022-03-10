#include "pretreat.h"

void pretreat::SubtractChannels(Mat raw_image, Mat& process_image,Base base1)
{
	Mat sub;//通道相减后的照片
	vector<Mat> channels;
	split(raw_image, channels);
    if (base1.buff_color) {
		sub = channels[2] - channels[0];
        threshold(sub, process_image, base1.low_red_thresh, base1.high_thresh, THRESH_BINARY);
		imshow("binary", process_image);
	}
	else {
		sub = channels[0] - channels[2];
        threshold(sub, process_image, base1.low_blue_thresh, base1.high_thresh, THRESH_BINARY);
		imshow("binary", process_image);
	}
}

void pretreat::ImageOptimize(Mat raw_image, Mat& process_image)
{
	Mat jpg;
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	
	erode(raw_image, jpg, kernel2);
	imshow("erode", jpg);
	dilate(jpg, process_image, kernel1);
	dilate(jpg, process_image, kernel2);
	imshow("dilate", process_image);
	
/*	
	dilate(raw_image, jpg, kernel1);
	imshow("dilate", jpg);
	erode(jpg, process_image, kernel2);
	imshow("erode", process_image);
*/
}

void pretreat::run(Mat raw_image, Mat& process_image,Base base1)
{
	Mat img = raw_image;
	Mat jpg;
    SubtractChannels(img, jpg,base1);

	ImageOptimize(jpg, process_image);
}
