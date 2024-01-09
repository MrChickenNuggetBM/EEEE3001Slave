#include "main.h"

const string TOPICS[]  =
{
    "parameters/xCenter",
    "parameters/yCenter",
    "parameters/xRadius",
    "parameters/yRadius",
    "parameters/thickness",
    "parameters/isCircle",
    "parameters/isBrightfield"
};
// mqtt broker definition
const string SERVER_ADDRESS("mqtt://localhost:1883");
async_client CLIENT(SERVER_ADDRESS, "raspberrypi");
// connection options
connect_options options;
// callback
Callback cb(CLIENT, options, TOPICS, 7);

// variable for screen
Screen screen;

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

    options.set_clean_session(false);

    // Install the callback(s) before connecting.

    CLIENT.set_callback(cb);

    // Start the connection.
    // When completed, the callback will subscribe to topic.
    try
    {
        cout << "Connecting to the MQTT server..." << flush;
        CLIENT.connect(options, nullptr, cb);
    }
    catch (const mqtt::exception& exc)
    {
        cerr << "\nERROR: Unable to connect to MQTT server: '"
             << SERVER_ADDRESS << "'" << exc << endl;
        return false;
    }

    return true;
}

bool loop()
{
    using namespace mqtt::topics::parameters;
    // Mat cameraImage;
    // videoCapture.read(cameraImage);

    int ringColour = 255 * (int)isBrightfield;

    Mat frame(
        1080,
        1920,
        CV_8UC4,
        Scalar(
            255 - ringColour,
            255 - ringColour,
            255 - ringColour,
            255
        )
    );

    Ellipse ellipse(
        Point2f(
            960 + xCenter,
            540 + yCenter
        ),
        Size2f(
            2 * (isCircle ? std::min(xRadius, yRadius) : xRadius),
            2 * (isCircle ? std::min(xRadius, yRadius) : yRadius)
        ),
        0,
        Scalar(
            ringColour,
            ringColour,
            ringColour
        ),
        thickness
    );
    ellipse(frame);

    // imshow("hi", cameraImage);
    return (screen.send(frame) && (waitKey(1) < 0));
}

void teardown()
{
    system("setterm -cursor on");

    cout << endl
         << "Stopped after " << i << " frames" << endl;

    // Disconnect

    try
    {
        cout << "\nDisconnecting from the MQTT server..." << flush;
        CLIENT.disconnect()->wait();
        cout << "OK" << endl;
    }
    catch (const mqtt::exception& exc)
    {
        cerr << exc << endl;
    }

    // videoCapture.release();

    // destroyAllWindows();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}
