#include "main.h"

// Callback for when a message arrives.
void Callback::message_arrived(const_message_ptr msg)
{
    using namespace topics;

    // extract the topic and payload
    string topic = msg->get_topic();
    string payload = msg->to_string();

    /*
    cout << "Message arrived!" << endl;
    cout << "\ttopic: '" << topic << "'" << endl;
    cout << "\tpayload: '" << payload << "'\n" << endl;
    */

   // according to the topic
   // process the payload
    if (topic == "cv/threshold")
        cv::threshold = stoi(payload);
    else if (topic == "cv/noiseKernel")
        cv::noiseKernel = stoi(payload);
    else if (topic == "cv/adaptiveSize")
        cv::adaptiveSize = stoi(payload);
}

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

    // define camera image constants
    const float _brightness = 0.55,
                contrast = 0.50,
                saturation = 0.67;

    cout << "brightness: " << _brightness << endl;
    cout << "contrast: " << contrast << endl;
    cout << "saturation: " << saturation << endl;

    // set camera image settings
    videoCapture.set(CAP_PROP_FRAME_WIDTH, 480);
    videoCapture.set(CAP_PROP_FRAME_HEIGHT, 270);
    videoCapture.set(CAP_PROP_BRIGHTNESS, _brightness);
    videoCapture.set(CAP_PROP_CONTRAST, contrast);
    videoCapture.set(CAP_PROP_SATURATION, saturation);

    // set camera settings
    system("v4l2-ctl -c exposure_dynamic_framerate=1");
    system("v4l2-ctl -c scene_mode=8");

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

    // publish default values
    try
    {
        auto token = publishMessage("cv/thresholdSet", to_string(topics::cv::threshold));
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("cv/noiseKernelSet", to_string(topics::cv::noiseKernel));
        token->wait_for(std::chrono::seconds(10));

        token = publishMessage("cv/adaptiveSizeSet", to_string(topics::cv::adaptiveSize));
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
    // take image and rearrange
    Mat cameraImage;
    videoCapture.read(cameraImage);
    flip(cameraImage, cameraImage, 0);
    flip(cameraImage, cameraImage, 1);
    cameraImage = cameraImage(Range(70,190), Range(200,320));

    // defined Ellipses to be overrided by the detected ellipses
    static vector<Ellipse> detectedEllipses =
    {
        Ellipse(
            Point2f(
                61,
                62
            ),
            Size2f(
                54.6,
                54
            ),
            -0.695,
            Scalar(255, 0, 0, 255),
            1
        ),
        Ellipse(
            Point2f(
                61,
                63
            ),
            Size2f(
                46.6,
                46
            ),
            0.755,
            Scalar(255, 0, 0, 255),
            1
        )
    };

    // only do once in 10 loops
    // to reduce latency
    if (!(ndx % 10) && (ndx > 70))
    {
        detectedEllipses = detectEllipses(cameraImage.clone());
    }

    // put ellipses on frame
    // for (unsigned int i = 0; i < detectedEllipses.size(); i++)
    //     detectedEllipses[i](cameraImage);
    for (unsigned int i = 0; i < phaseEllipses.size(); i++)
        phaseEllipses[i](cameraImage);

    // send image plane image to Node-RED Dashboard
    auto token = publishImage("images/backFocalPlane", cameraImage);
    token->wait_for(std::chrono::seconds(10));

    // get the average of both phase rings
    Ellipse phaseAverage = ellipseAverage(phaseEllipses[0], phaseEllipses[1]);
    //phaseAverage(cameraImage);

    if (isEllipseFound)
    {
        // apply detected images to camera image
        Ellipse detectedAverage = ellipseAverage(detectedEllipses[0], detectedEllipses[1]);
        detectedAverage(cameraImage);
        detectedEllipses[0](cameraImage);
        detectedEllipses[1](cameraImage);
        // publishCorrections(detectedAverage, phaseAverage);

        // find and publish corrections to the broker
        publishCorrections(detectedAverage, phaseAverage);
    }

    // send Hough Transform image to broker
    token = publishImage("cvimages/hough",cameraImage);
    token->wait_for(std::chrono::seconds(10));

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
         << "Stopped after " << ndx << " frames" << endl;
}
