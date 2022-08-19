#pragma once
#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"

template <typename T>
T computeB1(const T& P, const T& P_,
             const T& Q, const T& Q_,
             const T& C)
{
    return (Q-P)*(P-C) + (Q_-P_)*(P_-C);
}

template <typename T>
T computeB2(const T& P, const T& P_,
             const T& Q, const T& Q_,
             const T& C)
{
    return (Q-P_)*(P_-C) + (Q_-P)*(P-C);
}

template <typename T>
T computeR(const T& P, const T& P_, const T& C)
{
    return (P-C)*(P-C) + (P_-C)*(P_-C);
}

template <typename T>
T computeB(const T& B1, const T& B2)
{
    return (B1 <= B2) ? B1:B2;
}

template <typename T>
T computeA(const T& Ra, const T& Rb, const T& B)
{
    return Rb - Ra - 2*B;
}

template <typename T>
auto computeInterPixel(const T& Ra, const T& Rb,
                      const T& A, const T& B, const T& C)
{
    return (B<0 && A+B>0) ? C-(B*B)/A : ((Ra<Rb) ? Ra:Rb);
}

void computeSimpleCornerness(const cv::Mat& img, std::vector<std::vector<int>>& potential,
                       const float& threshold);

void computeFullCornerness(const cv::Mat& resized, const cv::Mat& img,
                           std::vector<std::vector<int>>& corners,
                           const float& threshold,
                           const int& scaleX, const int& scaleY,
                           const std::vector<std::vector<int>>& simplePotential);

void computeGaussian(const cv::Mat& src, cv::Mat& dst, const int& kernelSize);
