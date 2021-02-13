#include "Particle.h"

Particle::Particle(float x, float y) : x(x), y(y)
{}

Particle::Particle(const Particle& p) : x(p.x), y(p.y)
{}

float Particle::X() const
{
    return x;
}

float Particle::Y() const
{
    return y;
}

float Particle::FindDistance(const Particle& p) const
{
    float dist_x = x - p.x;
    float dist_y = y - p.y;
    return sqrt(dist_x*dist_x + dist_y*dist_y);
}

bool Particle::operator== (const Particle& p) const
{
    float dist_x = abs(x - p.x);
    float dist_y = abs(y - p.y);

    if(dist_x >= EPSILON || dist_y >= EPSILON)
        return false;
    return true;
}

ostream& operator<<(ostream& os, const Particle& p)
{
    os << '(' << p.x << ',' << p.y << ')';
    return os;
}
