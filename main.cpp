#include "main.h"

VideoCapture *videoCapture;

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
    // videoCapture->read(cameraImage);

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

    std::ofstream frameBuffer("/dev/fb0", std::ios::binary);

    if (!frameBuffer.is_open())
    {
        std::cerr << "Error: Unable to open framebuffer device." << std::endl;
        return false;
    }

    frameBuffer.write(reinterpret_cast<char *>(frame.data), static_cast<std::streamsize>(frame.total() * frame.elemSize()));

    frameBuffer.close();

    // waitKey(0);

    if (false)
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
