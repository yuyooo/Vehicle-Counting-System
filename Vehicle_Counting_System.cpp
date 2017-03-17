#include "opencv2/opencv.hpp"
#include<iostream>
using namespace std;
using namespace cv;

int CarNum = 0;
Mat MoveDetect(Mat frame1, Mat frame2){
	Mat result = frame2.clone();
	Mat gray1, gray2;
	cvtColor(frame1, gray1, CV_BGR2GRAY);
	cvtColor(frame2, gray2, CV_BGR2GRAY);

	Mat diff;
	absdiff(gray1, gray2, diff);
	//imshow("absdiss", diff);
	//threshold(diff, diff, 45, 255, CV_THRESH_BINARY); //�򵥶�ֵ��
	adaptiveThreshold(diff, diff, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 3);
	imshow("threshold", diff);
 

	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element2 = getStructuringElement(MORPH_RECT, Size(31, 31));
	//������̬ѧ����
	morphologyEx(diff, diff, MORPH_OPEN, element);
	medianBlur(diff, diff, 9);
	dilate(diff, diff, element2);
	imshow("dilate", diff);
	vector<vector<Point>> contours;  
	vector<Vec4i> hierarcy;
	findContours(diff, contours, hierarcy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE); //��������
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size()); //������Ӿ��μ���

	//drawContours(img2, contours, -1, Scalar(0, 0, 255), 1, 8);  //��������
	int x0=0, y0=0, w0=0, h0=0;
	char Num[10];
	for(int i=0; i<contours.size(); i++){
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true); //����αƽ�
		boundRect[i] = boundingRect((Mat)contours_poly[i]); //����ÿ����������Ӿ���
		if(boundRect[i].width > 55 && boundRect[i].width <180 && boundRect[i].height > 55 && boundRect[i].height < 180){
			x0 = boundRect[i].x;  //��õ�i����Ӿ��ε����Ͻǵ�x����
			y0 = boundRect[i].y; //��õ�i����Ӿ��ε����Ͻǵ�y����
			w0 = boundRect[i].width; //��õ�i����Ӿ��εĿ��
			h0 = boundRect[i].height; //��õ�i����Ӿ��εĸ߶�
			rectangle(result, Point(x0, y0), Point(x0+w0, y0+h0), Scalar(0, 255, 0), 2, 8); //���Ƶ�i����Ӿ���		
		}
		if((y0 + h0/2 + 1) >= 138 && (y0 + h0/2 - 1) <= 142){
			CarNum++;
		}
		line(result, Point(0,140), Point(568,140), Scalar(0,0,255), 1, 8);
		Point org(0,35);
		
		sprintf(Num, "CarNum=%d", CarNum);
		putText(result, Num, org, CV_FONT_HERSHEY_SIMPLEX, 0.8f, CV_RGB(0,255,0), 2);
	}
	return result;	
}

void main(){	 
	VideoCapture cap("car.avi");
    if(!cap.isOpened()) //�����Ƿ�ɹ�
         return;
    Mat frame;
	Mat result; 
	Mat temp;
	int count=0;
    while(1){
        cap>>frame;
        if(!frame.empty()){
			count++;
			if(count==1)
				 result = MoveDetect(frame, frame);
			else
				 result = MoveDetect(temp, frame);
			imshow("video", frame);
			imshow("result", result);
			temp = frame.clone();
            if(waitKey(5)==27)
               break;
        }
        else
            break;
    }
    cap.release();  
	waitKey(0);
}