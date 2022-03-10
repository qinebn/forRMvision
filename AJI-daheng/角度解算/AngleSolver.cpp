#include "AngleSolver.h"

AngleSolver::AngleSolver()
{
}

AngleSolver::~AngleSolver()
{
}

//void AngleSolver::test(RotatedRect last)
//{
//   if(last.angle <= 0 )
//       printf("sada\n");
//   else {
//       printf("???");
//   }

//}


void AngleSolver::setCameraParam( cv::Mat& CAMERA_MATRIX,  cv::Mat& DISTORTION_COEFF)
{
//   CAMERA_MATRIX = (Mat_<double>(3, 3) <<1.285354341402344e+03 ,0 , 3.282348530855244e+02, 0 ,1.285891067253485e+03 , 2.731768801449926e+02 , 0 , 0 , 1);
//   DISTORTION_COEFF = (Mat_<double>(4, 1) <<-0.081939362249884, 0.027195359366403 , 0 ,0);
    CAMERA_MATRIX = (Mat_<double>(3, 3) <<1.286466747979980e+03 , 0 , 3.496687475370769e+02 , 0 , 1.285843187236136e+03 , 2.468373374247954e+02 , 0 , 0 , 1);
    DISTORTION_COEFF = (Mat_<double>(4, 1) <<-0.071391126254941 , -0.040910807800513 , 0 , 0);



}

//void AngleSolver::receLastarmor(const cv::RotatedRect& rect)
//{
//    //rect1=rect;

//}


//void AngleSolver::setArmorSize(ArmorType type, double width, double height)
void AngleSolver::setArmorSize(ArmorType type)
{
//	double small_half_x = 135/ 2.0;
//	double small_half_y = 125 / 2.0;
//	double big_half_x = 230/ 2.0;
//	double big_half_y = 127 / 2.0;


//    double half_x=width/2.0;
//   double half_y=height/2.0;

    switch (type)
    {
	case SMALL_ARMOR:     
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(-half_x, half_y, 0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(half_x, half_y, 0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(half_x, -half_y, 0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(-half_x,- half_y, 0));
        //从lu开始顺时针排序
        SMALL_ARMOR_POINTS_3D.clear();
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(-65.0, -37.5,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(-65.0, 37.5,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(65.0,37.5,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(65.0,-37.5,0.0));


//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 0.0,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 37.5,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(65.0,37.5,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(65.0,0.0,0.0));

//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 0.0,0.0));//傻逼小小怪
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 75.00,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(130.0,75.00,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(130.0,0.0,0.0));
          SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 0.0,0.0));//
          SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 100.00,0.0));
          SMALL_ARMOR_POINTS_3D.push_back(Point3f(125.0,100.00,0.0));
          SMALL_ARMOR_POINTS_3D.push_back(Point3f(125.0,0.0,0.0));



//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 0.0,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(0.0, 40.0,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(70.0,40.0,0.0));
//        SMALL_ARMOR_POINTS_3D.push_back(Point3f(70.0,0.0,0.0));

          break;

	case BIG_ARMOR:
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(half_x, -half_y, 0));
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(half_x, half_y, 0));
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(-half_x, half_y, 0));
//       BIG_ARMOR_POINTS_3D.push_back(Point3f(-half_x, -half_y, 0));
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(115, -28.5, 0));
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(115, 28.5, 0));
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(-115, 28.5, 0));
//        BIG_ARMOR_POINTS_3D.push_back(Point3f(-115,- 28.5, 0));
                BIG_ARMOR_POINTS_3D.push_back(Point3f(0.0, 0.0, 0.0));
                BIG_ARMOR_POINTS_3D.push_back(Point3f(0.0, 70, 0.0));
                BIG_ARMOR_POINTS_3D.push_back(Point3f(235.0,70, 0.0));
                BIG_ARMOR_POINTS_3D.push_back(Point3f(235,0.0, 0.0));
//                BIG_ARMOR_POINTS_3D.push_back(Point3f(0.0, 0.0, 0.0));
//                BIG_ARMOR_POINTS_3D.push_back(Point3f(0.0, 135, 0.0));
//                BIG_ARMOR_POINTS_3D.push_back(Point3f(230.0,135, 0.0));
//                BIG_ARMOR_POINTS_3D.push_back(Point3f(230,0.0, 0.0));
        break;

    default:
        break;

	}
}


