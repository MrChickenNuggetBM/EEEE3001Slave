#include "main.h"

// bool setup()
// {
//     return true;
// }

// bool loop(ullint i)
// {
//     if (i > 100)
//         return false;

//     cout << i << endl;
//     return true;
// }


int main(int argc, char *argv[]) {
    // Image file path
    const char* imageFilePath = "test.jpg";

    // Check if the image file exists
    if (access(imageFilePath, F_OK) == -1) {
        cerr << "Error: Image file not found" << endl;
        return -1;
    }

    // Read the image
    Mat image = imread(imageFilePath);

    // Assuming your second display is /dev/fb1 (adjust if needed)
    const char* framebufferPath = "/dev/fb1";

    // Get framebuffer information
    int fb = open(framebufferPath, O_RDWR);
    if (fb == -1) {
        cerr << "Error opening framebuffer device" << endl;
        return -1;
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
        cerr << "Error reading variable information" << endl;
        close(fb);
        return -1;
    }

    // Calculate the size of the framebuffer
    size_t screensize = vinfo.yres_virtual * vinfo.xres_virtual * vinfo.bits_per_pixel / 8;

    // Map framebuffer to user space
    unsigned char* framebuffer = (unsigned char*)mmap(nullptr, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if (framebuffer == MAP_FAILED) {
        cerr << "Error mapping framebuffer device to memory" << endl;
        close(fb);
        return -1;
    }

    // Copy OpenCV image to the framebuffer
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            // Assuming your image is in BGR format
            unsigned char* pixel = &framebuffer[(vinfo.yoffset + y) * vinfo.xres_virtual * 4 + (vinfo.xoffset + x) * 4];
            pixel[0] = image.at<Vec3b>(y, x)[0]; // Blue
            pixel[1] = image.at<Vec3b>(y, x)[1]; // Green
            pixel[2] = image.at<Vec3b>(y, x)[2]; // Red
            pixel[3] = 0; // Alpha (transparency)
        }
    }

    // Optionally, you can add a delay to display the image for some time
    usleep(5000000); // 5 seconds

    // Cleanup and close the framebuffer
    munmap(framebuffer, screensize);
    close(fb);

    return 0;
}
