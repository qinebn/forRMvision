#include "Threads.h"
#include "armor.h"
#include "light.h"
#include "base.h"
#include "AngleSolver.h"
#include "serial.h"
#include "streambuf"
#include <time.h>
#include "daheng.h"
#include"armorpredictrun.h"
#include"Gernal.h"
#include <pthread.h>
#include <stdio.h>
#include "predict.h"
#include"sentry/sentry.h"
#include "tuoluo.h"

#define has_none_mode 0x00
#define fire_sure 0x11
#define wait_state 0x22
#define up_ptz_pitch 0x33

pthread_spinlock_t spinlock;
pthread_mutex_t mutex;

void Threads::avgsita(double* yaw, double* pitch)    //求一段时间的绝对角度平均值
{
    int i,j;
    deque<double> sita_yaw = sita_yaw_avg;
    deque<double> sita_pitch = sita_pitch_avg;
    avg_time += last_t;
    move_yaw = 0;
    move_pitch = 0;
    cout << "sita_yaw_size: " << sita_yaw.size()<<endl;
    cout << "avg_count: " << avg_count << endl;
    if(avg_count < 2)
    {
        sita_yaw.push_back(*yaw);
        sita_pitch.push_back(*pitch);
        avg_count++;
    }
    else
    {
        for(i = 1;i < sita_yaw.size();i++)
        {
            move_yaw +=(sita_yaw.at(i) - sita_yaw.at(i-1));
            cout << i << " :sita_yaw: "<< sita_yaw.at(i) << endl;
        }
        *yaw = *yaw +(move_yaw / avg_time);//cjr_sb
        for(j = 1;j < sita_pitch.size();j++)
        {
            move_pitch +=(sita_pitch.at(j) - sita_pitch.at(j-1));
            cout << "sita_pitch: "<< sita_pitch.at(j) << endl;
        }
        *pitch = *pitch + (move_pitch / avg_time);
        cout << "move_yaw: "<< move_yaw << "move_pitch: " << move_pitch << endl;
        sita_yaw.clear();
        sita_pitch.clear();
        avg_count++;
    }
    sita_yaw_avg = sita_yaw;
    sita_pitch_avg = sita_pitch;
}

bool iflargejump(RotatedRect a, RotatedRect b)
{
    if(b.center.x - a.center.x > 50)
        return 1;
    if(a.center.y - a.center.y > 30)
        return 1;
    return 0;
}

void Threads::reset_kalman(Base *base)
{
    last_pitch = 0;
    last_yaw = 0;
    last_t = 0;
    pre_count = 0;
    base->pre_flag = 0;
    base->X = MatrixXd(4,1);
    base->X << 0,
               0,
               0,
               0;
    base->X_ = MatrixXd(4,1);
    base->X_<< 0,
               0,
               0,
               0;
    base->K = MatrixXd(4,4);
    base->K << 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1;
    base->P = MatrixXd(4,4);
    base->P << 1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1;
    base->P_ = MatrixXd(4,4);
    base->P_ <<1,0,0,0,
               0,1,0,0,
               0,0,1,0,
               0,0,0,1;
}

Threads::Threads()
{
    camera_count = 0;
    image_count = 0;
}

Threads::~Threads()
{}

void save_video(VideoWriter video, Mat image)
{
    video.write(image);
}

void Threads::change_exposure(Base &base)
{
    if(0 == base.Exposure_type)
    {
        if(base.armor_number > 10)
        {
            base.whitemore = 0;
            base.whiteless = 0;
        }
        if(base.whitemore > 3)
        {
            base.change = 1;
            dExposureValue -= 1000.0;
            base.whitemore = 0;
            base.whiteless = 0;
        }
        if(base.whiteless > 5)
        {
            base.change = 1;
            dExposureValue += 200.0;
            base.whitemore = 0;
            base.whiteless = 0;
        }
        printf("dExposureValue:%lf\n",dExposureValue);
        if (dExposureValue < 2000)
        {
            dExposureValue = 2000;
            exposure_low ++;
            if(exposure_low >= 15)
            {
                base.Exposure_type = 0;
                exposure_low = 0;
                exposure_high = 0;
            }
        }
        if (dExposureValue > 60000)
        {
            dExposureValue = 60000;
            exposure_high ++;
            if(exposure_high >= 15)
            {
                base.Exposure_type = 0;
                exposure_low = 0;
                exposure_high = 0;
            }
        }
    }
    else {
        dExposureValue = 6000;
        exposure_type1_count++;
        if(exposure_type1_count >= 1500)
        {
            exposure_type1_count = 0;
            base.Exposure_type = 1;
        }
    }
}

