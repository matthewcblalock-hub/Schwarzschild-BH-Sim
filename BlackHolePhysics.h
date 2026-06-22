#pragma once
#include "Vec.h"
#include <cmath>

// Physical Constants:
const double C = 299792458.0; // Speed in M/s
const double G = 6.67430e-11;   // Gravitational constant

struct BlackHole
{
    Vec3 position = Vec3(0,0,0);
    double mass;
    double r_s;

    BlackHole(Vec3 position, double mass) : position(position), mass(mass)
    {
        r_s = (2.0 * G*mass)/ (C*C);
    }
};

inline BlackHole SagA(Vec3(0.0f, 0.0f, 0.0f), 8.54e36);

struct GRRay
{
    double x,y,z;
    double r, theta, phi;
    double dr, dtheta, dphi;
    double E;

    GRRay(Vec3 pos,Vec3 dir) : x(pos.x), y(pos.y), z(pos.z)
    {
        r = std::sqrt(x*x + y*y + z*z);
        theta = std::acos(z/r);
        phi = std::atan2(y,x);

        double dx = dir.x, dy = dir.y, dz = dir.z;

        dr = (dx) * (std::sin(theta) * std::cos(phi)) + (dy) * (std::sin(theta) * std::sin(phi)) + (dz) * (std::cos(theta));
        dtheta = ((dx) * (std::cos(theta) * std::cos(phi)) + (dy) * (std::cos(theta) * std::sin(phi)) - (dz) * (std::sin(theta))) / r;
        dphi = ((-std::sin(phi)) * (dx) + (std::cos(phi) * (dy))) / (r * (std::sin(theta)));

        double f = 1.0 - 1.0/r;

        double dt_dlambda = std::sqrt((dr*dr)/f + r*r*dtheta*dtheta + r*r*std::sin(theta)*std::sin(theta)*dphi*dphi) / f;
        E = f * dt_dlambda;
    }
    void syncCartesian()
    {
        x = r * std::sin(theta) * std::cos(phi);
        y = r * std::sin(theta) * std::sin(phi);
        z = r * std::cos(theta);
    }
};

inline void GeodesicRHS(GRRay& ray, double rhs[6], double r_s)
{
    double r = ray.r;
    double theta = ray.theta;
    double dr = ray.dr;
    double dtheta = ray.dtheta;
    double dphi = ray.dphi;
    double E = ray.E;


    double f = 1.0 - r_s / r;
    double dt_dlambda = E/f;

    // First derivatives, rate of change of positions (velocity):
    rhs[0] = dr;
    rhs[1] = dtheta;
    rhs[2] = dphi;

    // Radial Acceleration. Second derivatives, acceleration equations:   
    rhs[3] = -((r_s * f) / (2.0 * r*r)) * dt_dlambda*dt_dlambda
             +((r_s) / (2.0 * r*r * f)) * dr*dr + (r * f * dtheta*dtheta)
             + (r * f * std::sin(theta)*std::sin(theta) * dphi*dphi);
    // Latitude Acceleration:
    rhs[4] = -(2.0/r) * (dr) * (dtheta) + (std::sin(theta) * std::cos(theta) * dphi*dphi);
    // Longitude Acceleration:
    rhs[5] = -(2.0/r) * (dr) * (dphi) - ((2.0 * std::cos(theta)) / std::sin(theta)) * (dtheta) * (dphi);
}