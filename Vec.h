#pragma once
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <cmath>

class Vec3{
    public:

    float x, y, z;

    // Constructor:
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
    Vec3() : x(0), y(0), z(0) {}

    // all Vec3 operators for adition
    Vec3 operator+(const Vec3 &v) const {return { x + v.x, y + v.y, z + v.z };}
    Vec3 operator-(const Vec3 &v) const {return { x - v.x, y - v.y, z - v.z };}
    Vec3 operator*(float t) const {return{ x*t, y*t, z*t};}
    Vec3 operator*(const Vec3 &v) const {return { x*v.x, y*v.y, z*v.z };}

    // Different formulas to be applied to rays:
    float dot(const Vec3 &v) const { return (x*v.x) + (y*v.y) + (z*v.z); }
    float length() const { return sqrt((x*x)+(y*y)+(z*z));}

    Vec3 cross(const Vec3 &v)const { return {(y*v.z - z*v.y), (z*v.x - x*v.z), (x*v.y - y*v.x)}; };
    Vec3 normalize() const {float l = length(); return { x/l, y/l, z/l};}

};
class ray{
    public:

    Vec3 Origin;
    Vec3 Direction;

    ray(const Vec3 &Origin, const Vec3 &Direction) : Origin(Origin), Direction(Direction.normalize()) {}

    Vec3 at(float t) const {return {Origin + Direction *t};}

    float sphereint(float r, const Vec3 &center){

        Vec3 oc = Origin - center;
        float b = oc.dot(Direction);      
        float c = oc.dot(oc) - r * r;
        float disc = b * b - c;
        if (disc < 0) return -1.0f;
        return -b - sqrt(disc);
                   
    }
};
