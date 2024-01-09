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
    unsigned char* frameBuffer;
    int fb;
    struct fb_var_screeninfo vinfo;
    size_t screenSize;
    char errorStatus;

public:
    // constructors
    Screen(const char* frameBufferPath = "/dev/fb1");
    Screen(const Screen& copiedScreen);

    // destructor
    ~Screen();

    // operator overloads
    Screen& operator=(const Screen& assignedScreen);

    void fitToMe(cv::Mat& image);
    void send(cv::Mat image);
    void operator()(cv::Mat image);

    char getErrorStatus() const;
    int getWidth() const;
    int getHeight() const;

    void setErrorStatus(char newErrorStatus);
};

#endif // SCREEN_H
