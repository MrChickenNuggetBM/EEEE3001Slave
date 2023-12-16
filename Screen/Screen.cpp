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

void Screen::send(Mat image, bool isFullScreen)
{
    // Iterate through framebuffer dimensions
    for (int y = 0; y < vinfo.yres_virtual; ++y)
    {
        for (int x = 0; x < vinfo.xres_virtual; ++x)
        {
            // Calculate corresponding pixel position in the original image
            int img_x = (x * (isFullScreen ? image.cols : vinfo.xres_virtual)) / vinfo.xres_virtual;
            int img_y = (y * (isFullScreen ? image.rows : vinfo.yres_virtual)) / vinfo.yres_virtual;

            // Check if the coordinates are within image bounds
            if (img_x >= 0 && img_x < image.cols && img_y >= 0 && img_y < image.rows)
            {
                // Assuming your image is in BGR format
                unsigned char* pixel = &frameBuffer[y * vinfo.xres_virtual * 4 + x * 4];
                pixel[0] = image.at<Vec3b>(img_y, img_x)[0]; // Blue
                pixel[1] = image.at<Vec3b>(img_y, img_x)[1]; // Green
                pixel[2] = image.at<Vec3b>(img_y, img_x)[2]; // Red
                pixel[3] = 0; // Alpha (transparency)
            }
        }
    }
}

void Screen::operator()(Mat image, bool isFullScreen) { send(image, isFullScreen); }

char Screen::getErrorStatus() const { return errorStatus; }
int Screen::getWidth() const { return vinfo.xres_virtual; }
int Screen::getHeight() const { return vinfo.yres_virtual; }

void Screen::setErrorStatus(char newErrorStatus) { errorStatus = newErrorStatus; }
