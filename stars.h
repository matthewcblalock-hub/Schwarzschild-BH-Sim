#pragma once

// Foreground stars for blackhole to bend:
struct star
{
    double x,y,z, radius;
    float r,g,b;
};

inline star sceneStars[] = 
{
    {40.0, 8.0, 0.0, 4.0, 0.3f, 0.0f, 1.0f}, // Orangish
    {-35.0, -5.0, 20.0,  3.0,  1.0f, 0.9f, 0.7f} // Bluish
};

inline int numStars = 2;

// Returns true if point is inside any star:
inline bool hitStar(double px, double py, double pz, float &r, float &g, float &b)
{
    for(int s = 0; s < numStars; s++)
    {
        double dx = px - sceneStars[s].x;
        double dy = py - sceneStars[s].y;
        double dz = pz - sceneStars[s].z;
        double d2 = dx*dx + dy*dy + dz*dz;

        if (d2 <= sceneStars[s].radius * sceneStars[s].radius)
        {
            r = sceneStars[s].r;
            g = sceneStars[s].g;
            b = sceneStars[s].b;
            return true;
        }
    }
    return false;
}