#include "opencv2/opencv.hpp"
#include<iostream>
using namespace std;
using namespace cv;

Mat MoveDetect(Mat background, Mat img)
{
	Mat result = img.clone();
	Mat gray1, gray2;
	cvtColor(background, gray1, CV_BGR2GRAY);
	cvtColor(img, gray2, CV_BGR2GRAY);

	Mat diff;
	absdiff(gray1, gray2, diff);
	//imshow("absdiss", diff);
	threshold(diff, diff, 45, 255, CV_THRESH_BINARY);
	//imshow("threshold", diff);
 
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element2 = getStructuringElement(MORPH_RECT, Size(15, 15));
	erode(diff, diff, element);
	//imshow("erode", diff);
 
	dilate(diff, diff, element2);
	//imshow("dilate", diff);

	vector<vector<Point>> contours;  
	vector<Vec4i> hierarcy;
	findContours(diff, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE); //��������
	vector<Rect> boundRect(contours.size()); //������Ӿ��μ���
	//drawContours(img2, contours, -1, Scalar(0, 0, 255), 1, 8);  //��������
	int x0=0, y0=0, w0=0, h0=0;
	for(int i=0; i<contours.size(); i++)
	{
		boundRect[i] = boundingRect((Mat)contours[i]); //����ÿ����������Ӿ���
	
		x0 = boundRect[i].x;  //��õ�i����Ӿ��ε����Ͻǵ�x����
		y0 = boundRect[i].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
		w0 = boundRect[i].width; //��õ�i����Ӿ��εĿ��
		h0 = boundRect[i].height; //��õ�i����Ӿ��εĸ߶�
		rectangle(result, Point(x0, y0), Point(x0+w0, y0+h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���
	}
	return result;
}

void main()
{	 
    VideoCapture cap("bike.avi");
    if(!cap.isOpened()) //�����Ƿ�ɹ�
         return;
    Mat frame;
	Mat background;
	Mat result;
	int count=0;
    while(1)
    {
        cap>>frame;
        if(!frame.empty())
        {
			count++;
			if(count==1)
				background = frame.clone(); //��ȡ��һ֡Ϊ����֡
            imshow("video", frame);
			result = MoveDetect(background, frame);
			imshow("result", result);
            if(waitKey(50)==27)
               break;
        }
        else
            break;
    }
    cap.release();  


	
}