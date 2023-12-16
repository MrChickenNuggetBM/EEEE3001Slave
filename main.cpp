#include "main.h"

bool setup() {
    screen = new Screen("/dev/fb1");
    if (screen->getErrorStatus()) {
        return false;
    }

    videoCapture = new VideoCapture(0);
    if (!videoCapture->isOpened()) {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    atexit(teardown);
    signal(SIGINT, teardown);

    return true;
}

bool loop() {
    // Mat cameraImage;
    // videoCapture->read(cameraImage);

    Mat frame(540, 960, CV_8UC1, Scalar(255, 255, 255));

    int lool1 = 960 / ((i % 5) + 1);
    int lool2 = 540 / ((i % 5) + 1);

    Ellipse(Point2f(480,270), Size2f(lool1,lool2), 0, Scalar(0,0,0), 1)(frame);

    screen->send(frame);
    // screen->send(cameraImage);

    if (false) return false;

    return true;
}

void teardown() {
    cout << endl << "Stopped after " << i << " frames" << endl;

    delete screen;

    videoCapture->release();
    delete videoCapture;

    destroyAllWindows();
}

void teardown(int signal) {
    exit(EXIT_SUCCESS);
}
