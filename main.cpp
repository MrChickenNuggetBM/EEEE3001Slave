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

    Mat frame(1080, 1920, CV_8UC3, Scalar(255, 255, 255));

    Ellipse(Point2f(540,960), Size2f(540,960), 0, Scalar(0,0,0), 1)(frame);

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

