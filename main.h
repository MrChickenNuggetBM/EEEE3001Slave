#ifndef MAIN_HPP
#define MAIN_HPP

#define PWM_PIN 12

#include <termios.h>
#include <unistd.h>
#include <csignal>
#include "include/correction.h"

namespace mqtt
{
    const string TOPICS[] =
        {
            "parameters/xCenter",
            "parameters/yCenter",
            "parameters/xDiameter",
            "parameters/yDiameter",
            "parameters/thickness",
            "parameters/isCircle",
            "parameters/modality",
            "parameters/isGUIControl",
            "parameters/isGUIControl",
            "brightness/isAutomaticBrightness",
            "brightness/dutyCycle"};

    // mqtt broker definition
    const string SERVER_ADDRESS("mqtt://localhost:1883");
    async_client CLIENT(SERVER_ADDRESS, "raspberrypi");

    // connection OPTIONS
    connect_options OPTIONS;

    // callback
    Callback CALLBACK(CLIENT, OPTIONS, TOPICS, 11);
}

// variable for screen
Screen screen("/dev/fb1");
// screen dimensions
const int
    sWidth = screen.getWidth(),
    sHeight = screen.getHeight();

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
        // cout << "Frame: " << ndx << endl;
    }

    return 0;
}

#endif // MAIN_HPP
