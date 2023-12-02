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
	Ellipse(const cv::Point2f& center, cv::Size2f newSize, float angle, const cv::Scalar& newColour = cv::Scalar(255, 255, 255, 1), int newThickness = 1);
	Ellipse(const Ellipse& newEllipse);

	// destructor
	~Ellipse() = default;

	// operator overloads
	Ellipse& operator=(const Ellipse& assignedEllipse);

	// gets
	cv::Scalar getColour() const;
	int getThickness() const;
	float getMinorRadius() const;
	float getMajorRadius() const;
    cv::Size2f Ellipse::getCenter();
    float getEccentricity() const;
    float getArea() const;

	// sets
	void setColour(const cv::Scalar& newColour);
	void setThickness(const int& newThickness);
    void setMinorRadius(float newMinorRadius);
    void setMajorRadius(float newMajorRadius);
    void setCenter(float newX, float newY);
    void setCenterX(float newX);
    void setCenterY(float newX);

    bool isPointInside(const cv::Point2f& point) const;
	bool isCircle() const;

	void translate(float xTranslation, float yTranslation);
	void operator+(float xyTranslation);

	void scale(float xScale, float yScale);
	void operator*(float xyScale);

	void rotate(float angleDeg);
	void rotateRad(float angleRad);

	void draw(const cv::Mat& frame) const;
	void operator()(const cv::Mat& frame) const;
};

#endif // ELLIPSE_H
