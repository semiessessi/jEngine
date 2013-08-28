/* gameCamera class by Semi Essessi
 *
 * (class description goes here)
 *
 */

#include "mVector.h"

#ifndef __GAMECAMERA_H
#define __GAMECAMERA_H

class gameCamera
{
private:
    mVector3d   pos;
    float       rx;
    float       ry;
    float       z;

    float       fov;
public:
    gameCamera() : pos(0.0f), rx(0.0f), ry(0.0f), z(0.0f), fov(80.0f) {}
    ~gameCamera() {}

    float getDistance() const { return -z; }
    float getPitch() const { return rx; }
    float getYaw() const { return ry; }
    float getFOV() const { return fov; }
    mVector3d getViewPosition() { return pos; }
    mVector3d getPhysicalPosition();

    void setViewPosition(mVector3d v) { pos = v; }
    void setDistance(const float& distance) { z = -distance; }
    void setPitch(const float& pitch) { rx = pitch; }
    void setYaw(const float& yaw) { ry = yaw; }

    void incrementPitch(const float& speed);
    void decrementPitch(const float& speed);
    void incrementYaw(const float& speed);
    void decrementYaw(const float& speed);
    void incrementDistance(const float& speed);
    void decrementDistance(const float& speed);

    void glTransforms();
    void glRotates();
};

#endif