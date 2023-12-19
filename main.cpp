#include "main.h"

VideoCapture videoCapture(0);

bool display(Mat &image)
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

bool setup()
{
    system("setterm -cursor off;clear");

    if (!videoCapture.isOpened())
    {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    videoCapture.set(CAP_PROP_FRAME_WIDTH, 960);
    videoCapture.set(CAP_PROP_FRAME_HEIGHT, 540);

    atexit(teardown);
    signal(SIGINT, teardown);

    return true;
}

bool loop()
{
    Mat cameraImage(
        1080,
        1920,
        CV_8UC4,
        Scalar(0, 0, 0, 255));
    videoCapture.read(cameraImage);

    Mat frame(
        1080,
        1920,
        CV_8UC4,
        Scalar(0, 0, 0, 255));

    Ellipse ellipse(
        Point2f(960, 540),
        Size2f(
            1920 - 10. * float(i % 100),
            1080 - 10. * float(i % 100)),
        0,
        Scalar(255, 255, 255),
        3);
    ellipse(frame);

    cvtColor(cameraImage, cameraImage, COLOR_BGR2RGBA);
    imshow("hi", cameraImage);
    return (display(frame) && (waitKey(1) < 0));
}

void teardown()
{
    system("setterm -cursor on;clear");

    cout << endl
         << "Stopped after " << i << " frames" << endl;

    videoCapture.release();

    destroyAllWindows();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}
