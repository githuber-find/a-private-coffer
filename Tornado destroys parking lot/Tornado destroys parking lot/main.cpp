#include<opencv2/opencv.hpp>
#include"opencv.hpp"
#include "get_omega.h"

buff_detect buff;
get_omega GetOmega;

using namespace cv;
using namespace std;

void energy(VideoCapture capture);

int main()
{
	VideoCapture capture("C:\\Users\\86184\\Desktop\\视频\\能量机关\\blue3.mp4");
	GetOmega.frame_center_y = capture.get(CAP_PROP_FRAME_HEIGHT) / 2;
	GetOmega.frame_center_x = capture.get(CAP_PROP_FRAME_WIDTH) / 2;

	energy(capture);
	
	waitKey(0);

}

void energy(VideoCapture capture)
{
	Mat frame, image, dst;
	int color = 0;
	int target_num = 0;
	int threshValue = 0;
	int show_num = 0; 
	float angle = 0;
	logo center_R;

	buff.read_xml(color, threshValue);

	capture >> frame;
	dst.create(frame.size(), CV_8UC1);

	namedWindow("效果");
	createTrackbar("阈值", "效果", &threshValue, 255);	
	
	while (1)
	{
		capture >> frame;
		if (frame.empty())
		{
			break;
		}
		frame.copyTo(image);
		
		//直接取单颜色通道并阈值化
		buff.color_buff(image, dst, threshValue, color);

		//执行图像处理及能量机关的识别任务		image 原图像    dst颜色通道相减
		target_num = buff.BuffDetectTask(image, dst, angle, center_R);

		if (target_num != 0 )
		{
			if (show_num == 1)
			{
				GetOmega.time0 = static_cast<double>(getTickCount());//开始计时
			}

			GetOmega.big_buff(image, show_num, angle, center_R);//预测下一点的位置
			
			show_num++;
		}

		imshow("效果", image);
		imshow("二值图", dst);

		waitKey(10);
		
	}
}

