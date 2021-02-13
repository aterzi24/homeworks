#include "CurvyWall.h"

CurvyWall::CurvyWall(float length, float radius) : Wall(length), center(0,0), radius(radius)
{
    straight = false;
}

CurvyWall::CurvyWall(const Particle& st, const Particle& end, const Particle& cent) : Wall(st, end), center(cent)
{
    straight = false;
    radius = center.FindDistance(st);
    ComputeLength();
}

CurvyWall::CurvyWall(const CurvyWall& rhs) : Wall(rhs), center(rhs.center), radius(rhs.radius)
{}

float CurvyWall::GetRadius() const
{
    return radius;
}

const Particle& CurvyWall::GetCenter() const
{
    return center;
}

void CurvyWall::ComputeLength()
{
    Wall w1(center, start_p);
    Wall w2(center, end_p);
    length = radius * w2.FindAngleBetween(w1);
}

Wall* CurvyWall::Clone() const
{
    Wall* newWall = new CurvyWall(*this);
    return newWall;
}

bool CurvyWall::IsContinuousLinear(const Wall& rhs) const
{
    return false;
}

const Wall& CurvyWall::operator+(const Wall& wall) const
{
    if(wall.straight)
        throw ApplePearException();
    CurvyWall *rhs = (CurvyWall *)&wall;
    if(!(center == rhs->center))
        throw ApplePearException();
    if(!(end_p == rhs->start_p || start_p == rhs->end_p))
        throw ApplePearException();

    CurvyWall* newWall;
    if(start_p == rhs->end_p)
        newWall = new CurvyWall(rhs->start_p, end_p, center);
    else
        newWall = new CurvyWall(start_p, rhs->end_p, center);
    return *newWall;
}

CurvyWall::~CurvyWall()
{}
