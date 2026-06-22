#include "Vec.h"
#include "BlackHolePhysics.h"
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>

void setPixel(float x, float y, float r, float g, float b);

void renderRows(int startY, int endY,Vec3 CameraPos, Vec3 forward, Vec3 right, Vec3 up, float aspect, int WIDTH, int HEIGHT);

void render(Vec3 &CameraPos, double &yaw, double &pitch,int WIDTH, int HEIGHT)
{
    // Making origin and point based off camera angle:
    Vec3 forward(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch));

    Vec3 right = forward.cross(Vec3(0,1,0)).normalize();

    Vec3 up = right.cross(forward).normalize();

    float aspect = (float)WIDTH/HEIGHT;

    int numThreads = std::thread::hardware_concurrency();
    int rowsPerThread = HEIGHT/numThreads;

    std::vector<std::thread> threads;

    for(int i = 0; i < numThreads; i++)
    {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? HEIGHT : startY + rowsPerThread;

        threads.emplace_back(renderRows, startY, endY, CameraPos, forward, right, up, aspect, WIDTH, HEIGHT);
    }
    for (auto& t : threads)
    t.join();
}

void renderRows(int startY, int endY,Vec3 CameraPos, Vec3 forward, Vec3 right, Vec3 up, float aspect, int WIDTH, int HEIGHT)
{
    // Continue later
}