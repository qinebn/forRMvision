#include "shootTuoLuo.h"
#include "algorithm"
#include<math.h>

//���ݵõ��ľ�����һ˲��ı��ж���С����״̬
bool shootTuoLuo::StateRecognition(Base *base)
{
    int m=0;
    int n=0;
    for (int i = 0;i < 5;i++) {//ѭ��5���ж�
        for (int j = 1;j < 5;j++) {//ѹ��25֡��ȥ
            //base->move_pre_rects.push_back(rect);
            if ((base->move_pre_TL[j].center.x - base->move_pre_TL[j - 1].center.x) > 100){
                m++;

            }
            if ((base->move_pre_TL[j].center.x - base->move_pre_TL[j - 1].center.x) < -100){
                n++;
            }
        }
    }
    if((m=0&&n>0)||(n=0&&m>0))return false;
}



//bool shootTuoLuo::StateRecognition(Base *base) {
//  for (int i = 0; i < 11; i++) {
//       base.move_pre_rects.push_back(rect);
//       if (fabs(base->move_pre_rects[i].center.x - base->move_pre_rects[i + 1].center.x) >50) return false;
//       if(){}

//   }

//}
		
//���x�������ֵ���ó�����
/*        void shootTuoLuo::TuoLuoPredict(Base base)
{
			int sum=0;
			int max = 0;
			int min =9999;
			float x[60];
			if(bool StateRecognition = false)
			for (int i = 0;i < 60;i++) {
                x[i] = base.move_pre_rects[i].center.x;
				sort(x,x+60);
				max = x[0];
				min = x[59];
			}
            base.move_pre_rects[61].center.x = max - min;
			for (int i = 0;i < 60;i++) {
				
                sum += base.move_pre_rects[i].center.x;
                base.move_pre_rects[61].center.x = sum / 60;
			}
}*/



Point2f shootTuoLuo::TuoLuoPredict(Base *base)
{
    	int sum=0;
   	int max = 0;
    	int min =0;
    	float x[25]={0};
    	for (int i = 0;i < 25;i++) {
        x[i] = base->move_pre_TL[i].center.x;
        sort(x,x+25);
        max = x[0];
        min = x[24];
    	}
    	armor_pre.x = (max - min)/2;
    	for (int i = 0;i < 25;i++) {
        sum += base->move_pre_TL[i].center.y;
   	 }
     	armor_pre.y = sum / 25;
	armor_pre.x=fabs(armor_pre.x);
	armor_pre.y=fabs(armor_pre.y);







//    armor_pre.x = fabs(base->move_pre_rects[11].center.x);
//    armor_pre.y = fabs(base->move_pre_rects[11].center.y);
//    cout << " tuoluox:" << armor_pre.x;
//    cout << " tuoluoy:" << armor_pre.y << endl;

#ifdef yuce2
    Mat src;
    base->src.copyTo(src);
    circle(src,Point(armor_pre.x ,armor_pre.y),5,Scalar(0,0,255));
    imshow("yuce2",src);
#endif
    return Point2f(armor_pre.x, armor_pre.y);
}
