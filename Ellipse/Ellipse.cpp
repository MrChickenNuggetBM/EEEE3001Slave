#include "Ellipse.h"
using namespace cv;
using namespace std;

// constructors
Ellipse::Ellipse(const Point2f& newCenter, Size2f newSize, float newAngleDeg, const Scalar& newColour, int newThickness) :
	RotatedRect(newCenter, newSize, (float)fmod(newAngleDeg, 360.0))
{
	setColour(newColour);
	setThickness(newThickness);
}

Ellipse::Ellipse(const Ellipse& copiedEllipse) :
    RotatedRect(copiedEllipse.center, copiedEllipse.size, copiedEllipse.angle)
{
	setColour(copiedEllipse.getColour());
	setThickness(copiedEllipse.getThickness());
}

// operator overloads
Ellipse& Ellipse::operator=(const Ellipse& assignedEllipse)
{
	center = assignedEllipse.center;
	size = assignedEllipse.size;
	angle = assignedEllipse.angle;
	return (*this);
}

// gets
Scalar Ellipse::getColour() const { return colour; }
int Ellipse::getThickness() const { return thickness; }

float Ellipse::getMinorRadius() const { return (size.height / 2.0f); }

float Ellipse::getMajorRadius() const { return (size.width / 2.0f); }

float Ellipse::getEccentricity() const {
	float sqrMinRad(getMinorRadius());
    sqrMinRad *= sqrMinRad;

	float sqrMajRad(getMajorRadius());
    sqrMajRad *= sqrMajRad;

	return ((float)sqrt(1 - (sqrMinRad / sqrMajRad)));
}

Size2f Ellipse::getCenter() { return(center); }

float Ellipse::getArea() const { return((float)(CV_PI * getMinorRadius() * getMajorRadius())); }

void Ellipse::setColour(const Scalar& newColour)
{
	colour = newColour;
	// Validate and correct color values
	if (colour[0] < 0) {
		colour[0] = 0;
		cout << "Blue less than 0. Trimming...";
	}
	else if (colour[0] > 255) {
		colour[0] = 255;
		cout << "Blue more than 255. Trimming...";
	}

	if (colour[1] < 0) {
		colour[1] = 0;
		cout << "Green less than 0. Trimming...";
	}
	else if (colour[1] > 255) {
		colour[1] = 255;
		cout << "Green more than 255. Trimming...";
	}

	if (colour[2] < 0) {
		colour[2] = 0;
		cout << "Red less than 0. Trimming...";
	}
	else if (colour[2] > 255) {
		colour[2] = 255;
		cout << "Red more than 255. Trimming...";
	}
}

void Ellipse::setThickness(const int& newThickness) { thickness = max(1, newThickness); }

void Ellipse::setMinorRadius(float newMinorRadius) { size.width = newMinorRadius * 2; }

void Ellipse::setMajorRadius(float newMajorRadius) { size.height = newMajorRadius * 2; }

void Ellipse::setCenter(float newX, float newY) { center.x = newX; center.y = newY; }
void Ellipse::setCenterX(float newX) { center.x = newX; }
void Ellipse::setCenterY(float newY) { center.y = newY; }

bool Ellipse::isPointInside(const Point2f& point) const
{
	Rect_<float> boundingRect = boundingRect2f();
	if (!boundingRect.contains(point))
		return(false);

	float a = getMinorRadius();
	float b = getMajorRadius();
	float xDiff = point.x - center.x;
	float yDiff = point.y - center.y;


	return((xDiff * xDiff) / (a * a) + (yDiff * yDiff) / (b * b) <= 1.0f);
}

bool Ellipse::isCircle() const { return(getEccentricity() <= 0.2f); }

void Ellipse::translate(float xTranslation, float yTranslation)
{
	center.x += xTranslation;
	center.y += yTranslation;
}
void Ellipse::operator+(float xyTranslation) { translate(xyTranslation, xyTranslation); }

void Ellipse::scale(float xScale, float yScale)
{
	size.width *= xScale;
	size.height *= yScale;
}
void Ellipse::operator*(float xyScale) { scale(xyScale, xyScale); }

void Ellipse::rotate(float angleDeg) { angle += angleDeg; }
void Ellipse::rotateRad(float angleRad) { rotate((float)(angleRad * 180 / CV_PI)); }

void Ellipse::draw(const Mat& frame) const {
	ellipse(frame, *this, colour, thickness);
}

void Ellipse::operator()(const Mat& frame) const { draw(frame); }
