#pragma once
#include <opencv2/opencv.hpp>
#include "buff_detect.h"
#define PI 3.1415927

using namespace cv;
using namespace std;

class get_omega
{
public:
	
	int frame_center_y = 0;
	int frame_center_x = 0;
	float time0 = 0;

	Point forecast_point(float angle, struct logo R);
	void big_buff(Mat& image, int show_num, float angle, struct logo final_center_R);
	void drawgraph(float point_x, float point_y, int show_num);

private:

	float target_angle1 = -1;
	float target_angle2 = 0;
	float dangle = 0;
	int direction = -1;
	float time1 = 0;
	float T = 0.5;//1s

	Mat graph_x, graph_y;
	vector<Point> point1;
	vector<Point> point2;
	Point2f point;

	float get_delta_x(float time)
	{
		float x = 0;
		x = -0.785 / 1.884 * cosf(1.884 * (time + T)) + 1.305 * (time + T)+
			0.785 / 1.884 * cosf(1.884 * time) - 1.306 * time;//对速度进行定积分

		return x * 360 / (2 * PI); //x的单位为rad 1rad == 360 / (2 * PI)
	}
};

