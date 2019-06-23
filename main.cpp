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
    Mat img_input;
    
    //비디오 캡쳐 초기화
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        cerr << "에러 - 카메라를 열 수 없습니다.\n";
        return -1;
    }
    
    
    while (1)
    {
        // 카메라로부터 캡쳐한 영상을 frame에 저장합니다.
        cap.read(img_input);
        if (img_input.empty()) {
            cerr << "빈 영상이 캡쳐되었습니다.\n";
            break;
        }
        
        Mat img_grayscale;
        cvtColor(img_input, img_grayscale, COLOR_BGR2GRAY);
        
        Mat img_canny;
        Canny(img_grayscale, img_canny, 50, 150);
        
        Mat img_result;
        cvtColor(img_canny, img_canny, COLOR_GRAY2BGR);
        
        hconcat(img_input, img_canny, img_result);
        
        namedWindow("result", WINDOW_AUTOSIZE);
        imshow("result", img_result);
        
        
        if (waitKey(25) >= 0)
            break;
    }
    
    
    return 0;
}
