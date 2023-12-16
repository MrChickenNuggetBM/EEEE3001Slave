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
    Mat cameraImage;
    videoCapture->read(cameraImage);

    Mat frame(480, 640, CV_8UC3, Scalar(255, 255, 255));

    Ellipse(Point2f(50,((50 + i) % 100)), Size2f(25,50), 0, Scalar(0,0,0), 1)(frame);

    screen->send(frame, false);
    //screen->send(cameraImage, false);


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


