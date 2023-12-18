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
    Mat cameraImage;
    videoCapture->read(cameraImage);

    Mat frame(
        1080,
        1920,
        CV_8UC4,
        Scalar(0, 0, 0, 0));

    Ellipse ellipse(
        Point2f(960, 540),
        Size2f(
            1920 / (1 + float(i % 10)),
            1080 / (1 + float(i % 10))),
        0,
        Scalar(255, 255, 255),
        3);
    ellipse(frame);

    std::ofstream fb("/dev/fb1", std::ios::binary);

    if (!fb.is_open())
    {
        std::cerr << "Error: Unable to open framebuffer device." << std::endl;
        return false;
    }

    fb.write(reinterpret_cast<char *>(frame.data), static_cast<std::streamsize>(frame.total() * frame.elemSize()));

    fb.close();

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
