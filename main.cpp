#include "main.h"

int main() {
    // Create an Ellipse with default values
    Ellipse defaultEllipse(cv::Point2f(100, 100), cv::Size2f(50, 30), 45);

    // Create an Ellipse with custom color and thickness
    cv::Scalar customColor(0, 255, 0); // Green color
    int customThickness = 2;
    Ellipse customEllipse(cv::Point2f(200, 200), cv::Size2f(40, 40), 30, customColor, customThickness);

    // Output information about the ellipses
    std::cout << "Default Ellipse:\n";
    std::cout << "Color: " << defaultEllipse.getColour() << "\n";
    std::cout << "Thickness: " << defaultEllipse.getThickness() << "\n";
    std::cout << "Is Circle? " << (defaultEllipse.isCircle() ? "Yes" : "No") << "\n\n";

    std::cout << "Custom Ellipse:\n";
    std::cout << "Color: " << customEllipse.getColour() << "\n";
    std::cout << "Thickness: " << customEllipse.getThickness() << "\n";
    std::cout << "Is Circle? " << (customEllipse.isCircle() ? "Yes" : "No") << "\n\n";

    return 0;
}
