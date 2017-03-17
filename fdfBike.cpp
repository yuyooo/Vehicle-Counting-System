#include "opencv2/opencv.hpp"
#include<iostream>
using namespace std;
using namespace cv;

Mat MoveDetect(Mat frame1, Mat frame2)
{
	Mat result = frame2.clone();
	Mat gray1, gray2;
	cvtColor(frame1, gray1, CV_BGR2GRAY);
	cvtColor(frame2, gray2, CV_BGR2GRAY);

	Mat diff;
	absdiff(gray1, gray2, diff);
	imshow("absdiss", diff);
	threshold(diff, diff, 45, 255, CV_THRESH_BINARY);
	imshow("threshold", diff);
 
	Mat element = getStructuringElement(MORPH_RECT, Size(1, 1));
	Mat element2 = getStructuringElement(MORPH_RECT, Size(7, 7));
	erode(diff, diff, element);
	imshow("erode", diff);
 
	dilate(diff, diff, element2);
	imshow("dilate", diff);

	vector<vector<Point>> contours;  
	vector<Vec4i> hierarcy;
	findContours(diff, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE); //查找轮廓
	vector<Rect> boundRect(contours.size()); //定义外接矩形集合
	//drawContours(img2, contours, -1, Scalar(0, 0, 255), 1, 8);  //绘制轮廓
	int x0=0, y0=0, w0=0, h0=0;
	for(int i=0; i<contours.size(); i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]); //查找每个轮廓的外接矩形
	
		x0 = boundRect[i].x;  //获得第i个外接矩形的左上角的x坐标
		y0 = boundRect[i].y; //获得第i个外接矩形的左上角的y坐标
		w0 = boundRect[i].width; //获得第i个外接矩形的宽度
		h0 = boundRect[i].height; //获得第i个外接矩形的高度
		rectangle(result, Point(x0, y0), Point(x0+w0, y0+h0), Scalar(0, 255, 0), 2, 8); //绘制第i个外接矩形
	}
	return result;	
}

void main()
{	 
	VideoCapture cap("bike.avi");
    if(!cap.isOpened()) //检查打开是否成功
         return;
    Mat frame;
	Mat result;
	Mat temp;
	int count=0;
    while(1)
    {
        cap>>frame;
        if(!frame.empty())
        {
			count++;
			if(count==1)
				 result = MoveDetect(frame, frame);
			else
				 result = MoveDetect(temp, frame);
			imshow("video", frame);
			imshow("result", result);
			temp = frame.clone();
            if(waitKey(50)==27)
               break;
        }
        else
            break;
    }
    cap.release();  
}
