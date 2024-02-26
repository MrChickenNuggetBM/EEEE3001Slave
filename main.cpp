#include "main.h"

const string TOPICS[] = {
    "cv/threshold"
};

// mqtt broker definition
const string SERVER_ADDRESS("mqtt://192.168.2.1:1883");
async_client CLIENT(SERVER_ADDRESS, "raspberrypi2");
// connection OPTIONS
connect_options OPTIONS;
// callback
Callback CALLBACK(CLIENT, OPTIONS, TOPICS, 1);

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

    try{
        auto token = publishMessage("cv/thresholdSet", "75", CLIENT);
        token->wait_for(std::chrono::seconds(10));
    }
    catch (const mqtt::exception& exc)
    {
        cerr << "Error during publish" <<endl;
    }

    return true;
}

bool loop()
{
    Mat cameraImage;
    videoCapture.read(cameraImage);

    // send image plane image to Node-RED Dashboard
    auto token = publishImage("images/backFocalPlane", cameraImage, CLIENT);
    token->wait_for(std::chrono::seconds(10));

    vector<Ellipse> ellipses = detectEllipses(cameraImage.clone(), 2);

    imshow("susThings", cameraImage);

    return (waitKey(1) < 0);
}

void teardown()
{
    system("setterm -cursor on");

    cout << "Stopping..." << endl;

    videoCapture.release();

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
