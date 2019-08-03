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


    cam.open(0);
    if (!cam.isOpened()) {
        cerr << "ERROR - Failed to Open Cam\n";
        return -1;
    }
    
    
    while (1)
    {
        cam >> img;

        cvtColor(img, HSV, COLOR_BGR2HSV);
        inRange(HSV, Scalar(0,0,0), Scalar(255,255,255), mask);
        cvtColor(mask, mask, COLOR_GRAY2BGR);
        hconcat(img, mask, result);

        imshow("result", result);
        
        
        if (waitKey(5) >= 0)
            break;
    }
    
    
    return 0;
}