void Threads::camera_thread()//山川！！！！！
{
    printf("gp\n");
    daheng a;
    a.start(640,480);
    while(1)
    {
        while(camera_count - image_count >= 1)
        {
            //waitKey(1);
           // printf("camera:%d\n",camera_count);
        }
#ifdef SHOW_TIME
        double t= (double) cvGetTickCount();
#endif
        if(camera_count % 2 == 0 && camera_count > 3)
        {
            camera_count++;
            continue;
        }
          //  return;
        a.aProcGetImage(dExposureValue, &base.change);
        base.src = imread("pic.ppm");



#ifdef SHOW_CAMERA
        imshow("camera",base.src);
        waitKey(1);
#endif
#ifdef SAVE_VIDEO
        if(video_flag)
        {
            FILE *fp;
            size_t video_number;
            char video_r[100] = {0};
            char video_w[100] = {0};
            if (!(fp=fopen("video.txt","r")))
            {
                printf("Error in open file!\n");
            }
            fscanf(fp, "%d", &video_number);
            sprintf(video_r, "video%d.avi", video_number);
            fclose(fp);
            video = VideoWriter(video_r, CV_FOURCC('P','I','M','1'), 30, Size(640,480), 1);
            if(!video.isOpened())
            {
                printf("cant create video\n");
            }
            if(video_flag)
            {
                if (!(fp=fopen("video.txt","w")))
                {
                    printf("Error in open file!\n");
                }
                video_number++;
                sprintf(video_w, "%d", video_number);
                fputs(video_w, fp);
                fclose(fp);
                cout << "video: " << video_w << endl;
                video_flag--;
            }
        }
#endif
#ifdef SAVE_VIDEO
        save_video(video, base.src);//保存视频
#endif
        camera_count++;
        if(camera_count >= 99900)
        {
            camera_count = 1;
            image_count = 0;
        }
#ifdef SHOW_TIME
        t=(double)cvGetTickCount() -t;
        t=t/(cvGetTickFrequency()*1000);
        printf("camera Use time is: %gms\n", t);
#endif
    }
    return;
}
 /*   daheng a;
        a.start();
    while(1)
    {
    Mat frame = imread("pic.ppm");
    imshow("Camera",frame);
    a.aProcGetImage();
    char key = waitKey(1);
        if (key==27) break;
    }
}*/










