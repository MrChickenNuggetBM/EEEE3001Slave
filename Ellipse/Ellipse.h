#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <string>
#include <utility>

template<typename radiiType, typename posType>
class Ellipse
{
private:
	std::string name;
	std::pair<posType, posType> center;
	radiiType minorRadius;
	radiiType majorRadius;
	
public:
	// constructors
	Ellipse(radiiType newMinorRadius, radiiType newMajorRadius, std::pair<posType, posType> newCenter);
	Ellipse(std::string newName, radiiType newMinorRadius, radiiType newMajorRadius, std::pair<posType, posType> newCenter);
	Ellipse(Ellipse& newEllipse);

	// deconstructor
	~Ellipse();

	// operator overloads
	Ellipse& operator=(Ellipse& assignedEllipse);

};

#endif // ELLIPSE_H
