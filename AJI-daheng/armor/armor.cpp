#include "light.h"
#include "armor.h"
#include <QTime>
#include "AngleSolver.h"

int total = 0;
int n=0;
char* cstr = new char[120];

bool truerect(RotatedRect rect)
{
    if(rect.size.width / rect.size.height > 1.0 && rect.size.width / rect.size.height < 4.0 )
        return 1;
    else return 0;
}
void Armor::start(Light& light, Base& base)
{
    flag = istruetemple(light, base);
}

void savenumber(const Mat src1)
{
    Mat dst1;
    src1.copyTo(dst1);
    resize(dst1,dst1,Size(100,100));
    sprintf(cstr,"%d%s",n++,".jpg");
    imwrite(cstr ,dst1);
}

void Gamma(Mat& src, Mat& dst, float fGamma)        //Gamma提升亮度
{
    unsigned char lut[256];
    for (int i = 0; i < 256; i++)
    {
        lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
    }
    dst = src.clone();
    const int channels = dst.channels();
    MatIterator_<uchar> it, end;
    for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
        *it = lut[(*it)];
}

void Gamma_normal(Base base, Mat& gray)
{
    size_t i =0;
    for(i = 0; i < 2; i++)
    {
        Gamma(gray, gray, base.gamma);
    }
}

bool Armor::islargetemple(RotatedRect rect)
{
    if(rect.size.width/1.0/rect.size.height >= 2.5) return 1;
    else return 0;
}

bool Armor::isnumber(RotatedRect rect , Base& base)		//扫描数字区域来判断
{
    bai = 0;
    sum = 0;
    int width = rect.size.width/2;
    int height = rect.size.height*2;
    if(rect.center.x - rect.size.width/4 <= 0 || rect.center.y - rect.size.height <= 0)
        return 0;
    if(rect.center.x + rect.size.width/2 > base.gray.cols)
        width = base.gray.cols - rect.center.x;
    if(rect.center.y + rect.size.height*2 > base.gray.rows)
        height = base.gray.rows - rect.center.y;
    Rect gray_rect = Rect(rect.center.x - rect.size.width/4, rect.center.y - rect.size.height*2/3, width , height*2/3);
    Mat gray = base.gray(gray_rect);
    resize(gray,gray,Size(50,50));
    Gamma_normal(base,gray);
/*  for (size_t i = 0; i < 50; i++)		//计算白色比值确定数字
    {
        for (size_t j = 0; j < 50; j++)
        {
            if ((int)gray.at<uchar>(i, j) >= 0)
            printf("%3d",(int)gray.at<uchar>(i, j));
        }
        printf("\n ");
    }
    printf("\n ");

    waitKey(5000);*/

#ifdef SHOW_GRAY_NUMBER_IMAGE
    namedWindow("basegray",WINDOW_NORMAL);
    imshow("basegray",gray);
#endif

    threshold(gray, gray, base.white_threshould, 255, CV_THRESH_BINARY);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3 ,3), Point(-1, -1));
    erode(gray, gray, kernel);
    for (size_t i = 0; i < 50; i++)		//计算白色比值确定数字
    {
        for (size_t j = 0; j < 50; j++)
        {
            sum++;
            if ((int)gray.at<uchar>(j, i) > 0) bai++;
            //printf("%d ",(int)gray.at<uchar>(j, i));
        }
    }


#ifdef	SHOW_WHITE_IMAGE
    namedWindow("gray", CV_WINDOW_NORMAL);
    Mat img;
    base.src.copyTo(img);
    Rect rect1 = gray_rect;
    rectangle(img, rect1, Scalar(255, 0, 0), 3, LINE_8);
#ifdef LARGE_SRC
    resize(img, img,
    Point(img.rows * 0.3, img.cols * 0.15));
#endif
    if(!bai)
        cout << "bai = 0" << endl;
    else
    {
        cout << "bai " << bai << endl;
        cout << "sum: " << sum << endl;
        cout << "bai/sum: " << (sum / 1.0 / bai ) << endl;
    }
#endif
    if(0 == base.Exposure_type)
    {
        if (0 == bai)
        {
            base.whiteless++;
            return 0;
        }
        if ((sum / 1.0 / bai) > base.white_pixel)
        {
            base.whiteless++;
            return 0;
        }
        if((sum / 1.0 / bai ) < 1.25)
        {
            base.whitemore++;
            return 0;
        }
    }
    else
    {
        if (0 == bai)
        {
            base.whiteless++;
            return 0;
        }
        if((sum / 1.0 / bai ) < 1.25)
        {
            base.whitemore++;
            return 0;
        }
    }
#ifdef SHOW_WHITE_IMAGE
    imshow("gray", gray);
    cv::imshow("isnumber", img);
#endif
    return 1;
    //savenumber(gray);
}

