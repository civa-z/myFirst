#ifndef _OPENCV_LIB_H_
#define _OPENCV_LIB_H_

#include <opencv2\opencv.hpp>

#ifdef _DEBUG

#pragma comment(lib, "../../opencv249/lib/opencv_imgproc249d.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_core249d.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_highgui249d.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_ml249d.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_photo249d.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_video249d.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_objdetect249d.lib")

#else

#pragma comment(lib, "../../opencv249/lib/opencv_imgproc249.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_core249.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_highgui249.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_ml249.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_photo249.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_video249.lib")
#pragma comment(lib, "../../opencv249/lib/opencv_objdetect249.lib")

#endif

#endif //_OPENCV_LIB_H_
