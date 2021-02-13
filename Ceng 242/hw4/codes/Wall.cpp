#include "Wall.h"

Wall::Wall(float l) : start_p(0,0), end_p(0,0), length(l), straight(true)
{}

Wall::Wall(const Particle& p1, const Particle& p2) : start_p(p1), end_p(p2), length(0), straight(true)
{
    ComputeLength();
}

Wall::Wall(const Wall& rhs) : start_p(rhs.start_p), end_p(rhs.end_p), length(rhs.length), straight(rhs.straight)
{}

const Particle& Wall::GetInitialParticle() const
{
    return start_p;
}

const Particle& Wall::GetFinalParticle() const
{
    return end_p;
}

float Wall::FindAngleBetween(const Wall& rhs) const
{
    float angle1 = atan2(end_p.Y()-start_p.Y(), end_p.X()-start_p.X());
    float angle2 = atan2(rhs.end_p.Y()-rhs.start_p.Y(), rhs.end_p.X()-rhs.start_p.X());
    float result = angle1 - angle2;
    if(result <= 0)
        result += 2*3.14159265;
    return result;
}

float Wall::GetLength() const
{
    return length;
}

void Wall::ComputeLength()
{
    length = end_p.FindDistance(start_p);
}

Wall* Wall::Clone() const
{
    Wall* newWall = new Wall(*this);
    return newWall;
}

bool Wall::IsContinuousLinear(const Wall& rhs) const
{
    if(!rhs.straight)
        return false;
    if(!(end_p == rhs.start_p || start_p == rhs.end_p))
        return false;
    if(abs(end_p.X() - start_p.X()) < EPSILON)
    {
        if(abs(rhs.end_p.X() - rhs.start_p.X()) < EPSILON)
            return true;
        return false;
    }
    if(abs(rhs.end_p.X() - rhs.start_p.X()) < EPSILON)
        return false;
    float m1 = (end_p.Y() - start_p.Y()) / (end_p.X() - start_p.X());
    float m2 = (rhs.end_p.Y() - rhs.start_p.Y()) / (rhs.end_p.X() - rhs.start_p.X());
    if(abs(m1 - m2) < EPSILON)
        return true;
    return false;
}

const Wall& Wall::operator+(const Wall& rhs) const
{
    if(!IsContinuousLinear(rhs))
        throw ApplePearException();
    Wall* newWall;
    if(start_p == rhs.end_p)
        newWall = new Wall(rhs.start_p, end_p);
    else
        newWall = new Wall(start_p, rhs.end_p);
    return *newWall;
}

Wall::~Wall()
{}
