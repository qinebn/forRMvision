#include "light.h"
#include "armor.h"
#include "lastarmor.h"
#include <algorithm>

bool truelengthRatio(RotatedRect& rect)		//对单个灯条长宽和比值进行判断（红：宽2高10；蓝：宽2高10）
{
    if (rect.size.height / rect.size.width >1.5 && rect.size.height / rect.size.width < 10  && rect.size.width > 1 &&  rect.size.width <20 && rect.size.height > 10  && rect.size.height < 200)
        return 1;
    else
        return 0;
}

void truerect(RotatedRect& rect)		//调整灯条的正确长宽
{
    float ch;
    ch = rect.size.width;
    rect.size.width = rect.size.height;
    rect.size.height = ch;
}

bool trueangle(RotatedRect& rect , Base base)		//判断灯条角度
{
    /*if (rect.angle < -(base.angle_single_threshold))
    {
        rect.angle += 90;
        truerect(rect);
    }
    if (rect.angle < base.angle_single_threshold)
        return 1;
    else
        return 0;*/
//    cout<<"single angle"<<endl;
//    cout<<rect.angle<<endl;
    if (rect.angle < 180 && rect.angle > 150)
        return 1;
    else if(rect.angle < 30 && rect.angle > -1)
        return 1;
    else
        return 0;
}

bool cmp (RotatedRect x ,RotatedRect y)
{
    if(x.size.height < 20)
    {
        if(abs(x.center.y - y.center.y) < 20 && abs(x.center.x - y.center.x) > 10 )
            return x.center.x < y.center.x;
        else
            return x.center.y < y.center.y;
    }
    else
    {
        if(abs(x.center.y - y.center.y) < 40 && abs(x.center.x - y.center.x) > 10 )
            return x.center.x < y.center.x;
        else
            return x.center.y < y.center.y;
    }
}

bool cmp1(RotatedRect x ,RotatedRect y)
{
    if(abs(x.center.y - y.center.y) > 10)
    {
        return x.center.x < y.center.x;
    }
}
Mat Light::fire(Mat src)
{
    Rect rect(200,150,300,200);
    return src(rect);
}

