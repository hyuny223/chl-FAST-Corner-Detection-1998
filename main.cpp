#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>
#include <fstream>

#include "opencv2/opencv.hpp"
#include "compute.h"


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "put the path of images. Only one argument available!" << std::endl;
        return -1;
    }

    std::experimental::filesystem::path p(argv[1]);
    cv::Mat origin = cv::imread(p, cv::IMREAD_COLOR);
    cv::resize(origin,origin,cv::Size(1600,1200));

    cv::Mat img;
    cv::cvtColor(origin,img, cv::COLOR_BGR2GRAY);

    cv::Mat resized;
    cv::resize(img,resized,cv::Size(400,300));
    cv::Mat blur;
    resized.copyTo(blur);
    computeGaussian(resized,blur,3);

    auto scaleY = img.rows / resized.rows;
    auto scaleX = img.cols / resized.cols;

    std::vector<std::vector<int>> potential;
    potential.assign(300,std::vector<int>(400,0));

    computeSimpleCornerness(blur,potential,300);

    std::vector<std::vector<int>> corners;
    corners.assign(1200,std::vector<int>(1600,0));

    computeFullCornerness(blur, img, corners, 300, scaleX, scaleY, potential);


    for(int y = 0; y < 1200; ++y)
    {
        for(int x = 0; x < 1600; ++x)
        {
            if(corners[y][x] != 0)
            {
                cv::circle(origin,cv::Point(x,y),6,cv::Scalar(0,255,0));
            }
        }
    }

    cv::imshow("img",origin);
    cv::waitKey(0);

    // std::ifstream myFile;
    // myFile.open(argv[1]);

    // if(myFile.is_open())
    // {
    //     myFile
    // }
}
