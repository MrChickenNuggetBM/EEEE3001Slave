#ifndef CORRECTION_HPP
#define CORRECTION_HPP

#include "../Shared/include/CV++.h"

// declare functions to calculate corrections
cv::Point2f calculateTranslationalCorrection(const Ellipse& elp, const Ellipse& refElp);
float calculateAngleCorrection(const Ellipse& elp, const Ellipse& refElp);
float calculateMajorRadiusCorrection(const Ellipse& elp, const Ellipse& refElp);
float calculateMinorRadiusCorrection(const Ellipse& elp, const Ellipse& refElp);

// declare function which publishes corrections
// corrections are calculated using the functions above
void publishCorrections(const Ellipse& elp, const Ellipse& refElp);

#endif // CORRECTION_HPP
