#include "main.h"

// displays an image on fb1
bool display(Mat &image)
{
    ofstream frameBuffer("/dev/fb0", ios::binary);

    if (!frameBuffer.is_open())
    {
        cerr << "Error: Unable to open framebuffer device." << endl;
        return false;
    }

    frameBuffer.write(reinterpret_cast<char *>(image.data), static_cast<streamsize>(image.total() * image.elemSize()));

    frameBuffer.close();

    return true;
}

//class Callback : public virtual mqtt::callback
//{
//public:
//    void message_arrived(mqtt::const_message_ptr msg) override
//    {
//        cout << "Message received: " << msg->to_string() << endl;
//        cout << "Payload received: " << msg->get_payload_str() << endl;
//    }
//};

bool setup()
{
    // clear the terminal
    system("setterm -cursor off;clear");

    // set up the camera
    // if (!videoCapture.isOpened())
    // {
    //     cerr << "Error: Could not open camera" << endl;
    //     return false;
    // }

    // videoCapture.set(CAP_PROP_FRAME_WIDTH, 960);
    // videoCapture.set(CAP_PROP_FRAME_HEIGHT, 540);

    // configure code termination
    atexit(teardown);
    signal(SIGINT, teardown);

    // establish broker-client connection
//    connect_options connOpts;
//    connOpts.set_clean_session(true);

//    Callback callback;
//    client.set_callback(callback);

//    try {
//        client.connect(connOpts)->wait();
//        client.subscribe("hello/hi", 1)->wait();
//    } catch (const mqtt::exception &exc) {
//        cerr << "Error: " << exc.what() << endl;
//        return false;
//    }

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

    // imshow("hi", cameraImage);
    return (display(frame) && (waitKey(1) < 0));
}

void teardown()
{
    system("setterm -cursor on;clear");

    cout << endl
         << "Stopped after " << i << " frames" << endl;

    // videoCapture.release();

    // destroyAllWindows();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}