void AngleSolver::setBulletSpeed(int bulletSpeed)
{
	BULLET_SPEED = bulletSpeed;
}



//void AngleSolver::getTarget2dPoinstion(const cv::RotatedRect& rect, vector<Point2f>& target2d, const cv::Point2f& offset) {

//    Point2f vertices[4];
//    rect.points(vertices);


//    targetContour.push_back(vertices[0]);
//    targetContour.push_back(vertices[1]);
//    targetContour.push_back(vertices[2]);
//    targetContour.push_back(vertices[3]);

//    Point2f lu, ld, ru, rd;
//    int ii, jj;
//    for (ii = 0; ii < 3; ii++)
//    {
//        int k = ii;
//        for (jj = ii + 1; jj < 4; jj++)
//        {
//            if (vertices[jj].x < vertices[ii].x)
//                k = jj;
//        }
//        if (k != ii)
//        {
//            Point2f exchange;
//            exchange = vertices[ii];
//            vertices[ii] = vertices[k];
//            vertices[k] = exchange;
//        }
//    }
//    if (vertices[0].y < vertices[1].y) {
//        lu = vertices[0];
//        ld = vertices[1];
//    }
//    else {
//        lu = vertices[1];
//        ld = vertices[0];
//    }
//    if (vertices[2].y < vertices[3].y) {
//        ru = vertices[2];
//        rd = vertices[3];
//    }
//    else {
//        ru = vertices[3];
//        rd = vertices[2];
//    }

//    target2d.clear();
//    target2d.push_back(lu + offset);
//    target2d.push_back(ru + offset);
//    target2d.push_back(rd + offset);
//    target2d.push_back(ld + offset);


//}


//void AngleSolver::setTarget(vector<Point2f> contourPoints, Point2f centerPoint, ArmorType type)



void AngleSolver::setTarget(const cv::RotatedRect& rect, Point2f centerPoint, ArmorType type)
{
     Point2f vertices[4];
//     rect.points(vertices);
     //图片的左上到右下
     float lx = (rect.center.x - rect.size.width/2);
     float ly = (rect.center.y - rect.size.height/2);
     float rx = (rect.center.x + rect.size.width/2);
     float ry = (rect.center.y + rect.size.height/2);
     vertices[0] = Point2f(lx,ry);
     vertices[1] = Point2f(lx,ly);
     vertices[2] = Point2f(rx,ly);
     vertices[3] = Point2f(rx,ry);
     targetContour.push_back(vertices[0]);
     targetContour.push_back(vertices[1]);
     targetContour.push_back(vertices[2]);
     targetContour.push_back(vertices[3]);
     
//     cout<<vertices[0]<<endl;
//     cout<<vertices[1]<<endl;
//     cout<<vertices[2]<<endl;
//     cout<<vertices[3]<<endl;

//    targetContour = target2d;
//    targetCenter = Point2f((target2d[0].x+target2d[1].x)/2,(target2d[1].y+target2d[2].y)/2);
//    targetCenter=base.lastarmor.center;

     targetCenter=centerPoint;

     targetType = type;

}

//   重载
//void AngleSolver::setTarget(const cv::RotatedRect& rect)
//{
//     Point2f vertices[4];
//     rect.points(vertices);
//     targetContour.push_back(vertices[0]);
//     targetContour.push_back(vertices[1]);
//     targetContour.push_back(vertices[2]);
//     targetContour.push_back(vertices[3]);
////    targetContour = target2d;
////    targetCenter = Point2f((target2d[0].x+target2d[1].x)/2,(target2d[1].y+target2d[2].y)/2);
////    targetCenter=base.lastarmor.center;


//}






