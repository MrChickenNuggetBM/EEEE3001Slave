#ifndef SCREEN_HPP
#define SCREEN_HPP

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
    const char* fbPath;

public:
    // constructors
    Screen(const char* path = "/dev/fb0");
    Screen(const Screen& copiedScreen);

    // destructor
    ~Screen();

    // operator overloads
    Screen& operator=(const Screen& assignedScreen);

    bool send(cv::Mat& image) const;
};

#endif // SCREEN_HPP
