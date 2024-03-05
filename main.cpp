#include "main.h"

// Callback for when a message arrives.
void Callback::message_arrived(const_message_ptr msg)
{
    using namespace topics;

    string topic = msg->get_topic();
    string payload = msg->to_string();

    /*
    cout << "Message arrived!" << endl;
    cout << "\ttopic: '" << topic << "'" << endl;
    cout << "\tpayload: '" << payload << "'\n" << endl;
    */

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
    Mat cameraImage;
    videoCapture.read(cameraImage);

    // send image plane image to Node-RED Dashboard
    auto token = publishImage("images/backFocalPlane", cameraImage);
    token->wait_for(std::chrono::seconds(10));

    // only do once in 10 loops
    // to reduce latency
    static vector<Ellipse> detectedEllipses;
    if (!(ndx % 10))
        detectedEllipses = detectEllipses(cameraImage.clone());

    // put ellipses on frame
    // for (unsigned int i = 0; i < detectedEllipses.size(); i++)
    //     detectedEllipses[i](cameraImage);
    for (unsigned int i = 0; i < phaseEllipses.size(); i++)
        phaseEllipses[i](cameraImage);

    Ellipse phaseAverage = ellipseAverage(phaseEllipses[0], phaseEllipses[1]);
    //phaseAverage(cameraImage);

    if (isEllipseFound)
    {
        Ellipse detectedAverage = ellipseAverage(detectedEllipses[0], detectedEllipses[1]);
        detectedAverage(cameraImage);
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
