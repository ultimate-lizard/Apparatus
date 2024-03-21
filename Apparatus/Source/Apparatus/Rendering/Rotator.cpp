#include "Rotator.h"

Rotator::Rotator(float pitch, float yaw, float roll)
{
    angles[Euler::Pitch] = pitch;
    angles[Euler::Yaw] = yaw;
    angles[Euler::Roll] = roll;
}

glm::quat Rotator::asQuat() const
{
    return glm::normalize(glm::quat(glm::radians(glm::vec3(angles[0], angles[1], angles[2]))));
}

float Rotator::get(Euler angle) const
{
    if (angle < Euler::Pitch || angle > Euler::Roll)
    {
        return 0.0f;
    }

    return angles[angle];
}

void Rotator::set(float degree, Euler angle)
{
    if (angle < Euler::Pitch || angle > Euler::Roll)
    {
        return;
    }

    angles[angle] = degree;
}

void Rotator::rotate(float degree, Euler angle)
{
    if (angle < Euler::Pitch || angle > Euler::Roll)
    {
        return;
    }

    if (angles[angle] + degree >= 360.0f)
    {
        angles[angle] = angles[angle] + degree - 360.0f;
    }
    else if (angles[angle] + degree < 0.0f)
    {
        angles[angle] = angles[angle] + degree + 360.0f;
    }
    else
    {
        angles[angle] += degree;
    }
}

void Rotator::clamp(float min, float max, Euler angle)
{
    if (angle < Euler::Pitch || angle > Euler::Roll)
    {
        return;
    }
    
    const float minCorrected = min - 180.0f;
    const float maxCorrected = max - 180.0f;

    float currentAngle = angles[angle] - 180.0f;
    if (angles[angle] > 180.0f)
    {
        currentAngle -= 180.0f;
    }
    else if (angles[angle] < 180.0f)
    {
        currentAngle += 180.0f;
    }

    if (currentAngle < minCorrected)
    {
        angles[angle] = max;
    }
    else if (currentAngle > maxCorrected)
    {
        angles[angle] = min;
    }
}
