#ifndef MAIN_H
#define MAIN_H

#include "Ellipse/Ellipse.h"
#include "Screen/Screen.h"
#include <termios.h>
#include <unistd.h>
#include <csignal>
#include <fstream>

using namespace cv;
using namespace std;

using ullint = unsigned long long int;

ullint i(0);

bool sendToScreen(Mat image);
bool setup();
bool loop();
void teardown();
void teardown(int signal);

VideoCapture *videoCapture;

int main(int argc, char *argv[]) {
    // run setup, if failure return
    if (!setup())
    {
        cerr << "Error in setup" << endl;
        return(-1);
    }

    // run loop until loop() returns false
    while (loop(), ++i) {
        // cout << "Frame: " << i << endl;
    }

    return 0;
}

#endif // MAIN_H
