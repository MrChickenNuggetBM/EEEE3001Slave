#ifndef MAIN_H
#define MAIN_H

#include "Ellipse/Ellipse.h"
#include <iostream>
using namespace cv;
using namespace std;

using ullint = unsigned long long int;

bool setup();
bool loop(ullint i);

int main(int argc, char *argv[]) {
    // run setup, if failure return
    if (!setup()) return(1);

    // run loop until loop() returns false
    ullint i(0);
    while (loop(i++));

    return 0;
}

#endif // MAIN_H
