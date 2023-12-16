#ifndef MAIN_H
#define MAIN_H

#include "Ellipse/Ellipse.h"
#include "Screen/Screen.h"

using namespace cv;
using namespace std;

using ullint = unsigned long long int;

bool setup();
bool loop(ullint i);
bool teardown();

int main(int argc, char *argv[]) {
    // run setup, if failure return
    if (!setup())
    {
        cerr << "Error in setup" << endl;
        return(-1);
    }

    // run loop until loop() returns false
    ullint i(0);
    while (loop(i++));

    cout << "Stopped after " << i << " frames" << endl;

    if (!teardown())
    {
        cerr << "Error in teardown" << endl;
        return(-1);
    }

    return 0;
}

#endif // MAIN_H
