/* This code has been modified from https://github.com/siamezzze/ellipse_detection

The MIT License (MIT)

Copyright (c) 2015 Maria Gluhova

Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
        copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/



#ifndef COMPUTER_VISION_HPP
#define COMPUTER_VISION_HPP

#include "Ellipse.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>

struct ellipse_data {

    ellipse_data() : x0(-1), y1(-1) {};

    ellipse_data(int _x0, int _y0, double _a, double _b, double th);

    ellipse_data(int _x0, int _y0, double _a, double _b, double th, int _x1, int _y1, int _x2, int _y2);

    double a, b;
    int x0, y0;
    int x1, y1, x2, y2;

    double orient;
};

std::vector<Ellipse> detectEllipses(cv::Mat src, unsigned int numEllipses = 1, int minimizedSize = 64);

#endif // COMPUTER_VISION_HPP
