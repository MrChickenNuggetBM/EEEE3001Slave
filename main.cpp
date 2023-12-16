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

    Mat frame = Mat::ones(480, 640, CV_8UC3) * 255;

    Ellipse ellipse(Point2f(50,((50 + i) % 100)), Size2f(25,50), 0, Scalar(0,0,0), 1);

    ellipse(frame);

    screen->send(frame, false);

    // Mat blank(screen->getHeight(), screen->getWidth(), CV_8UC1, Scalar(0));

    cout << i << endl;

    if (i >= 100)
        return false;
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
