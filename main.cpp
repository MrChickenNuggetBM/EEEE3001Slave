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


int main() {
    // 📸 Image file path
    const char* imageFilePath = "your_image.jpg";

    // 🚨 Check if the image file exists
    if (access(imageFilePath, F_OK) == -1) {
        cerr << "Error: Image file not found" << endl;
        return -1;
    }

    // 📸 Read the image
    Mat image = imread(imageFilePath);
    
    // 🚀 Assuming your second display is /dev/fb1 (adjust if needed)
    const char* framebufferPath = "/dev/fb1";

    // 📏 Get framebuffer information
    int fb = open(framebufferPath, O_RDWR);
    if (fb == -1) {
        cerr << "Error opening framebuffer device" << endl;
        return -1;
    }

    // ... (Rest of the code remains unchanged)

    // 🔄 Cleanup and close the framebuffer
    munmap(framebuffer, screensize);
    close(fb);

    return 0;
}
