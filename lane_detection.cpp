#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<vector>
#include<cstdlib>
#include<stdio.h>
#include<iostream>
#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

using namespace std;
using namespace cv;


int main()
{
	    Mat img;
	    Mat edge_detecttion;
	    Mat greyscale_img;
	    Mat greyscale__blur_img;
	    Mat dst;
	    img = imread("test1.png");
		imshow("input_image", img);
		cvtColor(img, greyscale_img, COLOR_BGR2GRAY);
		blur(greyscale_img, greyscale__blur_img, Size(3, 3));

		Canny(greyscale__blur_img, greyscale__blur_img, 100, 300, 3);
		dst = Scalar::all(0);
		img.copyTo(dst, greyscale__blur_img);
	   	Mat inputImage = dst;
		if (inputImage.channels() > 1)
		{
			cv::cvtColor(inputImage, inputImage, CV_RGB2GRAY);
		}
		//co-ordinates of the region of interest
		int x0 = img.cols - 1;
		int y0 = img.rows - 1;
		int x1 = img.cols / 2-1;
		int y1 = img.rows / 2-1;
		int x2 = 0;
		int y2 = img.rows - 1;

		// then create a line masking using these three points
		cv::Mat lineMask = cv::Mat::zeros(inputImage.size(), inputImage.type());
		cv::line(lineMask, cv::Point(x0, y0), cv::Point(x1, y1), cv::Scalar(255, 255, 0), 1, 8, 0);
		cv::line(lineMask, cv::Point(x0, y0), cv::Point(x2, y2), cv::Scalar(255, 255, 0), 1, 8, 0);
		cv::line(lineMask, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255, 255, 0), 1, 8, 0);
		// perform contour detection on your line mask
		vector<vector<Point>> contours;
		vector<cv::Vec4i> hierarchy;
		cv::findContours(lineMask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		// calculate the distance to the contour
		cv::Mat raw_dist(lineMask.size(), CV_32FC1);
		
		for (int i = 0; i < lineMask.rows; i++)
		{
			for (int j = 0; j < lineMask.cols; j++)
			{
				raw_dist.at<float>(i, j) = cv::pointPolygonTest(contours[0], cv::Point2f(j, i), true);
				//cout << raw_dist.at<float>(i, j) << endl;
			}
		}

		double minVal; double maxVal;
		cv::minMaxLoc(raw_dist, &minVal, &maxVal, 0, 0, cv::Mat());
		minVal = std::abs(minVal);
		maxVal = std::abs(maxVal);

		// depicting the distances graphically
		cv::Mat mask = cv::Mat::zeros(inputImage.size(), CV_8UC1);

		for (int i = 0; i < mask.rows; i++)
		{
			for (int j = 0; j < mask.cols; j++)
			{
				if (raw_dist.at<float>(i, j) < 0)//point outside the mask
				{
					mask.at<uchar>(i, j) = static_cast<uchar>(0);
					continue;
				}
				mask.at<uchar>(i, j) = static_cast<uchar>(255);//point inside the mask
			}
		}

		cv::Mat invInput = inputImage;
		cv::Mat outputImage;
		invInput.copyTo(outputImage, mask);
		
		int thickness = 2;
		int lineType = 1;
		line(dst,
			Point(0, img.rows),
			Point(img.cols, img.rows),
			Scalar(0, 0, 255),
			thickness,
			lineType);
		line(dst,
			Point(0, img.rows),
			Point(img.cols / 2, img.rows / 2),
			Scalar(0, 0, 255),
			thickness,
			lineType);
		line(dst,
			Point(img.cols, img.rows),
			Point(img.cols / 2, img.rows / 2),
			Scalar(0, 0, 255),
			thickness,
			lineType);
	   
		
		vector<Vec4i> linesP; // will hold the results of the detection
		HoughLinesP(outputImage, linesP, 1, CV_PI / 180, 20, 20, 30);// runs the actual detection
		
		// Draw the lines
		double slope_thresh = 0.3;
		for (size_t i = 0; i < linesP.size(); i++)
		{
			Vec4i l = linesP[i];
			double slope = (static_cast<double>(l[3]) - static_cast<double>(l[1])) / (static_cast<double>(l[2]) - static_cast<double>(l[0]) + 0.00001);
			if (std::abs(slope) > slope_thresh)
			line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
		}
		imshow("final_output_image", img);
		
		waitKey(0);

	
}
