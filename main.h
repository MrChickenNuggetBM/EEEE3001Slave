#ifndef MAIN_HPP
#define MAIN_HPP

#include "include/CV++.h"
#include <termios.h>
#include <unistd.h>
#include <csignal>

#define PWM_PIN 12

using ullint = unsigned long long int;

using namespace std;
using namespace mqtt;
using namespace cv;

VideoCapture videoCapture(0);

bool setup();
bool loop();
void teardown();
void teardown(int signal);

ullint ndx(0);
int main(int argc, char *argv[]) {
    // run setup, if failure return
    if (!setup())
    {
        cerr << "Error in setup" << endl;
        return(-1);
    }

    // run loop until loop() returns false
    while (loop(), ++ndx) {
        // cout << "Frame: " << ndx << endl;
    }

    return 0;
}

#endif // MAIN_HPP
