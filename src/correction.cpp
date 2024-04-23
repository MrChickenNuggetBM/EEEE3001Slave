#include "../include/correction.h"
using namespace  cv;
using namespace std;
using namespace mqtt;

Point2f calculateTranslationalCorrection(const Ellipse& elp, const Ellipse& refElp)
{
    float x = refElp.center.x - elp.center.x;
    float y = refElp.center.y - elp.center.y;

    return Point2f(x, y);
}

float calculateAngleCorrection(const Ellipse& elp, const Ellipse& refElp)
{
    return refElp.angle - elp.angle;
}

float calculateMajorRadiusCorrection(const Ellipse& elp, const Ellipse& refElp)
{
        return refElp.getMajorAxis() - elp.getMajorAxis();
}

float calculateMinorRadiusCorrection(const Ellipse& elp, const Ellipse& refElp)
{
        return refElp.getMinorAxis() - elp.getMinorAxis();
}

void publishCorrections (const Ellipse& elp, const Ellipse& refElp)
{
    Point2f point = calculateTranslationalCorrection(elp, refElp);
    auto token = publishMessage("cv/x-correction", to_string(point.x));
    token->wait_for(std::chrono::seconds(10));
    token = publishMessage("cv/y-correction", to_string(point.y));
    token->wait_for(std::chrono::seconds(10));

    float angle = calculateAngleCorrection(elp, refElp);
    token = publishMessage("cv/angle-correction", to_string(angle));
    token->wait_for(std::chrono::seconds(10));

    float minRd = calculateMajorRadiusCorrection(elp, refElp);
    token = publishMessage("cv/minRad-correction", to_string(minRd));
    token->wait_for(std::chrono::seconds(10));

    float majRd = calculateMinorRadiusCorrection(elp, refElp);
    token = publishMessage("cv/majRad-correction", to_string(majRd));
    token->wait_for(std::chrono::seconds(10));

    token = publishMessage("cv/isNewValues", "true");
    token->wait_for(std::chrono::seconds(10));
}
