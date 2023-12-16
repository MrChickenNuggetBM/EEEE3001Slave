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
}

Screen::Screen(const Screen& copiedScreen)
{
    frameBuffer = copiedScreen.frameBuffer;
}

// operator overloads
Screen& Screen::operator=(const Screen& assignedScreen)
{
    frameBuffer = assignedScreen.frameBuffer;
    return (*this);char getErrorStatus();
    void setErrorStatus(char newErrorStatus);
}

void Screen::send(Mat image)
{
    // Copy OpenCV image to the framebuffer
    for (int y = 0; y < image.rows; ++y)
    {
        for (int x = 0; x < image.cols; ++x)
        {
            // Assuming your image is in BGR oid Screen::close()format
            unsigned char* pixel = &frameBuffer[(vinfo.yoffset + y) * vinfo.xres_virtual * 4 + (vinfo.xoffset + x) * 4];
            pixel[0] = image.at<Vec3b>(y, x)[0]; // Blue
            pixel[1] = image.at<Vec3b>(y, x)[1]; // Green
            pixel[2] = image.at<Vec3b>(y, x)[2]; // Red
            pixel[3] = 0; // Alpha (transparency)
        }
    }

    return;
}
void Screen::operator()(Mat image) { send(image); }

void Screen::endSession()
{
    // Cleanup and close the framebuffer
    munmap(frameBuffer, screenSize);
    close(fb);
}

char Screen::getErrorStatus() const { return errorStatus; }
int Screen::getWidth() const { return vinfo.xres_virtual; }
int Screen::getHeight() const { return vinfo.yres_virtual; }

void Screen::setErrorStatus(char newErrorStatus) { errorStatus = newErrorStatus; }
