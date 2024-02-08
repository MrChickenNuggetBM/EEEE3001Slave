#include "main.h"

const string TOPICS[]  =
{
    "parameters/xCenter",
    "parameters/yCenter",
    "parameters/xDiameter",
    "parameters/yDiameter",
    "parameters/thickness",
    "parameters/isCircle",
    "parameters/isBrightfield",
    "parameters/isGUIControl",
    "parameters/isGUIControl",
    "brightness/isAutomaticBrightness",
    "brightness/dutyCycle"
};

// mqtt broker definition
const string SERVER_ADDRESS("mqtt://localhost:1883");
async_client CLIENT(SERVER_ADDRESS, "raspberrypi");

// connection OPTIONS
connect_options OPTIONS;

// callback
Callback CALLBACK(CLIENT, OPTIONS, TOPICS, 11);

// variable for screen
Screen screen("/dev/fb1");
// screen dimensions
const int
    sWidth = screen.getWidth(),
    sHeight = screen.getHeight();

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

    // set the PWM signal
    if (gpioInitialise() < 0)
        return false;
    gpioSetMode(PWM_PIN, PI_OUTPUT);
    gpioSetPWMfrequency(PWM_PIN, 1000);
    gpioSetPWMrange(PWM_PIN, 100);
    gpioPWM(PWM_PIN, 50);

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

        token = publishMessage("parameters/xDiameterSet", "33", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/yDiameterSet", "60", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/thicknessSet", "150", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/isCircleSet", "false", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/isBrightfieldSet", "false", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("parameters/isGUIControlSet", "false", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("brightness/isAutomaticBrightnessSet", "true", CLIENT);
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("brightness/dutyCycleSet", "50", CLIENT);
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
    // do computer vision -----------------------
    Mat bfpImage;
    videoCapture.read(bfpImage);
    // replace default values
    int _xCenter = 0, _yCenter = 0,
        _xDiameter = sWidth * 0.33,
        _yDiameter = sHeight * 0.6,
        _thickness = 150,
        _dutyCycle = 50;
    bool _isCircle = false;
    // ------------------------------------------

    // if GUIControl
    // retrieving stored parameters from MQTT
    if (topics::parameters::isGUIControl)
    {
        using namespace topics::parameters;
        // percentage
        _xCenter = xCenter * sWidth / 100;
        _yCenter = yCenter * sHeight / 100;
        _xDiameter = xDiameter * sWidth / 100;
        _yDiameter = yDiameter * sHeight / 100;
        _thickness = thickness;
        _isCircle = isCircle;
    }

    // if AutomaticBrightness
    // retrieving stored parameters from MQTT
    if (!topics::brightness::isAutomaticBrightness)
        _dutyCycle = topics::brightness::dutyCycle;

    // if is brightfield, fill the circle
    if (topics::parameters::isBrightfield)
        _thickness = -1;

    // define the ringImage frame
    Mat ringImage(
        1080,
        1920,
        CV_8UC4,
        Scalar(0, 0, 0, 0)
    );

    // define the ellipse
    Ellipse ellipse(
        Point2f(
            960 + _xCenter,
            540 + _yCenter
        ),
        Size2f(
            _isCircle ? std::min(_xDiameter, _yDiameter) : _xDiameter,
            _isCircle ? std::min(_xDiameter, _yDiameter) : _yDiameter
        ),
        0,
        Scalar(255, 255, 255, 255),
        _thickness
    );

    // put the ellipse in ringImage
    ellipse(ringImage);

    // send ring image to Node-RED Dashboard
    auto token = publishImage("images/ring", ringImage, CLIENT);
    token->wait_for(std::chrono::seconds(10));

    // send bfp image to Node-RED Dashboard
    token = publishImage("images/backfocalplane", bfpImage, CLIENT);
    token->wait_for(std::chrono::seconds(10));

    // set the duty cycle
    gpioPWM(PWM_PIN, _dutyCycle);

    // imshow("hi", cameraImage);
    screen.send(ringImage);
    return (waitKey(1) < 0);
}

void teardown()
{
    system("setterm -cursor on");

    cout << "Stopping..." << endl;

    Mat emptyFrame(1080, 1920, CV_8UC4, Scalar(0,0,0,255));
    screen.send(emptyFrame);

    gpioTerminate();

    videoCapture.release();
    // destroyAllWindows();

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

    cout << endl
         << "Stopped after " << i << " frames" << endl;
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}
