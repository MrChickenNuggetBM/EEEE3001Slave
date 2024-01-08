#include "main.h"

// mqtt broker
async_client client("mqtt://localhost:1883", "raspberrypi");

// defining the callback
class _Callback : public virtual callback
{
public:
    void connection_lost(const std::string& cause) override {
        std::cout << "Connection lost: " << cause << std::endl;
    }

    void message_arrived(const_message_ptr msg) override
    {
        int whyme = 5;
        float ccuz = 6.7;
        cout << "sus1" << endl;
        cout << "Message received: " << msg->to_string() << endl;
        cout << "Payload received: " << msg->get_payload_str() << endl;
        cout << "sus2" << endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override {}
};

bool setup()
{
    atexit(teardown);
    signal(SIGINT, teardown);

    // establish broker-client connection
    connect_options connOpts;
    connOpts.set_clean_session(true);

    _Callback _callback;
    client.set_callback(_callback);

    try
    {
        client.connect(connOpts)->wait();
        client.subscribe("parameters/isCircle", 1)->wait();
        client.subscribe("parameters/minorRadius", 1)->wait();
        client.subscribe("parameters/majorRadius", 1)->wait();
        client.subscribe("parameters/thickness", 1)->wait();
    }
    catch (const mqtt::exception &exc)
    {
        cerr << "Error: " << exc.what() << endl;
        return false;
    }

    return true;
}

bool loop()
{
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

    return true;
}

void teardown()
{
    client.disconnect()->wait();
}

void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}

