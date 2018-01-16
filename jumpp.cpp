#include "jumpp.h"
float getdistance(cv::Mat &srcImage)
{   //获取图像缩减大小
    Mat reImage;
    pyrDown(srcImage, reImage, Size((&srcImage)->cols / 2, (&srcImage)->rows / 2));
    //转换到HSV
    Mat HSVImage;
    cvtColor(reImage, HSVImage, COLOR_BGR2HSV);
    //二值化
    Mat THImage;
    inRange(HSVImage, Scalar(105, 25, 45), Scalar(135, 125, 130), THImage);
    //开 闭操作
    Mat openImage;
    Mat element1 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat closeImage;
    Mat element2 = getStructuringElement(MORPH_RECT, Size(6, 12));
    morphologyEx(THImage, openImage, MORPH_OPEN, element1);
    morphologyEx(openImage, closeImage,MORPH_CLOSE, element2);
    //imshow("self", closeImage);
    // 寻找最大连通域
    vector<vector<cv::Point>> contours;
    findContours(closeImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    double maxArea = 0;
    vector<cv::Point> maxContour;
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            maxContour = contours[i];
        }
    }
    // 将轮廓转为矩形框
    cv::Rect maxRect = cv::boundingRect(maxContour);
    // 显示连通域
    cv::Mat result;
    closeImage.copyTo(result);
    cv::rectangle(result, maxRect, cv::Scalar(255));
    //小人脚下的点
    Point  centerpoint;
    centerpoint.x = maxRect.x+maxRect.width/2;
    centerpoint.y = maxRect.y+maxRect.height-10;
    circle(reImage, centerpoint ,2, Scalar(0, 255, 0),3);

    //下面处理目标

    int region_left, region_right;
    if (centerpoint.x < (reImage.cols / 2.0))
    {
        region_left = centerpoint.x + 20;
        region_right = reImage.cols;
    }
    else
    {
        region_left = 0;
        region_right = centerpoint.x - 20;
    }
    Mat region;

    HSVImage(Rect(region_left, reImage.rows*0.3, region_right- region_left, reImage.rows*0.29)).copyTo(region);
    cv::rectangle(reImage, Rect(region_left, reImage.rows*0.3, region_right - region_left, reImage.rows*0.29), cv::Scalar(255),2);

    //处理目标
    vector<cv::Mat> ImSplit;
    split(region, ImSplit);
    Mat hsv_s_list[3];
    Mat region_gray, region_sobel;
    Mat Temp;
    for (int i = 0; i < 3; i++)
    {

       GaussianBlur(ImSplit[i], region_gray, Size(5, 5), 0);
       Sobel(region_gray, region_sobel, CV_32F, 0, 1);
       convertScaleAbs(region_sobel, region_sobel);
       threshold(region_sobel, region_sobel, 10, 255, THRESH_BINARY);

        Mat openImage1;
        Mat element3 = getStructuringElement(MORPH_RECT, Size(3, 3));
        Mat closeImage1;
        Mat element4 = getStructuringElement(MORPH_RECT, Size(6, 12));
        morphologyEx(region_sobel, openImage1, MORPH_OPEN, element3);
        morphologyEx(openImage1, closeImage1, MORPH_CLOSE, element4);
        hsv_s_list[i] = closeImage1;
     }
     Mat region_sobel_final;
     bitwise_or(hsv_s_list[0], hsv_s_list[1], region_sobel_final);
     bitwise_or(hsv_s_list[2], region_sobel_final, region_sobel_final);

        // 寻找最大连通域
    vector<vector<cv::Point>>contours1;
    region_sobel_final.convertTo(Temp, CV_8U);
    findContours(Temp, contours1, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    cv::Rect maxArea1(1000,1000,1000,1000);
        vector<cv::Point> maxContour1;
        for (size_t i = 0; i < contours1.size(); i++)
        {
            cv::Rect maxRect1 = cv::boundingRect(contours1[i]);

            if (maxRect1.y < maxArea1.y)
            {
                maxArea1 = maxRect1;
                maxContour1 = contours1[i];
            }
        }

    cv::Rect maxRect1 = cv::boundingRect(maxContour1);
    // 显示连通域
     cv::Mat result1;
    region_sobel_final.copyTo(result1);
    cv::rectangle(result1, maxRect1, cv::Scalar(255),2);
    cv::rectangle(reImage, Rect(region_left+ maxRect1.x, reImage.rows*0.3+ maxRect1.y, maxRect1.width, maxRect1.height), cv::Scalar(255),2);

    cv::Mat result2;
    region_sobel_final(maxRect1).copyTo(result2);
    int left=0, right=0;
    int colNumber = result2.cols*result1.channels();  //列数 x 通道数=每一行元素的个数
    uchar* data = result2.ptr<uchar>(0);  //获取第0行的首地址
    for (int j = 0; j < colNumber-1; j++)   //列循环
    {
        if (data[j] <= 0 && data[j + 1] > 0)
            left = j;
        if (data[j] > 0 && data[j + 1] <= 0)
            right = j;
    }
    circle(reImage, Point(left  + region_left + maxRect1.x, reImage.rows*0.3 + maxRect1.y), 2, Scalar(0, 100, 0), 2);
    circle(reImage, Point(right + region_left + maxRect1.x, reImage.rows*0.3 + maxRect1.y), 2, Scalar(0, 100, 0), 2);
    Point  goalpoint;
    goalpoint.x = (left + right) / 2 + region_left + maxRect1.x;
    goalpoint.y = reImage.rows*0.3 + maxRect1.y;
    if (centerpoint.x < (reImage.cols / 2.0))
    {
        goalpoint.y = centerpoint.y-(int)((goalpoint.x - centerpoint.x)/1.73);
    }
    else
    {
        goalpoint.y = centerpoint.y-(int)((centerpoint.x- goalpoint.x)/1.73);
    }
    circle(reImage, goalpoint, 2, Scalar(0, 255, 0), 3);

    srcImage=reImage;
    double distance =sqrt((double)(goalpoint.x-centerpoint.x)*(goalpoint.x-centerpoint.x)+(goalpoint.y-centerpoint.y)*(goalpoint.y-centerpoint.y));
    return (float)distance;
}