void AngleSolver::solveAngles()
{
	Mat _rvec;    


    switch (teptype)
    {


    case SMALL_ARMOR:

   //solvePnPRansac(SMALL_ARMOR_POINTS_3D, targetContour, CAMERA_MATRIX, DISTORTION_COEFF, _rvec, tVec, true,1000, 1, 0.99);


    solvePnP(SMALL_ARMOR_POINTS_3D, targetContour, CAMERA_MATRIX, DISTORTION_COEFF, _rvec, tVec,false,CV_ITERATIVE);

//    Rodrigues(rVec,rotM);
//    Rodrigues(tVec,rotT);

//    cout<<targetContour<<endl;


     break;
    case BIG_ARMOR:
        solvePnP(BIG_ARMOR_POINTS_3D, targetContour, CAMERA_MATRIX, DISTORTION_COEFF, _rvec, tVec,false,CV_ITERATIVE);
   //solvePnPRansac(BIG_ARMOR_POINTS_3D, targetContour, CAMERA_MATRIX, DISTORTION_COEFF, _rvec, tVec, true, 1000, 1, 0.99); break;

    default:
        break;
    }

//    double theta_x,theta_y,theta_z;
//    theta_x=atan2(rotM.at<double>(2,1),rotM.at<double>(2,2));
//    theta_y=atan2(-rotM.at<double>(2,0),
//    sqrt(rotM.at<double>(2,1)*rotM.at<double>(2,1)+rotM.at<double>(2,2)*rotM.at<double>(2,2)));
//    theta_z=atan2(rotM.at<double>(1,0),rotM.at<double>(0,0));
//    theta_x=theta_x*(180/CV_PI);
//    theta_y=theta_y*(180/CV_PI);
//    theta_z=theta_z*(180/CV_PI);

//    Mat P;
//    P=(rotM.t())* tVec;

//    cout<<"angle************"<<endl;
//    cout<<theta_x<<endl;
//    cout<<theta_y<<endl;
//    cout<<theta_z<<endl;
//    cout<<"5555555555555555"<<P<<endl;




    GUN_CAM_DISTANCE_Y = 44.19;
    tVec.at<double>(1, 0) -= GUN_CAM_DISTANCE_Y;
//    tVec.at<double>(2, 0) -= 53.05;
    double x_pos = tVec.at<double>(0, 0);
    double y_pos = tVec.at<double>(1, 0);
    double z_pos = tVec.at<double>(2, 0);


    distance = sqrt(x_pos * x_pos + y_pos * y_pos + z_pos * z_pos);


	
    if (distance > 4000)
	{
		PinHole_solver();     
	}
	
	else
	{
		P4P_solver();
	}
}







