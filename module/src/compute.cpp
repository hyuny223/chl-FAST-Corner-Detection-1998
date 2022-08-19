#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"
#include "compute.h"



void computeSimpleCornerness(const cv::Mat& img, std::vector<std::vector<int>>& potential,
                             const float& threshold)
{
    int Ic, Ia, Ia_, Ib, Ib_, simpleCorner, Ra, Rb;
    int B1, B2, C, A, B;
    float interPixelCorner;

    for(int y = 1; y < img.rows-1; ++y)
    {
        for(int x = 1; x < img.cols-1; ++x)
        {
            Ic = static_cast<int>(img.ptr<uchar>(y)[x]);

            Ia = static_cast<int>(img.ptr<uchar>(y+1)[x]);
            Ia_ = static_cast<int>(img.ptr<uchar>(y-1)[x]);

            Ib = static_cast<int>(img.ptr<uchar>(y)[x+1]);
            Ib_ = static_cast<int>(img.ptr<uchar>(y)[x-1]);

            Ra = computeR(Ia, Ia_, Ic);
            Rb = computeR(Ib, Ib_, Ic);

            simpleCorner = Ra < Rb ? Ra : Rb;

            if(simpleCorner > threshold)
            {
                potential[y][x] = 255;
            }

        }
    }
}

void computeFullCornerness(const cv::Mat& blur, const cv::Mat& img,
                           std::vector<std::vector<int>>& corners,
                           const float& threshold,
                           const int& scaleX, const int& scaleY,
                           const std::vector<std::vector<int>>& simplePotential)
{
    int Ic, Ia, Ia_, Ib, Ib_, simpleCorner, Ra, Rb;
    int B1, B2, C, A, B;
    float interPixelCorner;


    for(int y = 1; y < blur.rows-1; ++y)
    {
        for(int x = 1; x < blur.cols-1; ++x)
        {
            if(simplePotential[y][x] == 0)
            {

                continue;
            }

            int blur_y = y * scaleY;
            int blur_x = x * scaleX;

            Ic = static_cast<int>(img.ptr<uchar>(blur_y)[blur_x]);

            Ia = static_cast<int>(img.ptr<uchar>(blur_y+1)[blur_x]);
            Ia_ = static_cast<int>(img.ptr<uchar>(blur_y-1)[blur_x]);

            Ib = static_cast<int>(img.ptr<uchar>(blur_y)[blur_x+1]);
            Ib_ = static_cast<int>(img.ptr<uchar>(blur_y)[blur_x-1]);

            Ra = computeR(Ia, Ia_, Ic);
            Rb = computeR(Ib, Ib_, Ic);

            simpleCorner = Ra < Rb ? Ra : Rb;

            if(simpleCorner > threshold)
            {
                B1 = computeB1(Ia, Ia_, Ib, Ib_, Ic);
                B2 = computeB2(Ia, Ia_, Ib, Ib_, Ic);
                C = Ra;
                B = computeB(B1, B2);
                A = computeA(Ra, Rb, B);
                interPixelCorner = computeInterPixel(Ra, Rb, A, B, C);
                if(interPixelCorner > threshold)
                {
                    corners[blur_y][blur_x] = interPixelCorner;
                }
            }
        }
    }
}

void computeGaussian(const cv::Mat& src, cv::Mat& dst, const int& kernelSize)
{
    std::vector<std::vector<float>> kernel;
    float n{0};

    if(kernelSize == 3)
    {
        kernel = {{1,2,1},
                  {2,4,2},
                  {1,2,1}};
        n = 16;
    }
    else if(kernelSize == 5)
    {
        kernel = {{1,4,7,4,1},
                  {4,16,26,16,4},
                  {7,26,41,26,7},
                  {4,16,26,16,4},
                  {1,4,7,4,1}};
        n = 273;
    }
    else if(kernelSize == 7)
    {
        kernel = {{0,0,1,2,1,0,0},
                  {0,3,13,22,13,3,0},
                  {1,13,59,97,59,13,1},
                  {2,22,97,159,97,22,2},
                  {1,13,59,97,59,13,1},
                  {0,3,13,22,13,3,0},
                  {0,0,1,2,1,0,0}};
        n = 1003;
    }

    for(int y=0; y<kernelSize; ++y)
    {
        for(int x=0; x<kernelSize; ++x)
        {
            kernel[y][x] /= n;
        }
    }

    auto step = static_cast<int>(kernelSize/2);

    for(int y = kernelSize; y < src.rows - kernelSize; ++y)
    {
        for(int x = kernelSize; x < src.cols - kernelSize; ++x)
        {
            float val{0};
            for(int ky = 0; ky < kernelSize; ++ky)
            {
                for(int kx = 0; kx < kernelSize; ++kx)
                {
                    val += static_cast<float>(src.ptr<uchar>(y-step+ky)[x-step+kx])*static_cast<float>(kernel[ky][kx]);
                }
            }
            dst.ptr<uchar>(y)[x] = static_cast<uchar>(val);
        }
    }
}
