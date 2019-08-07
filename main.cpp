//
//  main.cpp
//  Color_Tracking
//
//  Created by GyeongTaek Kim on 23/06/2019.
//  Copyright © 2019 GyeongTaek Kim. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cam;
    Mat img;
    Mat mask;
    Mat HSV;
    Mat result;

    const string configName = "CONFIG";
    int HMin, SMin, VMin, HMax = 256, SMax = 256, VMax = 256;
    int Osize, Csize;

    cam.open(0);
    if (!cam.isOpened())
    {
        cerr << "ERROR - Failed to Open Cam\n";
        return -1;
    }

    namedWindow(configName, WINDOW_AUTOSIZE);
    createTrackbar("H", configName, &HMin, 256);
    createTrackbar("S", configName, &SMin, 256);
    createTrackbar("V", configName, &VMin, 256);
    createTrackbar("HMax", configName, &HMax, 256);
    createTrackbar("SMax", configName, &SMax, 256);
    createTrackbar("VMax", configName, &VMax, 256);
    createTrackbar("OSize", configName, &Osize, 20);
    createTrackbar("CSize", configName, &Csize, 20);

    while (1)
    {
        cam >> img;

        cvtColor(img, HSV, COLOR_BGR2HSV);
        inRange(HSV, Scalar(HMin, SMin, VMin), Scalar(HMax, SMax, VMax), mask);

        // https://docs.opencv.org/4.0.1/d3/dbe/tutorial_opening_closing_hats.html
        morphologyEx(mask, mask, MORPH_OPEN, getStructuringElement(0, Size(2*Osize+1, 2*Osize+1), Point(Osize, Osize)));
        morphologyEx(mask, mask, MORPH_CLOSE,
                getStructuringElement(0, Size(2*Csize+1, 2*Csize+1), Point(Csize, Csize)));

        {
            // https://www.youtube.com/watch?v=T5Vv2mgj2Os
            Mat label, stat, centroid;
            int labels = connectedComponentsWithStats(mask, label, stat, centroid);

            int maxsize = -1, current = 0; // To Find Biggest Part!
            for (int i = 1; i<labels; i++)
            {
                int area = stat.at<int>(i, CC_STAT_AREA);
                if (maxsize<area)
                {
                    maxsize = area;
                    current = i;
                }
            }

            int height = stat.at<int>(current, CC_STAT_HEIGHT);
            int left = stat.at<int>(current, CC_STAT_LEFT);
            int top = stat.at<int>(current, CC_STAT_TOP);
            int width = stat.at<int>(current, CC_STAT_WIDTH);
            Point center = Point(centroid.at<double>(current, 0), centroid.at<double>(current, 1));

            rectangle(img, Point(left, top), Point(left+width, top+height), Scalar(0, 0, 255), 3);
            circle(img, center, 4, Scalar(0,255,0), 4);
            putText(img, to_string(center.x)+", "+to_string(center.y),Point(center.x, center.y+40), 1, 3, Scalar(0,255,0), 3);

        }

        cvtColor(mask, mask, COLOR_GRAY2BGR);
        hconcat(img, mask, result);

        imshow("result", result);

        if (waitKey(5)>=0)
            break;
    }

    return 0;
}
