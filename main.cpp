#include "main.h"

Screen *screen;
VideoCapture *videoCapture;

bool isQPressed() {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) == 1;
}

bool setup() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    screen = new Screen("/dev/fb1");
    if (screen->getErrorStatus()) {
        return false;
    }

    videoCapture = new VideoCapture(0);
    if (!videoCapture->isOpened()) {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    return true;
}

bool loop(ullint i) {
    Mat cameraImage;
    videoCapture->read(cameraImage);

    Mat frame = Mat::ones(480, 640, CV_8UC3) * 255;

    Ellipse(Point2f(50,((50 + i) % 100)), Size2f(25,50), 0, Scalar(0,0,0), 1)(frame);

    screen->send(frame, false);

    cout << i << endl;

    if (isQPressed()) {
        return false;
    }
    return true;
}

bool teardown() {
    delete screen;

    videoCapture->release();
    delete videoCapture;

    destroyAllWindows();

    return true;
}
