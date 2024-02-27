#ifndef CORRECTION_HPP
#define CORRECTION_HPP

#include "CV++.h"

Point2f calculateTranslationalCorrection(Ellipse elp, Ellipse refElp);
float calculateAngleCorrection(Ellipse elp, Ellipse refElp);
float calculateMajorRadiusCorrection(Ellipse elp, Ellipse refElp);
float calculateMinorRadiusCorrection(Ellipse elp, Ellipse refElp);

#endif // CORRECTION_HPP
