#pragma once


#include "Gernal.h"
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "base.h"


using namespace cv;
using namespace std;






class AngleSolver
{
public:
    AngleSolver();

	  ~AngleSolver();


      void test(RotatedRect sda);
	  void getTarget2dPoinstion(const cv::RotatedRect& rect, std::vector<cv::Point2f>& target2d, const cv::Point2f& offset);

      //void setCameraParam(const cv::Mat& camera_matrix, const cv::Mat& distortion_coeff);
      void setCameraParam( cv::Mat& CAMERA_MATRIX,  cv::Mat& DISTORTION_COEFF);

      void receLastarmor(const cv::RotatedRect& rect);
	  
  // void setArmorSize(ArmorType type, double width, double height);
     void setArmorSize(ArmorType type);


     // void setArmorSize(ArmorType type);
	  void setBulletSpeed(int bulletSpeed);

      //void setTarget(vector<Point2f> contoursPoints, Point2f centerPoint, ArmorType type);

      void setTarget(const cv::RotatedRect& rect, Point2f centerPoint, ArmorType type);
//      void setTarget(const cv::RotatedRect& rect);


      //void solveAngles( RotatedRect aw);
      void solveAngles();

      void P4P_solver();

	  void PinHole_solver();

      void compensateAngle(double PTZ_pitch);

      void compensateOffset();
    //  void compensateGravity();
      void compensateGravity(double PTZ_pitch);
      //void getAngle(vector<Point2f>& contourPoints, Point2f centerPoint, ArmorType type, double& yaw, double& pitch, double& evaluateDistance);
      void getAngle(const cv::RotatedRect& rect, Point2f centerPoint, ArmorType type, double& yaw, double& pitch, double& evaluateDistance,double speeds, double PTZ_pitch);

//      void end_solve();



//      vector<Point2f> getTarget2d();


public:
      Base base;
      std::vector<cv::Point2f> target2d;
      cv::RotatedRect  rect;
      cv::Point2f  offset = Point2f(0,0);
      vector<Point2f> targetContour;
      Point2f targetCenter;
      Point2f tepCenter;
      RotatedRect rect1;
private:


    Mat CAMERA_MATRIX;
    Mat DISTORTION_COEFF;

	
	vector<Point3f> SMALL_ARMOR_POINTS_3D;
	vector<Point3f> BIG_ARMOR_POINTS_3D;

	
	double BULLET_SPEED;

	
    double GUN_CAM_DISTANCE_Y=50.0;

	


	ArmorType targetType;
    ArmorType teptype;

	

    Mat rVec=Mat::zeros(3,1,CV_64FC1);
    Mat tVec=Mat::zeros(3,1,CV_64FC1);


//    Mat rotM=Mat::eye(3,1,CV_64FC1);
//    Mat rotT=Mat::eye(3,1,CV_64FC1);


    double y_yaw;
    double x_pitch;
	double distance;
public:
//    double up_pitch;       //上下云台pitch差
};








//class AngleSolverFactory {
//public:
//	AngleSolverFactory(AngleSolver* angle_solver = NULL) : slover(angle_solver) {
//	}

//	typedef enum { TARGET_RUNE, TARGET_ARMOR, TARGET_SAMLL_ATMOR } TargetType;

//	void setSolver(AngleSolver* angle_slover) {
//		slover = angle_slover;
//	}
//	AngleSolver& getSolver() {
//		return *slover;
//	}
//    void setTargetSize(double width, double height, TargetType type);


//	void adjustRect2FixedRatio(cv::RotatedRect& rect, double wh_ratio) {
//		rect.size.height = rect.size.width / wh_ratio;
//	}

//	bool getAngle(const cv::RotatedRect& rect, TargetType type, double& angle_x, double& angle_y, double& distance, double bullet_speed, double current_ptz_angle, const cv::Point2f& offset = cv::Point2f());

//private:
//	double armor_width;
//	double armor_height;
//	double small_armor_width;
//	double small_armor_height;
//	double rune_width;
//	double rune_height;
//	AngleSolver* slover;
//};


