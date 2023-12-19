#include "main.h"

VideoCapture videoCapture(0);
ofstream frameBuffer("/dev/fb1", ios::binary);

bool setup()
{
    system("setterm -cursor off;clear");

    if (!videoCapture.isOpened())
    {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    if (!frameBuffer.is_open())
    {
        cerr << "Error: Unable to open framebuffer device." << endl;
        return false;
    }

    videoCapture.set(CAP_PROP_FRAME_WIDTH, 1920);
    videoCapture.set(CAP_PROP_FRAME_HEIGHT, 1080);

    atexit(teardown);
    signal(SIGINT, teardown);

    return true;
}

bool loop()
{
    // Mat cameraImage;
    // videoCapture.read(cameraImage);

    Mat frame(
        1080,
        1920,
        CV_8UC4,
        Scalar(0, 0, 0, 0));

    Ellipse ellipse(
        Point2f(960, 540),
        Size2f(
            1920 - 10. * float(i % 100),
            1080 - 10. * float(i % 100)),
        0,
        Scalar(255, 255, 255),
        3);
    ellipse(frame);

    if (!frameBuffer.is_open())
    {
        cerr << "Error: Unable to open framebuffer device." << endl;
        return false;
    }

    frameBuffer.write(reinterpret_cast<char *>(frame.data), static_cast<streamsize>(frame.total() * frame.elemSize()));

    // waitKey(0);

    if (false)
        return false;

    return true;
}

void teardown()
{
    system("setterm -cursor on;clear");

    cout << endl
         << "Stopped after " << i << " frames" << endl;

    // delete screen;

    videoCapture.release();

    frameBuffer.close();

    destroyAllWindows();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}
