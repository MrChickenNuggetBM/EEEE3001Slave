#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <opencv2/opencv.hpp>

class Ellipse : public cv::RotatedRect
{
private:
	// public variables to store the colour and thickness of the ellipse
	cv::Scalar colour;
	int thickness;

public:
	// constructors
	Ellipse(const cv::Point2f& center, cv::Size2f newSize, float angle, const cv::Scalar& newColour = (255, 255, 255), int newThickness = 1);
	Ellipse(const Ellipse& newEllipse);

	// destructor
	~Ellipse();

	// operator overloads
	Ellipse& operator=(const Ellipse& assignedEllipse);

	// gets
	cv::Scalar getColour() const;
	int getThickness() const;
	float minorRadius() const;
	float majorRadius() const;
	float eccentricity() const;
	float area() const;

	// sets
	void setColour(const cv::Scalar& newColour);
	void setThickness(const int& newThickness);

	bool isPointInside(const cv::Point2f& point) const;
	bool isCircle() const;

	void translate(float xTranslation, float yTranslation);
	void operator+(float xyTranslation);

	void scale(float xScale, float yScale);
	void operator*(float xyScale);

	void rotate(float angleDeg);
	void rotateRad(float angleRad);

	cv::Mat draw(const cv::Mat& frame) const;
	cv::Mat operator()(cv::Mat frame) const;
};

#endif // ELLIPSE_H