bool Armor::istruetemple(Light& light, Base& base)		//检测是否有数字来判断是否是装甲板
{
    armor_rect.clear();
    for (size_t i = 0; i < light.rect_choose.size(); i++)
    {
        RotatedRect rect = light.rect_choose[i];
        if(!truerect(rect)) continue;
        if (isnumber(rect, base))
        {
            armor_rect.push_back(rect);
        }
    }
    if(armor_rect.size() < 1)
    {
        base.lost_cnt++;
        return 0;
    }
    if (armor_rect[0].size.width > 0)
    {
        base.lastarmor = choosetemple(&base);
        if(1 == base._temple_type)
        {
            double angle = base.lastarmor.angle;
            if(angle > 1.5 && angle < 5 || 180-angle > 1.5 && 180-angle < 5)
            {
                 base.lastarmor.size.width =  base.lastarmor.size.width *1.05;
            }
        }
        //base.armor = armor_rect;
        /*if(1 == base.last_roi)
        {
            base.lost_cnt = 0;
            base.lastarmor.center.x = armor_rect[0].center.x + 200;
            base.lastarmor.center.y = armor_rect[0].center.y ;
            base.lastarmor.size.width = armor_rect[0].size.width;
            base.lastarmor.size.height = armor_rect[0].size.height;
        }
        if(2 == base.last_roi)
        {
            base.lost_cnt = 0;
            base.lastarmor.center.x = armor_rect[0].center.x + 600;
            base.lastarmor.center.y = armor_rect[0].center.y ;
            base.lastarmor.size.width = armor_rect[0].size.width;
            base.lastarmor.size.height = armor_rect[0].size.height;
        }*/
    }
    else base.lost_cnt++;
    //getlasttemple(base);

#ifdef SHOW_RESULT_ARMOR
    Mat img;
    RNG rng(time(0));
    Point2f vertex[4];
    base.src.copyTo(img);
    cout << "armor_rect.size: " << armor_rect.size() <<  endl;
    if (armor_rect.size())
        for (size_t i = 0; i < armor_rect.size(); i++)
        {
            //armor_rect[i].points(vertex);
            base.lastarmor.points(vertex);
            for (size_t i = 0; i < 4; i++)
            {
                line(img, vertex[i], vertex[(i + 1) % 4], Scalar(0, 120, 255), 1, LINE_AA);
            }
            putText(img, "0", Point(vertex[0].x , vertex[0].y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            putText(img, "1", Point(vertex[1].x , vertex[1].y - 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            putText(img, "2", Point(vertex[2].x , vertex[2].y + 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            putText(img, "3", Point(vertex[3].x , vertex[3].y + 50), cv::FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2, 1);
            /*float lx = armor_rect[i].center.x - armor_rect[i].size.width/2;
            float ly = armor_rect[i].center.y - armor_rect[i].size.height/2;
            float rx = armor_rect[i].center.x + armor_rect[i].size.width/2;
            float ry = armor_rect[i].center.y + armor_rect[i].size.height/2;
            rectangle(img, Point(lx,ly), Point(rx,ry), Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), 3, LINE_8);*/
            cout << "lastarmor_rect" << "x: " << base.lastarmor.center.x << " y: " << base.lastarmor.center.y << " width: " << base.lastarmor.size.width << " height: " << base.lastarmor.size.height << " angle: " << base.lastarmor.angle << endl;
        }
    else
    {
        cout << "no true temple" << endl;
        cout << "rect_choose: " << light.rect_choose.size() << endl;
    }
#ifdef LARGE_SRC
    resize(img, img, Point(img.rows * 0.7, img.cols * 0.5));
#endif
    resize(img, img, Point(640, 480));cv::imshow("istruetemple", img);
    waitKey(1);
#endif
    if (armor_rect.size())
        return 1;
    else
        return 0;
}

RotatedRect Armor::choosetemple(Base *base)
{
    double yaw = 0,pitch = 0;
    double distance;
    int flag_ = 0;
    ArmorType teptype;
    for(size_t i = 0 ; i < armor_rect.size() ; i++)
    {
        AngleSolver agr;
        //调用距离api
        base->_temple_type = islargetemple(base->lastarmor);
        if(base->_temple_type ==0)
        {
            teptype=SMALL_ARMOR;
        }
        else {
            teptype=BIG_ARMOR;
        }
        agr.getAngle(armor_rect[i], armor_rect[i].center, teptype, yaw, pitch, distance, 0, 0);
        if(abs(distance_ - distance) <= 200)
        {
            if(abs(base->lastarmor.center.x - armor_rect[i].center.x)<= 40 &&
               abs(base->lastarmor.center.y - armor_rect[i].center.y)<= 30)
            {
                flag_ = i;
                distance_ = distance;
                break;
            }
        }
        if(distance_ > distance && distance > 100)
        {
            flag_ = i;
            distance_ = distance;
        }
    }
    base->distance = distance_;
    return armor_rect[flag_];
}

bool Armor::getlasttemple(Base &base)
{

}
