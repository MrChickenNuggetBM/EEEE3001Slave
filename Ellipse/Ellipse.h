#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <string>
#include <utility>

template<typename radiiType, typename posType>
class Ellipse
{
private:
	// constructors
	Ellipse(radiiType newMinorRadius, radiiType newMajorRadius, pair<posType, posType> newCenter);
	Ellipse(String newName, radiiType newMinorRadius, radiiType newMajorRadius, pair<posType, posType> newCenter);
	Ellipse(Ellipse& newEllipse);

	// deconstructor
	~Ellipse();

	// operator overloads
	Ellipse& operator=(Ellipse& assignedEllipse);
public:
};

#endif // ELLIPSE_H
