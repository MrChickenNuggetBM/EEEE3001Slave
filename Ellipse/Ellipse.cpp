#include "Ellipse.h"
using namespace cv;
using namespace std;

// constructors
Ellipse::Ellipse(const Point2f& center, const Size2f& size, float angle, cv::Scalar newColour, int newThickness) :
	RotatedRect(center, size, angle),
	colour(newColour),
	thickness(newThickness) {}

Ellipse::Ellipse(const Ellipse& copiedEllipse) :
	RotatedRect(copiedEllipse.center, copiedEllipse.size, copiedEllipse.angle),
	colour(copiedEllipse.colour),
	thickness(copiedEllipse.thickness) {}

// destructor
Ellipse::~Ellipse() {}

// operator overloads
Ellipse& Ellipse::operator=(const Ellipse& assignedEllipse)
{
	center = assignedEllipse.center;
	size = assignedEllipse.size;
	angle = assignedEllipse.angle;
	return (*this);
}

// gets
float Ellipse::minorRadius() const { return (min(size.width, size.height) / 2.0f); }

float Ellipse::majorRadius() const { return (max(size.width, size.height) / 2.0f); }

float Ellipse::eccentricity() const {
	float minRad(minorRadius());
	float majRad(majorRadius());
	return (sqrt(1 - ((minRad * minRad) / (majRad * majRad))));
}

float Ellipse::area() const
{
	return(CV_PI * minorRadius() * majorRadius());
}

bool Ellipse::isPointInside(const Point2f& point)
{
	Rect_<float> boundingRect = boundingRect2f();
	if (!boundingRect.contains(point))
		return(false);

	float a = minorRadius();
	float b = majorRadius();
	float xDiff = point.x - center.x;
	float yDiff = point.y - center.y;


	return((xDiff * xDiff) / (a * a) + (yDiff * yDiff) / (b * b) <= 1.0f);
}

bool Ellipse::isCircle()
{
	return(eccentricity() <= 0.2f);
}

void Ellipse::translate(float xTranslation, float yTranslation)
{
	center.x += xTranslation;
	center.y += yTranslation;
}
void Ellipse::operator+(float xyTranslation)
{
	translate(xyTranslation, xyTranslation);
}

void Ellipse::scale(float xScale, float yScale)
{
	size.width *= xScale;
	size.height *= yScale;
}
void Ellipse::operator*(float xyScale)
{
	scale(xyScale, xyScale);
}

void Ellipse::rotate(float angleDeg)
{
	angle += angleDeg;
}
void Ellipse::rotateRad(float angleRad)
{
	rotate(angleRad * 180 / CV_PI);
}

void Ellipse::draw(Mat frame)
{
	ellipse(frame, *this, colour, thickness);
}
void Ellipse::operator()(Mat frame)
{
	draw(frame);
}
