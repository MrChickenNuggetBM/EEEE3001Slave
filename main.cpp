#include "main.h"

const string TOPICS[]  =
{
    "parameters/xCenter",
    "parameters/yCenter",
    "parameters/xRadius",
    "parameters/yRadius",
    "parameters/thickness",
    "parameters/isCircle",
    "parameters/isBrightfield",
    "parameters/isGUIControl"
};

// mqtt broker definition
const string SERVER_ADDRESS("mqtt://localhost:1883");
async_client CLIENT(SERVER_ADDRESS, "raspberrypi");

// connection OPTIONS
connect_options OPTIONS;

// callback
Callback CALLBACK(CLIENT, OPTIONS, TOPICS, 8);

// variable for screen
Screen screen;

bool setup()
{
    // clear the terminal
    system("setterm -cursor off;clear");

    // set up the camera
    if (!videoCapture.isOpened())
    {
        cerr << "Error: Could not open camera" << endl;
        return false;
    }

    videoCapture.set(CAP_PROP_FRAME_WIDTH, 480);
    videoCapture.set(CAP_PROP_FRAME_HEIGHT, 270);

    // configure code termination
    atexit(teardown);
    signal(SIGINT, teardown);

    // establish broker-client connection

    OPTIONS.set_clean_session(false);

    // Install the callback(s) before connecting.

    CLIENT.set_callback(CALLBACK);

    // Start the connection.
    // When completed, the callback will subscribe to topic.
    try
    {
        cout << "Connecting to the MQTT server..." << flush;
        auto connectToken = CLIENT.connect(OPTIONS, nullptr, CALLBACK);
        connectToken->wait_for(std::chrono::seconds(10));
    }
    catch (const mqtt::exception& exc)
    {
        cerr << "\nERROR: Unable to connect to MQTT server: '"
             << SERVER_ADDRESS << "'" << exc << endl;
        return false;
    }

    // publishing the default values
    try
    {
        using namespace topics::parameters;
        auto token = publishMessage("parameters/xCenterSet", "0", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/yCenterSet", "0", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/xRadiusSet", "960", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/yRadiusSet", "540", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/thicknessSet", "5", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/isCircleSet", "false", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/isBrightfieldSet", "true", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/isGUIControlSet", "false", CLIENT);
        token->wait_for(std::chrono::seconds(10));
    }
    catch (const mqtt::exception& exc)
    {
        std::cerr << "Error during publish" << exc.what() << std::endl;
    }

    return true;
}

bool loop()
{
    Mat bfpImage;
    videoCapture.read(bfpImage);

    int _xCenter = 0, _yCenter = 0,
        _xRadius = 960, _yRadius = 540,
        _thickness = 3,
        _ringColour = 255 * (int) topics::parameters::isBrightfield;
    bool _isCircle = false;

    // if GUIControl
    // retrieving stored parameters from MQTT
    if (topics::parameters::isGUIControl)
    {
        using namespace topics::parameters;
        _xCenter = xCenter;
        _yCenter = yCenter;
        _xRadius = xRadius;
        _yRadius = yRadius;
        _thickness = thickness;
        _isCircle = isCircle;
    }
    else {}

    // define the ringImage frame
    Mat ringImage(
        1080,
        1920,
        CV_8UC4,
        Scalar(
            255 - _ringColour,
            255 - _ringColour,
            255 - _ringColour,
            255
        )
    );

    // define the ellipse
    Ellipse ellipse(
        Point2f(
            960 + _xCenter,
            540 + _yCenter
        ),
        Size2f(
            2 * (_isCircle ? std::min(_xRadius, _yRadius) : _xRadius),
            2 * (_isCircle ? std::min(_xRadius, _yRadius) : _yRadius)
        ),
        0,
        Scalar(
            _ringColour,
            _ringColour,
            _ringColour
        ),
        _thickness
    );

    // put the ellipse in ringImage
    ellipse(ringImage);

    // send images
    auto token = publishImage("images/backfocalplane", bfpImage, CLIENT);
    token->wait_for(std::chrono::seconds(10));

    token = publishImage("images/ring", ringImage, CLIENT);
    token->wait_for(std::chrono::seconds(10));

    // imshow("hi", cameraImage);
    return (screen.send(ringImage) && (waitKey(1) < 0));
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
