#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

class Screen
{
private:
    char* frameBufferPath;

public:
    // constructors
    Screen(const cv::Point2f& center, cv::Size2f newSize, float angle, const cv::Scalar& newColour = cv::Scalar(255, 255, 255, 1), int newThickness = 1);
    Screen(const Screen& newScreen);

    // destructor
    ~Screen() = default;

    // operator overloads
    Screen& operator=(const Screen& assignedScreen);

    void send(Mat image);
    void operator()();
};

#endif // SCREEN_H
