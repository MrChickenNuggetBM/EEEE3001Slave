#include "Screen.h"
using namespace std;
using namespace cv;

// constructors
    Screen::Screen(const cv::Point2f& center, cv::Size2f newSize, float angle, const cv::Scalar& newColour = cv::Scalar(255, 255, 255, 1), int newThickness = 1);
    Screen::Screen(const Screen& newScreen);

    // destructor
    Screen::~Screen() = default;

    // operator overloads
    Screen& Screen::operator=(const Screen& assignedScreen);

    void Screen::send(Mat image)()
    {
        // Assuming your second display is /dev/fb1 (adjust if needed)
        const char* framebufferPath = "/dev/fb1";

        // Get framebuffer information
        int fb = open(framebufferPath, O_RDWR);
        if (fb == -1)
        {
            cerr << "Error opening framebuffer device" << endl;
            return -1;
        }

        struct fb_var_screeninfo vinfo;
        if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo))
        {
            cerr << "Error reading variable information" << endl;
            close(fb);
            return -1;
        }

        // Calculate the size of the framebuffer
        size_t screensize = vinfo.yres_virtual * vinfo.xres_virtual * vinfo.bits_per_pixel / 8;

        // Map framebuffer to user space
        unsigned char* framebuffer = (unsigned char*)mmap(nullptr, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
        if (framebuffer == MAP_FAILED)
        {
            cerr << "Error mapping framebuffer device to memory" << endl;
            close(fb);
            return -1;
        }

        // Copy OpenCV image to the framebuffer
        for (int y = 0; y < image.rows; ++y)
        {
            for (int x = 0; x < image.cols; ++x)
            {
                // Assuming your image is in BGR format
                unsigned char* pixel = &framebuffer[(vinfo.yoffset + y) * vinfo.xres_virtual * 4 + (vinfo.xoffset + x) * 4];
                pixel[0] = image.at<Vec3b>(y, x)[0]; // Blue
                pixel[1] = image.at<Vec3b>(y, x)[1]; // Green
                pixel[2] = image.at<Vec3b>(y, x)[2]; // Red
                pixel[3] = 0; // Alpha (transparency)
            }
        }

        // Cleanup and close the framebuffer
        munmap(framebuffer, screensize);
        close(fb);
    }
    void Screen::operator()(Mat image) { send(image); }
