#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <opencv2/opencv.hpp>

class Ellipse : public cv::RotatedRect
{
public:
	// constructors
	Ellipse(const cv::Point2f& center, const cv::Size2f& size, float angle, cv::Scalar newColour = (255, 255, 255), int newThickness = 1);
	Ellipse(const Ellipse& newEllipse);

	// destructor
	~Ellipse();

	// operator overloads
	Ellipse& operator=(const Ellipse& assignedEllipse);

	// public variables to store the colour and thickness of the ellipse
	cv::Scalar colour;
	int thickness;

	// gets
	float minorRadius() const;
	float majorRadius() const;
	float eccentricity() const;
	float area() const;

	bool isPointInside(const cv::Point2f& point);
	bool isCircle();

	void translate(float xTranslation, float yTranslation);
	void operator+(float xyTranslation);

	void scale(float xScale, float yScale);
	void operator*(float xyScale);

	void rotate(float angleDeg);
	void rotateRad(float angleRad);

	void draw(cv::Mat frame);
	void operator()(cv::Mat frame);
};

#endif // ELLIPSE_H
