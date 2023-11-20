#include "main.h"
using namespace std;
using namespace cv;

int main() {
    // Create an Ellipse with default values
    Ellipse defaultEllipse(Point2f(100, 100), Size2f(50, 30), 45);

    // Create an Ellipse with custom color and thickness
    Scalar customColor(0, 255, 0); // Green color
    int customThickness = 2;
    Ellipse customEllipse(Point2f(200, 200), Size2f(40, 40), 30, customColor, customThickness);

    // Output information about the ellipses
    cout << "Default Ellipse:\n";
    cout << "Color: " << defaultEllipse.getColour() << "\n";
    cout << "Thickness: " << defaultEllipse.getThickness() << "\n";
    cout << "Is Circle? " << (defaultEllipse.isCircle() ? "Yes" : "No") << "\n\n";

    cout << "Custom Ellipse:\n";
    cout << "Color: " << customEllipse.getColour() << "\n";
    cout << "Thickness: " << customEllipse.getThickness() << "\n";
    cout << "Is Circle? " << (customEllipse.isCircle() ? "Yes" : "No") << "\n\n";

    return 0;
}
