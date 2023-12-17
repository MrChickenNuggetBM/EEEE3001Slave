#include "main.h"

bool setup() {
//    screen = new Screen("/dev/fb1");
//    if (screen->getErrorStatus()) {
//        return false;
//    }

    videoCapture = new VideoCapture(0);
    if (!videoCapture->isOpened()) {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }
    videoCapture->set(CAP_PROP_FRAME_WIDTH, 960);
    videoCapture->set(CAP_PROP_FRAME_HEIGHT, 540);

    atexit(teardown);
    signal(SIGINT, teardown);

    // Get the dimensions of the framebuffer
    int screenWidth = 0;  // Initialize to 0
    int screenHeight = 0; // Initialize to 0

    std::ifstream fb_dimensions("/sys/class/graphics/fb1/virtual_size");
    fb_dimensions >> screenWidth >> screenHeight;
    fb_dimensions.close();

    return true;
}

bool loop() {
    Mat cameraImage;
    videoCapture->read(cameraImage);
    cvtColor(cameraImage, cameraImage, COLOR_BGR2GRAY);

    Mat frame(540, 960, CV_8UC1, Scalar(255, 255, 255));

    //imshow("img", cameraImage);

    int lool1 = 900 / ((i % 5) + 1);
    int lool2 = 480 / ((i % 5) + 1);

    Ellipse(Point2f(480,270), Size2f(lool1,lool2), 0, Scalar(0,0,0), 3)(frame);

    // screen->send(frame);
    // screen->fitToMe(cameraImage);
    // cout << cameraImage.cols << " * " << cameraImage.rows << endl;
    // screen->send(cameraImage);

    std::ofstream fb("/dev/fb1", std::ios::binary);

    if (!fb.is_open()) {
        std::cerr << "Error: Unable to open framebuffer device." << std::endl;
        return false;
    }

    fb.write(reinterpret_cast<char*>(cameraImage.data), cameraImage.total() * cameraImage.elemSize());

    fb.close();

    //waitKey(0);

    if (false) return false;

    return true;
}

void teardown() {
    cout << endl << "Stopped after " << i << " frames" << endl;

    // delete screen;

    videoCapture->release();
    delete videoCapture;

    destroyAllWindows();
}

void teardown(int signal) {
    exit(EXIT_SUCCESS);
}
