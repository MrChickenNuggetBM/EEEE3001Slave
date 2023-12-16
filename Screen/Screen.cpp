#include "Screen.h"
using namespace std;
using namespace cv;

// constructors
Screen::Screen(const char* frameBufferPath) :
    errorStatus(0)
{
    // Get framebuffer information
    fb = open(frameBufferPath, O_RDWR);
    if (fb == -1)
    {
        cerr << "Error opening framebuffer device" << endl;
        setErrorStatus(1);
        return;
    }

    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo))
    {
        cerr << "Error reading variable information" << endl;
        close(fb);
        setErrorStatus(2);
        return;
    }

    // Calculate the size of the framebuffer
    screenSize = vinfo.yres_virtual * vinfo.xres_virtual * vinfo.bits_per_pixel / 8;

    // Map framebuffer to user space
    frameBuffer = (unsigned char*)mmap(nullptr, screenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if (frameBuffer == MAP_FAILED)
    {
        cerr << "Error mapping framebuffer device to memory" << endl;
        close(fb);
        setErrorStatus(3);
        return;
    }

    memset(frameBuffer, 0, screenSize);
}

Screen::Screen(const Screen& copiedScreen)
{
    (*this) = copiedScreen;
}

Screen::~Screen()
{
    // Cleanup and close the framebuffer
    memset(frameBuffer, 0, screenSize);
    munmap(frameBuffer, screenSize);
    close(fb);
}

// operator overloads
Screen& Screen::operator=(const Screen& assignedScreen)
{
    frameBuffer = assignedScreen.frameBuffer;
    fb = assignedScreen.fb;
    vinfo = assignedScreen.vinfo;
    screenSize = assignedScreen.screenSize;
    errorStatus = assignedScreen.errorStatus;
    return (*this);
}

void Screen::fitToMe(Mat& image)
{
    double xScale = (double)vinfo.xres_virtual / (double)image.cols;
    double yScale = (double)vinfo.yres_virtual / (double)image.rows;

    resize(image, image, cv::Size(), xScale, yScale);

    return;
}

void Screen::send(Mat image)
{
    const int screenWidth = vinfo.xres_virtual;
    const int screenHeight = vinfo.yres_virtual;

    // Iterate through framebuffer dimensions
    for (int imageY = 0; imageY < min(screenHeight, image.rows); ++imageY)
    {
        for (int imageX = 0; imageX < min(screenWidth, image.cols); ++imageX)
        {
            // Assuming your image is in BGR format
            uchar* pixel = &frameBuffer[imageY * screenWidth * 4 + imageX * 4];
            uchar intensity = image.at<uchar>(imageY, imageX);
            pixel[0] = pixel[1] = pixel[2] = intensity;
            pixel[3] = 0; // Alpha (transparency)
        }
    }
}

void Screen::operator()(Mat image) { send(image); }

char Screen::getErrorStatus() const { return errorStatus; }
int Screen::getWidth() const { return vinfo.xres_virtual; }
int Screen::getHeight() const { return vinfo.yres_virtual; }

void Screen::setErrorStatus(char newErrorStatus) { errorStatus = newErrorStatus; }