void Threads::image_thread()
{
#ifdef VIDEO
        VideoCapture cap;
        cap.open("video2.avi");
        if (!cap.isOpened())
        {
            std::cout << "video not open." << std::endl;
            return ;
        }
#endif
    while(1)
    {
        cout<<"-------------------------------------------------------------------------"<<endl;
#ifdef SHOW_TIME
#endif
        double all_t= (double) cvGetTickCount();
#ifdef VIDEO
        cap >> base.src;
#endif
        //base.src = imread("pic.bmp");
#ifdef VIDEO
        cout << "c" << endl;
        imshow("Camera",base.src);
        cout << "d" << endl;
#endif
    //    yuc predict;
#ifdef CAMERA
        while(camera_count - image_count < 1)
        {
        }
#endif
#ifdef SHOW_TIME
        double t1= (double) cvGetTickCount();
#endif

        Light light(base.src, base);
        Armor armor;
        if (light.flag)
        {
            armor.start(light, base);
        }
        change_exposure(base);
        if(armor.flag )
        {
            printf("armor yes!\n");\
            cout << base.getYaw<< endl;
            shaobing = 0;
            base.armor_number++;
            back=0;
        }
        else
        {
            printf("armor no!\n");

             base.armor_number = 0;
            image_count++;
#ifdef VIDEO
   //     waitKey(300);
#endif
            double gyro_yaw = base.getYaw;
            if((gyro_yaw - 180) >= 0.1) gyro_yaw -= 360;
            cout << gyro_yaw<< endl;
            port_RUN(gyro_yaw,0.00,has_none_mode,&base);
            continue;
        }
        //image_count++;

        //角度结算
       double yaw,pitch,evaluateDistance;
       double distance = base.distance;
       AngleSolver Abbs;
       vector<Point2f> target2d,targetCenter;
       ArmorType teptype;
       size_t predict_count = 1;
       Tuoluo tuoluo_1;

       //Abbs.receLastarmor(base.lastarmor);
       if(base._temple_type ==0)
       {
           teptype=SMALL_ARMOR;
       }
       else if(base._temple_type==1){
           teptype=BIG_ARMOR;
       }
       Predict pre;
//       if(1 == base.pre_flag && (base.lastarmor.center.x > 540 || base.lastarmor.center.x < 100) || iflargejump(prev_armor, base.lastarmor))


       if((base.lastarmor.center.x > 540 || base.lastarmor.center.x < 100) )
       {
           cout<<"predict start....................................................!"<<endl;
             // pre.pre_nov(&base, last_pitch, last_yaw);
       }

       if(base.distance <= 8000)
       {
           //                            _ooOoo_
           //                           o8888888o
           //                           88" . "88
           //                           (| -_- |)
           //                           O\  =  /O
           //                        ____/`---'\____
           //                      .'  \\|     |//  `.
           //                     /  \\|||  :  |||//  \
           //                    /  _||||| -:- |||||-  \
           //                    |   | \\\  -  /// |   |
           //                    | \_|  ''\---/''  |   |
           //                    \  .-\__  `-`  ___/-. /
           //                  ___`. .'  /--.--\  `. . __
           //               ."" '<  `.___\_<|>_/___.'  >'"".
           //              | | :  `- \`.;`\ _ /`;.`/ - ` : | |
           //              \  \ `-.   \_ __\ /__ _/   .-` /  /
           //         ======`-.____`-.___\_____/___.-`____.-'======
           //                            `=---='

               Abbs.getAngle(base.lastarmor,base.lastarmor.center, teptype,  yaw, pitch, evaluateDistance, base.getSpeed, base.getPitch);




               int t_dis = distance/18000;
               yaw = -yaw;
#ifdef SHOW_PTZ
               cout << "base.getPitch:：：：：：：：：：：：：：：：：：：：：： " << '\n';
               cout << base.getPitch << '\n';
               cout << base.getYaw<< '\n';
               cout<<base.getSpeed<<endl;
               cout << base.getPitch_angle<< '\n';
               cout << base.getYaw_angle<< '\n';
               cout << pitch<< '\n';
               cout << yaw<< '\n';
               cout << "pre_count" << pre_count << '\n';
               //cout << "all_y" << base.getYaw-yaw << '\n';
#endif
               //all_yaw = base.getYaw + yaw + move_yaw;
               all_yaw = base.getYaw + yaw;
               all_pitch = base.getPitch - pitch;
               avgsita(&all_yaw, &all_pitch);
               if(avg_count > 2)
               {
                     pre.start(0.1 + t_dis,all_pitch,all_yaw,
                            &last_pitch,&last_yaw,&avg_time,&pre_count,&base,&last_vp,&last_vy, &maoqiaqia);
                     cout << "avg_time: " << avg_time << endl;
                     avg_count = 0;
                     avg_time = 0;
                     pre_pitch = pre._pre_pitch;
                     pre_yaw = pre._pre_yaw;
               }

              // bool tuoluo_status1 = tuoluo_1.start(&time_count, &tuoluo, &tuoluo_count, &tuoluo_status, &tuoluo_start, &last_yaw, &last_sita_y, &sita_yaw_tuoluo);
//               if(pre_count < 70)
                cout << "pre_yaw: "<< pre_yaw << endl;
                // if(1 == tuoluo_status1)
               if(0 == pre_yaw)
               {
                   double gyro_yaw = base.getYaw;
                   if((gyro_yaw - 180) >= 0.1) gyro_yaw -= 360;
                   port_RUN(gyro_yaw,0.00,has_none_mode,&base);
               }
               else port_RUN(pre_yaw,pitch,has_none_mode,&base);
//               else{
             cout<<"waibu   X"<<base.X<<endl;
             cout<<"waibu   X_"<<base.X_<<endl;




//            yuc predict;
//           //预测
//            AngleSolver Abbs1;
//               if(predict.run(&base, &center, &distance)){
//                   base.lastarmor.center = center;
//                   cout<<"base.lastarmor.center"<<base.lastarmor.center<<endl;
//                   base.move_pre_rects.clear();
//                   base.move_pre_dis.clear();
//                   base.move_pre_rects1.clear();
//                   base.lastarmor.center = Point2f( base.lastarmor.center.x, base.lastarmor.center.y);
//                   Abbs1.getAngle(base.lastarmor,base.lastarmor.center, teptype,  yaw, pitch, evaluateDistance,base.getSpeed, base.getPitch);
//                   if(fire_flag){
//                           port_RUN(-yaw,pitch,fire_sure,&base);
//                   }
//                   else{
//                           port_RUN(-yaw,pitch,has_none_mode,&base);
//                   }
//                  port_RUN(-yaw,pitch,fire_sure,&base);
//                }


/*          if(pre_flag == 1)
           {
{
               if(predict.run(&base, &center, &distance)){
                   base.lastarmor.center = center;
                   base.move_pre_rects.clear();
                   base.move_pre_dis.clear();
                   base.move_pre_rects1.clear();
                   /*cout<<"zhihong!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!~~~~~~~~~~~~~~~~~~~"<<endl;
           exit(0);*/
/*               base.lastarmor.center.x = base.lastarmor.center.x + 25 *(base.lastarmor.center.x - center.x);
               base.lastarmor.center.y = base.lastarmor.center.y + 10 *(base.lastarmor.center.y - center.y);*/
 /*                  cout << "base.lastarmor.center " << base.lastarmor.center << endl;
                   Abbs.getAngle(base.lastarmor,base.lastarmor.center, teptype,  yaw, pitch, evaluateDistance);
                   cout << "yaw " << yaw  <<"pit " << pitch << endl;
                   //waitKey(3000);
                   (port_RUN(yaw,pitch,3.00));
                   pre_flag = 1;
               }
           }
       }
      /*center = base.lastarmor.center;*/

               }
       all_t=(double)cvGetTickCount() -all_t;
       all_t=all_t/(cvGetTickFrequency()*1000);
#ifdef SHOW_TIME
        t1=(double)cvGetTickCount() -t1;
        t1=t1/(cvGetTickFrequency()*1000);
        printf("image Use time is: %gms\n", t1);
        printf("all Use time is: %gms\n", all_t);
#endif
        last_t = all_t;
 //        waitKey(300);
        image_count++;
    }
}



