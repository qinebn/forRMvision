#include "ArmorPredict.h"
#include"base.h"
#include"armor.h"
#include "math.h"
#include "cmath"


Point2f ArmorPredict::armordistance(Base base)
{
    //对x,y的拟合二次曲线
          float rectx[20] = {0};
          float recty[20] = {0};
          for (int i = 0; i < 4; i++) {
            rectx[i] = base.move_pre_rects[i].center.x;
            recty[i] = base.move_pre_rects[i].center.y;
            }
        X_now = MatrixXd(3, 3);
        X_now << rectx[1]* rectx[1], rectx[2]* rectx[2], rectx[3]*rectx[3],
                 rectx[1], rectx[2], rectx[3],
                 1, 1, 1;

        Y_now = MatrixXd(3,1);
        Y_now << rectx[1],
                 rectx[2],
                 rectx[3];

        Get1_now = MatrixXd(3, 1);
        Get1_now << a1,
                    b1,
                    c1;

        Get1_now = X_now.inverse() * Y_now;
        a1 = Get1_now(0, 0);
        b1 = Get1_now(0, 1);
        c1=  Get1_now(0, 2);
       // y=a1*x*x+b1*x+c1;

        //对距离帧数的二次曲线拟合
           double delta[10]={0};//距离
           double chazhi[2]={0};

                delta[0]=sqrt(pow(fabs(rectx[1]-rectx[0]),2)+pow(fabs(recty[1]-recty[0]),2));
                delta[1]=sqrt(pow(fabs(rectx[2]-rectx[1]),2)+pow(fabs(recty[2]-recty[1]),2));
                delta[2]=sqrt(pow(fabs(rectx[3]-rectx[2]),2)+pow(fabs(recty[3]-recty[2]),2));
                chazhi[0]= delta[1]-delta[0];
                chazhi[1]= delta[2]-delta[1];

            Z_now = MatrixXd(3, 3);
            Z_now << 1, 4, 9,
                     1, 2, 3,
                     1, 1, 1;

            D_now = MatrixXd(3, 1);
            D_now <<delta[0],
                    delta[1],
                    delta[2];

            Get1_now = MatrixXd(3, 1);
            Get1_now << a2,
                        b2,
                        c2;

            Get2_now = Z_now.inverse() * D_now;
            a2 = Get2_now(0, 0);
            b2 = Get2_now(0, 1);
            c2 = Get2_now(0, 2);
            double t=10;
            dispre=a2*t*t+b2*t+c2;

            //加速时
            if(fabs(chazhi[1]-chazhi[0])<10){
                dispre=1.5*dispre;
            }
            if(fabs(chazhi[1]-chazhi[0])<10&&fabs(chazhi[1]-chazhi[0])<20){
                dispre=2.5*dispre;
            }
            if(fabs(chazhi[1]-chazhi[0])>20){
                dispre=3*dispre;
            }

            //减速时
            if((chazhi[1]-chazhi[0])>-10&&(chazhi[1]-chazhi[0])<0){
                dispre=0.75*dispre;
            }
            if((chazhi[1]-chazhi[0])<-10&&(chazhi[1]-chazhi[0])>-20){
                dispre=0.5*dispre;
            }
            if((chazhi[1]-chazhi[0])<-20){
                dispre=0.25*dispre;
            }



            //dispre的分解,斜率直线
            double dispre_squa;
            double deno;

            dispre=pow(distance_x,2)+pow(distance_y,2);
            dispre_squa=pow(dispre,2);
            deno=4*pow(a1,2)+1;
            distance_x= sqrt(dispre_squa/deno);
            distance_y=distance_x*2*a1;


            //最后的预测
            armor_pre.x=rectx[4]+distance_x;
            armor_pre.y=a1*armor_pre.x*armor_pre.x+b1*armor_pre.x+c1;
           // armor_pre.y=recty[4]+distance_y;




        #ifdef  yuce3
        Mat src;
        base.src.copyTo(src);
        circle(src,Point(armor_pre.x ,armor_pre.y),5,Scalar(0,0,255));
        imshow("yuce3",src);
        #endif
        return Point2f(armor_pre.x, armor_pre.y);
}
















