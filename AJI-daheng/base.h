/*
    该文件用来:
            存储基本的内容方便操作
            储存各种必要数据便于在赛场上维护
            储存各种宏变量便于测试

*/
#ifndef BASE_H
#define BASE_H
#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <Eigen/Dense>


using namespace std;
using namespace cv;
using Eigen::MatrixXd;

#define _max(X,Y) ((X) > (Y) ? (X) : (Y))
#define _min(X,Y) ((X) < (Y) ? (X) : (Y))

//   #define PICTURE
//   #define VIDEO
  #define CAMERA
  //#define SAVE_VIDEO


//
// #define SHOW_CAMERA
 //#define SHOW_RESULT_ARMOR
//#define SHOW_SINGLE_LIGHT
 //#define SHOW_DOUBLE_LIGHT
//  #define SHOW_WHITE_IMAGE
  //
//#define SHOW_TIME
 //  #define LARGE_SRC

//#define yuce1
//#define yuce2
//#define yuce3

//#define SHOW_PTZ
//#define PRE_KALMAN
#define PRE_NUMBER




typedef struct base
{
    //armor
    bool _enemy_color = 0;		//0 for blue ,1 for red

    size_t single_light_lowest_threshold = 100;		//单个灯条阈值最低值
    size_t angle_single_threshold = 30;	//判断单个灯条角度的阈值
    double angle_double_threshould = 5;	//判断两个灯条角度差的阈值

    size_t white_threshould = 11;		//中间装甲板白色阈值
    //size_t high_white_threshould = 180;
    double white_pixel = 7;		//中间装甲板 白色比总像素 选择值
    float gamma = 0.8;        //gamma值
    size_t gamma_number = 1;  //gamma循环次数
    size_t whitemore = 0;      //白色大于阈值曝光修改计数器
    size_t whiteless = 0;    //白色小于阈值曝光修改计数器
    size_t armor_number = 0;  //装甲检测计数器
    size_t change = 0;       //曝光修改检测器

    size_t last_roi;       //0:0~200; 1:200~600; 2:600~800
    bool _temple_type = 0; //装甲类型 0是小装甲板 1是大装甲板 2 is buff
    Mat roi;
    bool image_roi = 0;
    double distance = 0.00;
    size_t Exposure_type = 0;  //0 for normal, 1 for special

    //armor

    vector<RotatedRect> armor; //装甲板组
    RotatedRect lastarmor; //最后装甲矩形


    String string = "/home/sc/第一代/1.mp4";		//图片地址

    Mat camera_image;  //相机获取图片
    Mat src ;		//图片
    Mat gray;		//src的灰度图片

    size_t lost_cnt = 0;

    size_t buff_color=1;//0 blue 1 red
    size_t low_red_thresh = 70;//敌人为红色时的低阈值
    size_t low_blue_thresh = 80;//敌人为蓝色时的高阈值
    size_t high_thresh = 255;
    double delay = 0.5;
    vector<RotatedRect> armors;

//PREDICT
    vector<RotatedRect> move_pre_rects;     //11
    vector<RotatedRect> move_pre_rects1;    //100
    vector<double> move_pre_dis;
    vector<RotatedRect> move_pre_TL;

//tuoluo
    vector<RotatedRect> save_armor;

    //serial
    int fd=-1;
    double getYaw=0;//串口读取到的yaw pitch time数据
    double getPitch=0;
    double getSpeed=0;
    double getYaw_angle=0;
    double getPitch_angle=0;
    int getState=1;//1步兵模式 2 风车模式
//predictor
    double pre_count = 0;
    //更新参数
    MatrixXd K; //卡尔曼增益
    MatrixXd P; //更新协方差
    MatrixXd X; //更新估计值

    //预测参数
    MatrixXd P_; //预测协方差
    MatrixXd X_; //预测估计值
    bool pre_flag = 0;


}Base;

struct
{

    size_t clockwise=0;//顺时针次数
    size_t anticlockwise=0;//逆时针次数
    size_t direction = 1;//1为顺时针//0为逆时针
    size_t is_direction = 0;
    vector<double> anglespeeds;
    vector<double> times;
    vector<double> thts;
    size_t smallbuff=0;//小风车次数
}buffbase;


#endif // BASE_H
