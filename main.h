#ifndef MAIN_HPP
#define MAIN_HPP

#define PWM_PIN 12

#include <termios.h>
#include <unistd.h>
#include <csignal>
#include <chrono>
#include "include/correction.h"

namespace mqtt
{
const string TOPICS[] =
{
    "cv/threshold",
    "cv/noiseKernel",
    "cv/adaptiveSize"
};
const int numTopics = sizeof(TOPICS)/sizeof(string);

// mqtt broker definition
const string SERVER_ADDRESS("mqtt://192.168.2.1:1883");
async_client CLIENT(SERVER_ADDRESS, "Slave");

// connection OPTIONS
connect_options OPTIONS;

// callback
Callback CALLBACK(CLIENT, OPTIONS, TOPICS, numTopics);
}

using ullint = unsigned long long int;

using namespace std;
using namespace mqtt;
using namespace cv;

VideoCapture videoCapture(0);

bool setup();
bool loop();
void teardown();
void teardown(int signal)
{
    exit(EXIT_SUCCESS);
}

ullint ndx(0);
int main(int argc, char *argv[])
{
    // run setup, if failure return
    if (!setup())
    {
        cerr << "Error in setup" << endl;
        return (-1);
    }

    // run loop until loop() returns false
    while (loop(), ++ndx)
    {
        cout << "Frame: " << ndx << endl;
    }

    return 0;
}

#endif // MAIN_HPP
