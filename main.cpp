#include "main.h"

Screen *screen;
VideoCapture *videoCapture;

bool setup()
{
    screen = new Screen("/dev/fb1");
    if (screen->getErrorStatus())
    {
        return false;
    }

    videoCapture = new VideoCapture(0);
    if (!videoCapture->isOpened())
    {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    return true;
}

bool loop(ullint i)
{
    Mat cameraImage;
    videoCapture->read(cameraImage);

    (*screen)(cameraImage);

    // Mat blank(screen->getHeight(), screen->getWidth(), CV_8UC1, Scalar(0));

    if (false)
        return false;

    cout << i << endl;
    return true;
}

bool teardown()
{
    delete screen;

    videoCapture->release();
    delete videoCapture;

    destroyAllWindows();

    return true;
}
