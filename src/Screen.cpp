#include "../include/Screen.h"
using namespace std;
using namespace cv;

// constructors
Screen::Screen(const char* path) : fbPath(path) {}
Screen::Screen(const Screen& copiedScreen) : fbPath(copiedScreen.fbPath) {}

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
bool Screen::send(Mat &image) const
{
    ofstream frameBuffer(fbPath, ios::binary);

    if (!frameBuffer.is_open())
    {
        cerr << "Error: Unable to open framebuffer device." << endl;
        return false;
    }

    frameBuffer.write(reinterpret_cast<char *>(image.data), static_cast<streamsize>(image.total() * image.elemSize()));
    frameBuffer.close();

    return true;
}
