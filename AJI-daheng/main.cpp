
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "armor/light.h"
#include "armor/armor.h"
#include <QTime>
#include <QDebug>
#include <thread>
#include "Threads.h"

//#include "allprocess.h"
//#include "inputchoose.h"

using namespace std;
using namespace cv;

int main()
{
   Threads Th;
#ifdef CAMERA
   std::thread camera_(&Threads::camera_thread, &Th);
#endif
   std::thread image_(&Threads::image_thread, &Th);
#ifdef CAMERA
   camera_.join();
   waitKey(10);
#endif
   image_.join();
   waitKey(0);
//   while(1);
   return 0;
}