bool Light::findsinglelight(Mat& src, Base& base)		//通过提取颜色和二值化提取轮廓，进而确定单个灯条(开/闭操作？)
{
    LED_SINGLE_CHOOSE.clear();
    //Lastarmor lastarmor;
/*    char abc;
    abc = waitKey(1);
    if(abc == 'q' || abc == 'Q')
         base.image_roi=1;
    if(abc == 'w'|| abc == 'W')
        base.image_roi=0;
    if(base.image_roi == 1)
    {
        base.roi = fire(src);
        src = base.roi;
        imshow("roi",base.roi);
    }*/
    vector<Mat> channels;
    split(src, channels);
    Mat enemy_img;
    RotatedRect rect;
    cvtColor(src, base.gray, COLOR_BGR2GRAY);
    if (base._enemy_color == 1)		//提取不同颜色下的灯条图像
    {
        subtract(channels[2] , channels[1] , enemy_img);
    }
    else
    {
        subtract(channels[0] , channels[2] , enemy_img);
    }
#ifdef SHOW_SINGLE_LIGHT
    Mat img;
    enemy_img.copyTo(img);
#ifdef LARGE_SRC
    resize(img, img, Point(img.rows * 0.3, img.cols * 0.15));
#endif
    imshow("enemy_img", img);
#endif
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    threshold(enemy_img, enemy_img, base.single_light_lowest_threshold, 255, CV_THRESH_BINARY);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(4 ,4), Point(-1, -1));
    dilate(enemy_img, enemy_img, kernel);
    //kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));
    //erode(enemy_img, enemy_img, kernel);
    findContours(enemy_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
#ifdef SHOW_SINGLE_LIGHT
    enemy_img.copyTo(img);
#ifdef LARGE_SRC
    resize(img, img, Point(img.rows * 0.3, img.cols * 0.15));
#endif
    imshow("enemy_img1",img);
#endif
    if (!contours.size())
    {
        base.lost_cnt++;
        printf("no contours\n");
        return 0;
    }
    for (size_t i = 0; i < contours.size(); i++)
    {
        if(contours[i].size() < 5)
        {
            continue;
        }
        //rect = minAreaRect(contours[i]);
        rect = fitEllipse(contours[i]);
        if (!trueangle(rect, base))
        {
            continue;
        }
        if (!truelengthRatio(rect))
        {
            continue;
        }
        LED_SINGLE_CHOOSE.push_back(rect);
    }
    if (LED_SINGLE_CHOOSE.size() < 2)
    {
        base.lost_cnt++;
        printf("single led < 2\n0");
        return 0;
    }
    sort(LED_SINGLE_CHOOSE.begin(),LED_SINGLE_CHOOSE.end(),cmp);
#ifdef SHOW_SINGLE_LIGHT
        base.src.copyTo(img);
        Point2f vertex[4];
        for (size_t j = 0; j < LED_SINGLE_CHOOSE.size(); j++)
        {
            LED_SINGLE_CHOOSE[j].points(vertex);
            for (size_t i = 0; i < 4; i++)
            {
                line(img, vertex[i], vertex[(i + 1) % 4], Scalar(0, 120, 255), 1, LINE_AA);
            }
                if (j == 0) putText(img, "1", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
                if (j == 1) putText(img, "2", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
                if (j == 2) putText(img, "3", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
                if (j == 3) putText(img, "4", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
                if (j == 4) putText(img, "5", Point(LED_SINGLE_CHOOSE[j].center.x - 10, LED_SINGLE_CHOOSE[j].center.y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);

            printf("x,y:%f,%f , width,height:%f,%f , angle:%f\n", LED_SINGLE_CHOOSE[j].center.x, LED_SINGLE_CHOOSE[j].center.y, LED_SINGLE_CHOOSE[j].size.width, LED_SINGLE_CHOOSE[j].size.height, LED_SINGLE_CHOOSE[j].angle);
        }
#ifdef LARGE_SRC
        resize(img, img, Point(img.rows * 0.7, img.cols * 0.5));
#endif
        imshow("findsinglelight", img);
        waitKey(1);
        cout << "LED_SINGLE_CHOOS: "  << LED_SINGLE_CHOOSE.size() << endl;
    cout << "LED_SINGLE_CHOOS: "  << LED_SINGLE_CHOOSE.size() << endl;
#endif
    return 1;
}

bool truedoubleangle(RotatedRect lightL, RotatedRect lightR , Base &base)		//两灯条的角度差
{
    float lL = lightL.angle, lR = lightR.angle;
    if(lL < 180 && lL > 165 && lR < 15 && lR > -1)
    {
        if (180 - lL + lR < base.angle_double_threshould)
        {
            return 1;
        }
        else
            return 0;
    }
    if (lR < 180 && lR > 165 && lL < 15 && lL > -1)
    {
        if (180 - lR + lL < base.angle_double_threshould)
            return 1;
        else
            return 0;
    }
    if (abs(lL - lR) < base.angle_double_threshould)
    {
        return 1;
    }
    else
        return 0;
}

bool truedoubley(RotatedRect lightL, RotatedRect lightR)		//两灯条y高度超过一定不要
{
//    cout<<"tha_y"<<endl;
//    cout<<abs(lightR.center.y - lightL.center.y)<<endl;
//    cout<<min(lightR.size.height, lightL.size.height)<<endl;

    if (abs(lightR.center.y - lightL.center.y) < 0.70 * min(lightR.size.height, lightL.size.height))
    {
        if(abs(lightL.size.height-lightR.size.height)<0.4*min(lightR.size.height, lightL.size.height))
        return 1;
    }
    else
    {
        return 0;
    }
}

bool truedoublex(RotatedRect lightL, RotatedRect lightR)		//两灯条x距离长度短或长过一定不要
{
    float width = min(lightR.size.width, lightL.size.width);
    size_t threshold = (size_t)(width);
    if ((size_t)(lightR.center.x) - (size_t)(lightL.center.x) > 0.8 * threshold && (size_t)(lightR.center.x) - (size_t)(lightL.center.x) < 50 * threshold)
        return 1;
    else
        return 0;
}

bool Light::finddoublelight(Base& base)		//通过两两匹配灯条确定是否可以拟合
{
    rect_choose.clear();
    RotatedRect lightL, lightR;
    RotatedRect rect;
    //Point2f lu, rd;
    for (size_t i = 0; i < LED_SINGLE_CHOOSE.size() - 1; i++)
    {
        for (size_t j = i + 1; j < i + 2 && j < LED_SINGLE_CHOOSE.size(); j++)
        {
            if (LED_SINGLE_CHOOSE[i].center.x < LED_SINGLE_CHOOSE[j].center.x)
            {
                lightL = LED_SINGLE_CHOOSE[i];
                lightR = LED_SINGLE_CHOOSE[j];
            }
           /* else
            {
                lightL = LED_SINGLE_CHOOSE[j];
                lightR = LED_SINGLE_CHOOSE[i];
            }*/
            if (!truedoublex(lightL, lightR)){
                cout<<"false truedoublex"<<endl;
                continue;
            }
            if (!truedoubleangle(lightL,lightR,base)){
                cout<<"false truedoubleangle"<<endl;
                continue;
            }
            if (!truedoubley(lightL, lightR)){
                cout<<"false truedoubley"<<endl;
                continue;
            }
/*            lightL.points(point);
            lu = point[1];
            lightR.points(point);
            rd = point[3];
            if (lu.x == rd.x || lu.y == rd.y)
                return 1;
            if (lightL.center.x  == lightR.center.x || lightL.center.y == lightR.center.y)
                return 0;*/
            //备注：从上往下数据依次变大,选点为左上（小）到右下（大）
  /*          lightL.points(point);
            for(size_t i = 0;i<4;i++)
            {
                pointlist1.push_back(point[i]);
            }
            sort(pointlist1.begin(), pointlist1.end(), cmp1);
            lx = (pointlist1[0].x + pointlist1[1].x)/2;
            ly = (pointlist1[0].y + pointlist1[1].y)/2;
            lightR.points(point);
            for(size_t i = 0;i<4;i++)
            {
                pointlist2.push_back(point[i]);
            }
            sort(pointlist2.begin(), pointlist2.end(), cmp1);
            rx = (pointlist2[2].x + pointlist2[3].x)/2;
            ry = (pointlist2[2].y + pointlist2[3].y)/2;
            cout << "lx:" << lx << " ly:" << ly << " rx:" << rx << " ry:" << ry << "\n" <<endl;
            if (abs(lightL.angle - lightR.angle) > 140 ) {
                rect = RotatedRect(Point2f((lx + rx)/2, (ly + ry)/2),Size2f(lightR.center.x - lightL.center.x, lightL.size.height), (180 - lightL.angle - lightR.angle)/2);
            }
            else {
                rect = RotatedRect(Point2f((lx + rx)/2, (ly + ry)/2),Size2f(lightR.center.x - lightL.center.x, lightL.size.height), (lightL.angle + lightR.angle)/2);
            }
            pointlist1.clear();
            pointlist2.clear();
            if (rx - lx <= 1 || ry -ly <= 1)
                continue;*/

            double depth = sqrt(pow(abs(lightR.center.y - lightL.center.y),4)) ;
            double x_2D = sqrt(pow(abs(lightR.center.x - lightL.center.x),2)+pow(abs(lightR.center.y - lightL.center.y),2));
            double width = sqrt(pow(x_2D,2)+pow(depth,2));
           // double width=fabs(lightL.center.x-lightR.center.x);
            if (abs(lightL.angle - lightR.angle) > 140 ) {
                rect = RotatedRect(Point2f((lightL.center.x + lightR.center.x)/2, (lightL.center.y + lightR.center.y)/2),Size2f(width, max(lightL.size.height, lightR.size.height)), (180 - lightL.angle - lightR.angle)/2);
            }
            else {
                rect = RotatedRect(Point2f((lightL.center.x + lightR.center.x)/2, (lightL.center.y + lightR.center.y)/2),Size2f(width, max(lightL.size.height, lightR.size.height)), min(lightL.angle , lightR.angle));
            }
            rect_choose.push_back(rect);
        }
    }
    if (!rect_choose.size())
    {
        printf("no rect_choose\n");
        return 0;
    }
#ifdef SHOW_DOUBLE_LIGHT
            Mat img1;
            RNG rng(time(0));
            Point2f vertex[4];
            base.src.copyTo(img1);
            for (size_t i = 0; i < rect_choose.size(); i++)
            {
                rect_choose[i].points(vertex);
                cout << "rect_choosexy:" << rect_choose[0].center.x << " " << rect_choose[0].center.y << endl;
                cout << "rect_choosehw:" << rect_choose[0].size.height << " " << rect_choose[0].size.width << endl;
                for (size_t i = 0; i < 4; i++)
                {
                    line(img1, vertex[i], vertex[(i + 1) % 4], Scalar(0, 120, 255), 1, LINE_AA);
                    cout << "point:"<< vertex[i].x << " " << vertex[i].y << "\n" << endl;
                }
               /* float lx = rect_choose[i].center.x - rect_choose[i].size.width/2;
                float ly = rect_choose[i].center.y - rect_choose[i].size.height/2;
                float rx = rect_choose[i].center.x + rect_choose[i].size.width/2;
                float ry = rect_choose[i].center.y + rect_choose[i].size.height/2;
                rectangle(img1, Point(lx,ly), Point(rx,ry), Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3, LINE_8, 0);*/
            }
            cout << "rect_choose: " << rect_choose.size() << endl;
           /* putText(img1, "0", Point(pointlist1[0].x , pointlist1[0].y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            putText(img1, "1", Point(pointlist1[1].x , pointlist1[1].y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            putText(img1, "2", Point(pointlist1[2].x , pointlist1[2].y + 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            putText(img1, "3", Point(pointlist1[3].x , pointlist1[3].y + 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);*/
#ifdef LARGE_SRC
            resize(img1, img1, Point(img1.rows * 0.7, img1.cols * 0.5));
#endif
            imshow("doublelight", img1);
            waitKey(1);
#endif
    return 1;
}
