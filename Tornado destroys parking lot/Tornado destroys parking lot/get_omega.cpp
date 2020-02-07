#include "get_omega.h"

#define PI 3.1415927
#define DISTENCE 8000.0

//#define DEBUG

void get_omega::big_buff(Mat& image, int show_num, float angle, struct logo final_center_R)
{
	float delta_x = 0;
	float delta_theta = 0;
	Point2f point0(0, 0);

	if (direction == -1)
	{
		if (show_num == 1)
		{
			target_angle1 = angle;
		}
		if (show_num == 7)
		{
			target_angle2 = angle;
			dangle = target_angle2 - target_angle1;
			if (dangle < -200 || dangle > 0)
			{
				direction = 1;
			}
			else
			{
				direction = 0;
			}
		}
	}

	time0 = ((double)getTickCount() - time0) / getTickFrequency();//停止计时
	if (time0 > 1 || time0 < 0)
	{
		time0 = static_cast<double>(getTickCount());//重新开始计时	
		return;
	}

	time1 = time1 + time0;
	
	delta_theta = get_delta_x(time1);
	
	time0 = static_cast<double>(getTickCount());//重新开始计时	

	point0 = forecast_point(angle + delta_theta, final_center_R);

#ifdef DEBUG

	circle(image, point0, 1, Scalar(0, 255, 0), 5);
	line(image, Point(0, 240), Point(894, 240), Scalar(124, 200, 150), 1);
	line(image, Point(427, 0), Point(427, 480), Scalar(124, 200, 150), 1);
	line(image, point0, Point(427, 240), Scalar(200, 150, 26), 1);


#endif // DEBUG

	//得到摄像头中心到达预测点所需要的角度
	float delta_x1 = point0.x - frame_center_x;//得到x方向上的象素距离
	float delta_y1 = -(point0.y - frame_center_y);
	float distance_x = delta_x1 / final_center_R.r * 700;//得到x方向的实际距离
	float distance_y = delta_y1 / final_center_R.r * 700;

	float yaw = atan(distance_x / DISTENCE) / PI * 180;
	float pitch = atan(distance_y / DISTENCE) / PI * 180;

	
	cout << "yaw = " << yaw;
	cout << "    pitch = " << pitch << endl;

	drawgraph(yaw, pitch, show_num);

	imshow("graph_x", graph_x);
	imshow("graph_y", graph_y);
}


Point get_omega::forecast_point(float angle, struct logo R)
{
	Point2f point;

	if (direction == 1)//顺时针情况
	{
		point.x = R.x + R.r * sin(angle / 180 * PI);
		point.y = R.y - R.r * cos(angle / 180 * PI);

	}
	else//逆时针情况
	{
		point.x = R.x + R.r * sin(angle / 180 * PI);
		point.y = R.y - R.r * cos(angle / 180 * PI);
	}
	return point;
}

void get_omega::drawgraph(float point_x, float point_y, int show_num)
{
	graph_x.create(Size(1400, 480), CV_8UC3);
	graph_y.create(Size(1400, 480), CV_8UC3);

	//Point point_1(show_num, 480 - point_x * 480 / (frame_center_x * 2));//point.x
	//Point point_2(show_num, 480 - point_y * 480 / (frame_center_y * 2));//point.y

	//Point point_1(show_num, 480 - point_x * 480 / 360);//angle

	Point point_1(show_num, 100 - point_x);//yaw
	Point point_2(show_num, 100 - point_y);//pitch

	point1.push_back(point_1);
	point2.push_back(point_2);

	if (point1.size() > 1)
	{
		line(graph_x, point1[point1.size() - 2], point1[point1.size() - 1], Scalar(0, 0, 255));
		line(graph_y, point2[point2.size() - 2], point2[point2.size() - 1], Scalar(0, 255, 0));
	}
}