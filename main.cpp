#include "main.h"

bool setup()
{
    videoCapture = new VideoCapture(0);
    if (!videoCapture->isOpened())
    {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    videoCapture->set(CAP_PROP_FRAME_WIDTH, 1920);
    videoCapture->set(CAP_PROP_FRAME_HEIGHT, 1080);

    atexit(teardown);
    signal(SIGINT, teardown);

    return true;
}

bool loop()
{
    // Mat cameraImage;
    //videoCapture->read(cameraImage);

    system("clear");

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

    

    // waitKey(0);

    if (sendToScreen(frame))
        return false;

    return true;
}

void teardown()
{
    cout << endl
         << "Stopped after " << i << " frames" << endl;

    // delete screen;

    videoCapture->release();
    delete videoCapture;

    destroyAllWindows();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}

bool sendToScreen(Mat image)
{
    ofstream frameBuffer("/dev/fb1", ios::binary);

    if (!frameBuffer.is_open())
    {
        cerr << "Error: Unable to open framebuffer device." << endl;
        return false;
    }

    frameBuffer.write(reinterpret_cast<char *>(image.data), static_cast<streamsize>(image.total() * image.elemSize()));

    frameBuffer.close();

    return true;
}
