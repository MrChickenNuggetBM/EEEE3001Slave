#include "../include/correction.h"

Point2f calculateTranslationalCorrection(Ellipse elp, Ellipse refElp)
{
        float x = refElp.center.x - elp.center.x;
        float y = refElp.center.y - elp.center.y;

        return Point2f(x, y);
}

float calculateAngleCorrection(Ellipse elp, Ellipse refElp)
{
        return refElp.angle - elp.angle;
}

float calculateMajorRadiusCorrection(Ellipse elp, Ellipse refElp)
{
        return refElp.getMajorAxis() - elp.getMajorAxis();
}

float calculateMinorRadiusCorrection(Ellipse elp, Ellipse refElp)
{
        return refElp.getMinorAxis() - elp.getMinorAxis();
}
