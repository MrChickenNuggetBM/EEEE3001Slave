#include "main.h"

// topics to subscribe to
string TOPICS[]  = {"parameters/minorRadius"};
// mqtt broker definition
string SERVER_ADDRESS("mqtt://localhost:1883");
async_client cli(SERVER_ADDRESS, "raspberrypi");

// displays an image on the specified frame buffer
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
    connect_options connOpts;
    connOpts.set_clean_session(false);

    // Install the callback(s) before connecting.
    Callback cb(cli, connOpts, TOPICS);
    cli.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.

    try {
        cout << "Connecting to the MQTT server..." << flush;
        cli.connect(connOpts, nullptr, cb);
    }
    catch (const mqtt::exception& exc) {
        cerr << "\nERROR: Unable to connect to MQTT server: '"
             << SERVER_ADDRESS << "'" << exc << endl;
        return 1;
    }

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
    return (/* display(frame) && */(waitKey(1) < 0));
}

void teardown()
{
    system("setterm -cursor on;clear");

    cout << endl
         << "Stopped after " << i << " frames" << endl;

    // Disconnect

    try {
        cout << "\nDisconnecting from the MQTT server..." << flush;
        cli.disconnect()->wait();
        cout << "OK" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc << endl;
        return 1;
    }

    // videoCapture.release();

    // destroyAllWindows();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}
