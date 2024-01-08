#ifndef MAIN_H
#define MAIN_H

#include "Ellipse/Ellipse.h"
#include "Screen/Screen.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <csignal>
#include <fstream>
#include <mqtt/async_client.h>

using ullint = unsigned long long int;

using namespace std;
using namespace mqtt;
using namespace cv;

// VideoCapture videoCapture(0);

bool display(Mat &image);
bool setup();
bool loop();
void teardown();
void teardown(int signal);

ullint i(0);
int main(int argc, char *argv[]) {
    // run setup, if failure return
    if (!setup())
    {
        cerr << "Error in setup" << endl;
        return(-1);
    }

    // run loop until loop() returns false
    while (loop(), ++i) {
        cout << "Frame: " << i << endl;
    }

    return 0;
}

#endif // MAIN_H
