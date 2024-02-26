#include "../include/Screen.h"
using namespace std;
using namespace cv;

// constructors
Screen::Screen(const char* path) : fbPath(path) {
    getResolution();
}
Screen::Screen(const Screen& copiedScreen) : fbPath(copiedScreen.fbPath){
    getResolution();
}

// retrieves the resolutions of the screen
void Screen::getResolution()
{
    int fbfd = open(fbPath, O_RDWR);
    if (fbfd == -1)
        throw std::runtime_error("Error: Unable to open framebuffer device.");

    struct fb_var_screeninfo vinfo;
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        close(fbfd);
        throw std::runtime_error("Error: Unable to retrieve framebuffer information.");
    }

    width = vinfo.xres;
    height = vinfo.yres;

    close(fbfd);
}

// destructor
Screen::~Screen()
{
    delete[] fbPath;
}

// operator overloads
Screen& Screen::operator=(const Screen& assignedScreen)
{
    fbPath = assignedScreen.fbPath;
    return (*this);
}

// displays an image on the specified frame buffer
void Screen::send(Mat &image) const
{
    ofstream frameBuffer(fbPath, ios::binary);

    if (!frameBuffer.is_open())
        throw std::runtime_error("Error: Unable to open framebuffer device.");

    frameBuffer.write(reinterpret_cast<char *>(image.data), static_cast<streamsize>(image.total() * image.elemSize()));
    frameBuffer.close();
}