void AngleSolver::P4P_solver()
{
    double x_pos = tVec.at<double>(0, 0);
    double y_pos = tVec.at<double>(1, 0);
    double z_pos = tVec.at<double>(2, 0);

    double tan_pitch = y_pos / sqrt(x_pos * x_pos + z_pos * z_pos);
    double tan_yaw = x_pos / z_pos;
    x_pitch = -atan(tan_pitch)*180/CV_PI;
    y_yaw = atan(tan_yaw)*180/CV_PI;

//    if(!teptype)
    if(1)
    {
        if(distance<1400)
        {
            y_yaw=y_yaw+3.0;
            x_pitch=x_pitch+5.5 ;
        }
        if(distance>=1400&&distance<1800)
        {
            y_yaw=y_yaw+1.611;
            x_pitch=x_pitch+4.3 ;
        }
        if(distance>=1800&&distance<2150)
        {
            y_yaw=y_yaw+1.6;
            x_pitch=x_pitch+4.21 ;
        }
        if(distance>=2150&&distance<2600)
        {
            y_yaw=y_yaw+1.3 ;
            x_pitch=x_pitch+4.0;
        }
        if(distance>=2600&&distance<3000)
        {
            y_yaw=y_yaw+1.0;
            x_pitch=x_pitch+3.7 ;
        }

        if(distance>=3000&&distance<3500)
        {
            y_yaw=y_yaw+0.715;
            x_pitch=x_pitch+3.82-0.38;
        }
        if(distance>=3500&&distance<4000)
        {
            y_yaw=y_yaw+0.8;
            x_pitch=x_pitch+3.5 ;
        }
//        if(distance>=3800&&distance<4300)
//        {
//            y_yaw=y_yaw+1.4;
//            x_pitch=x_pitch;
//        }
//        if(distance>=4300&&distance<5000)
//        {
//            y_yaw=y_yaw-18.1;
//            x_pitch=x_pitch+5.78;
//        }
//        if(distance>=5000&&distance<6000)
//        {
//            y_yaw=y_yaw;
//            x_pitch=x_pitch+;
//        }
//        if(distance >= 6000)
//        {
//            y_yaw += 0.981973;
//            x_pitch -= 1.28829;
//        }
    }
    else
    {
        if(distance<1800)
        {
            y_yaw=y_yaw+3.7;
            x_pitch=x_pitch+5.8 ;
        }
        if(distance>=1800&&distance<2150)
        {
            y_yaw=y_yaw+3.14;
            x_pitch=x_pitch+4.2;
        }
        if(distance>=2150&&distance<2600)
        {
            y_yaw=y_yaw+2.1;
            x_pitch=x_pitch+3.58 ;
        }
        if(distance>=2600&&distance<3000)
        {
            y_yaw=y_yaw+2.4;
            x_pitch=x_pitch+4.6 ;
        }

        if(distance>=3000&&distance<3500)
        {
            y_yaw=y_yaw+0.9;
            x_pitch=x_pitch +3.75;
        }
        if(distance>=3500&&distance<4000)
        {
            y_yaw=y_yaw+1.34;
            x_pitch=x_pitch +3.75;
        }
//        if(distance<1150)
//        {
//            y_yaw=y_yaw+3;
//            x_pitch=x_pitch;
//        }
//        if(distance>=1150 && distance<1650)
//        {
//            y_yaw=y_yaw+5;
//            x_pitch=x_pitch ;
//        }
//        if(distance>=1650&&distance<2150)
//        {
//            y_yaw=y_yaw+5;
//            x_pitch=x_pitch ;
//        }
//        if(distance>=2150&&distance<2600)
//        {
//            y_yaw=y_yaw+4;
//            x_pitch=x_pitch - 1;
//        }
//        if(distance>=2600&&distance<3000)
//        {
//            y_yaw=y_yaw+4;
//            x_pitch=x_pitch - 2;
//        }

//        if(distance>=3000&&distance<3800)
//        {
//            y_yaw=y_yaw+2;
//            x_pitch=x_pitch +10;
//        }
//        if(distance>=3800&&distance<4000)
//        {
//            y_yaw=y_yaw+20.120;
//            x_pitch=x_pitch-6.36689;
//        }
//        if(distance > 6000)
//        {
//            y_yaw += 0.981973;
//            x_pitch -= 1.28829;
//        }
    }

//    if(distance>=3800&&distance<4300)
//    {
//        y_yaw=y_yaw+20.120;
//        x_pitch=x_pitch-6.36689;
//    }



//    y_yaw=y_yaw+15.9;
//    x_pitch=x_pitch-9.10689;
//     y_yaw=y_yaw+15.5733;
//    x_pitch=x_pitch-11.90689;



//    y_yaw=atan(tVec.at<double>(0,0)/tVec.at<double>(2,0))/CV_PI*180;
//    x_pitch=atan(tVec.at<double>(0,0)/tVec.at<double>(2,0))/CV_PI*180;


//    cout<<"zzzzzzzzzz_poz"<<z_pos<<endl;
//    cout<<""<<x_pos<<endl;
//    cout<<""<<y_pos<<endl;






}

void AngleSolver::PinHole_solver()
{
    double fx = CAMERA_MATRIX.at<double>(0, 0);
    double fy = CAMERA_MATRIX.at<double>(1, 1);
    double cx = CAMERA_MATRIX.at<double>(0, 2);
    double cy = CAMERA_MATRIX.at<double>(1, 2);
    Point2f pnt;
    vector<cv::Point2f> in;
    vector<cv::Point2f> out;
    in.push_back(targetCenter);


    undistortPoints(in, out, CAMERA_MATRIX, DISTORTION_COEFF, noArray(), CAMERA_MATRIX);
    pnt = out.front();

    double rxNew = (pnt.x - cx) / fx;
    double ryNew = (pnt.y - cy) / fy;

    y_yaw = atan(rxNew) / CV_PI * 180;
    x_pitch = -atan(ryNew) / CV_PI * 180;

    if(distance>=4000&&distance<4500)
    {
        y_yaw=y_yaw-0.24;
        x_pitch=x_pitch+3.6;
    }
    if(distance>=4500&&distance<5000)
    {
        y_yaw=y_yaw-0.35;
        x_pitch=x_pitch+3.6;
    }
    if(distance>=5000&&distance<6000)
    {
        y_yaw=y_yaw-0.45;
        x_pitch=x_pitch+3.3;
    }


}




void AngleSolver::compensateAngle(double PTZ_pitch)
{
	compensateOffset();
    compensateGravity(PTZ_pitch);
}



void AngleSolver::compensateOffset()
{
    double camera_target_height = distance * sin(x_pitch / 180 * CV_PI);

    double gun_target_height = camera_target_height - GUN_CAM_DISTANCE_Y;

    double gun_pitch_tan = gun_target_height / (distance * cos(x_pitch /180 * CV_PI));
    x_pitch = atan(gun_pitch_tan)/CV_PI*180;

}

