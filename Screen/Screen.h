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
    int frameBuffer;
    struct fb_var_screeninfo vinfo;
    size_t screenSize;
    char errorStatus;

public:
    // constructors
    Screen::Screen(const char* frameBufferPath = "/dev/fb1");
    Screen(const Screen& copiedScreen);

    // destructor
    ~Screen() = default;

    // operator overloads
    Screen& operator=(const Screen& assignedScreen);

    void send(cv::Mat image);
    void operator()();

    char getErrorStatus();
    void setErrorStatus(char newErrorStatus);
};

#endif // SCREEN_H
