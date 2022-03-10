#include"findbuff.h"
#include"pretreat.h"
#include"findCenter.h"
#include"Rotate.h"
#include"hitpredict.h"


bool buffrun(Mat SRC,Base *base1,RotatedRect &res_center){
    Mat src=SRC;
    Mat jpg;
    RotatedRect armor;
    RotatedRect CenterR;
    Point2f hit;


    pretreat a;
    findbuff b;
    findCenter c;
    Rotate d;
    hitpredict e;
    Base base2= *base1;



        Point2f dian[4];
        double t1 = getTickCount();

        a.run(src, jpg,base2);
        b.runfind(jpg, armor);
        c.runCenter(b.Centers, armor, CenterR);
        if (buffbase.is_direction==0) {
            std::cout << "是否判断  "<<buffbase.is_direction << endl;
            base1->armors.push_back(armor);
            buffbase.times.push_back(t1 / getTickFrequency());
            size_t i = base1->armors.size();
            if (i < 10) return false;
            cout<<"时间差"<<buffbase.times[i - 1] - buffbase.times[i - 3] <<endl;
            e.SmallorBig(base1->armors[i - 3].center, base1->armors[i - 1].center,buffbase.times[i-3],buffbase.times[i-1], CenterR.center);
            d.runRotate(base1->armors[i - 3], base1->armors[i - 1],CenterR);
            return false;
        }
        buffbase.anglespeeds.clear();
        buffbase.thts.clear();
        buffbase.times.clear();
        base1->armors.clear();
        if (buffbase.smallbuff == -1) {
            std::cout << "旋转方向 " << buffbase.direction << endl;
            std::cout << "小风车" << endl;
            armor.points(dian);
            cv::line(src, dian[0], dian[1], cv::Scalar(255, 0, 0), 3);
            cv::line(src, dian[1], dian[2], cv::Scalar(255, 0, 0), 3);
            cv::line(src, dian[2], dian[3], cv::Scalar(255, 0, 0), 3);
            cv::line(src, dian[3], dian[0], cv::Scalar(255, 0, 0), 3);
            circle(src, armor.center, 2, Scalar(255, 0, 0), 2);
            circle(src, CenterR.center, 6, Scalar(0, 255, 0), 2);
            line(src, armor.center, CenterR.center, Scalar(255, 0, 0));
            e.predict_smallbuff(CenterR.center, armor.center, hit,buffbase.direction,base2);
            circle(src, hit, 2, Scalar(0, 255, 0), 3);
            imshow("armor", src);
            waitKey(1);
        }

        else {
            if (e.tht == -1) {
                e.predictTime();
                e.judgetht();
            }
            std::cout << "旋转方向 " << buffbase.direction << endl;
            std::cout << "大风车" << endl;
            armor.points(dian);
            cv::line(src, dian[0], dian[1], cv::Scalar(255, 0, 0), 3);
            cv::line(src, dian[1], dian[2], cv::Scalar(255, 0, 0), 3);
            cv::line(src, dian[2], dian[3], cv::Scalar(255, 0, 0), 3);
            cv::line(src, dian[3], dian[0], cv::Scalar(255, 0, 0), 3);
            circle(src, armor.center, 2, Scalar(255, 0, 0), 2);
            circle(src, CenterR.center, 6, Scalar(0, 255, 0), 2);
            line(src, armor.center, CenterR.center, Scalar(255, 0, 0));
            e.predict_bigbuff(CenterR.center, armor.center, t1/getTickFrequency(), hit, buffbase.direction,base2);
            circle(src, hit, 2, Scalar(0, 255, 0), 3);
            imshow("armor", src);
            waitKey(1);
        }

        res_center=RotatedRect(hit,armor.size,0);

        t1 = getTickCount() - t1;
        t1 = t1 / getTickFrequency();
        std::cout << t1 << endl;
        //cout << e.tht << endl;



    return 0;

}