void AngleSolver::compensateGravity(double PTZ_pitch)
{
        double dis=distance/1000.0, SPEED=25.000;
        cout<<"speed:   "<<BULLET_SPEED<<endl;
        if(BULLET_SPEED>18)
            SPEED=BULLET_SPEED+22.5;
        else if(BULLET_SPEED>17)
            SPEED=BULLET_SPEED+15.5;
        else if(BULLET_SPEED>15)
            SPEED=BULLET_SPEED+9.5;
        else if(BULLET_SPEED>14)
            SPEED=BULLET_SPEED+7.3;
        else if(BULLET_SPEED>13)
            SPEED=BULLET_SPEED+4.5;
        else if(BULLET_SPEED>12)
            SPEED=BULLET_SPEED+3.5;
//        else
//            SPEED=BULLET_SPEED+3;
//        cout<<"PTZ_pitch:    "<<PTZ_pitch<<endl;
        double un_pitch=360-(PTZ_pitch+34.009);
        if(un_pitch>=360.0)
            un_pitch-=360.0;

        double dian_pitch=un_pitch;
        cout<<"pitch  "<<x_pitch<<endl;
        cout<<"debug get pitcg :  "<<un_pitch<<endl;


        double a=0,b=0,c=0,z=0,y=0;
        z=cos((-x_pitch+dian_pitch)/180.0*CV_PI)*dis;
        y=sin((-x_pitch+dian_pitch)/180.0*CV_PI)*dis;

        a=-0.5*9.8*((z*z)/(SPEED*SPEED));
        b=z;
        c=-0.5*9.8*((z*z)/(SPEED*SPEED))+y;

        double tan_=0.0;
//        double discriminant=b*b-4*a*c;
        double tan_1=(-b+sqrt(b*b-4*a*c))/(2.0*a);
        double tan_2=(-b-sqrt(b*b-4*a*c))/(2.0*a);

        if(tan_1<=tan_2)
            tan_=tan_1;
        else if(tan_2<tan_1)
            tan_=tan_2;

//        double p=z*tan_+(c-y)*(1+tan_*tan_);

//        tan_=atan(tan_)/CV_PI*180;
//        double q=dian_pitch+tan_;
        dian_pitch=tan(dian_pitch/180.0*CV_PI);
        double q=(atan(dian_pitch+tan_)/CV_PI*180);
        x_pitch=q;
//        cout<<"debug compensateGravity :   "<<x_pitch<<"    "<<q<<"     "<<p<<"    "<<y<<endl;

//        //上云台pitch计算
//        c=-0.5*9.8*((z*z)/(SPEED*SPEED))+y+0.465;
//        double tan_0=0.0;
//        double tan_10=(-b+sqrt(b*b-4*a*c))/(2.0*a);
//        double tan_20=(-b-sqrt(b*b-4*a*c))/(2.0*a);
//        if(tan_10<=tan_20)
//            tan_0=tan_10;
//        else if(tan_20<tan_10)
//            tan_0=tan_20;

//        up_pitch=(atan(tan_0-tan_)/CV_PI*180);
//        cout<<"up pitch"<<up_pitch+x_pitch<<endl;

}

void AngleSolver::getAngle(const cv::RotatedRect& rect, Point2f cenerPoint, ArmorType type, double& yaw, double& pitch, double& evaluateDistance, double speeds, double PTZ_pitch)
//void AngleSolver::getAngle(vector<Point2f>& contourPoints, Point2f cenerPoint, ArmorType type, double& yaw, double& pitch, double& evaluateDistance)
{
    teptype = type;
    setCameraParam(CAMERA_MATRIX, DISTORTION_COEFF);
//  getTarget2dPoinstion(rect, target2d, offset);
    setArmorSize(type);
//  setTarget(rect,contourPoints, cenerPoint, type);
    setTarget(rect, cenerPoint, type);
    setBulletSpeed(speeds);

    solveAngles();
    compensateAngle(PTZ_pitch);

    yaw = y_yaw ;
    pitch = x_pitch ;
	evaluateDistance = distance;

     cout<<"yaw  "<<yaw<<endl;
     cout<<"pitch  "<<pitch<<endl;
     cout<<"distance"<<evaluateDistance<<endl;
}







//vector<Point2f> AngleSolver::getTarget2d()
//{
//    return targetContour;
//}
