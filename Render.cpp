#include "Vec.h"
#include "BlackHolePhysics.h"
#include "stars.h"
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>

void setPixel(float x, float y, float r, float g, float b);

// Making space scene behind
// Psuedo-random 0 to 1, from two values:
static float hash21(float x, float y)
{
    float h = std::sin(x*127.1f + y*311.7f) * 43758.5453f;
    return h - std::floor(h);
}

// Looking at someone elses code for the sky (Project was for GR not interested in generating space)
void SpaceScene(float dx, float dy, float dz, float &r, float &g, float &b)
{
    // Direction of sky coords
    float u = std::atan2(dz,dx);
    float v = std::acos(dy);

    const float N = 400.0f;
    float cu = std::floor(u * N);
    float cv = std::floor(v*N);

    float n = hash21(cu, cv);

    if (n > 0.997f)
    {
        float bright = 0.5f + 0.5f * hash21(cv, cu);
        r = g = b = bright;
        float tint = hash21(cu + 1.0f, cv + 1.0f);
        if (tint < 0.33f) {r *= 0.8f; g *= 0.9f; }
        else if ( tint > 0.66f) {b *= 0.8f; g*= 0.9f;}
    }
    else
    {
        r = g = b = 0.02f;
    }
}

void renderRows(int startY, int endY,Vec3 CameraPos, Vec3 forward, Vec3 right, Vec3 up, float aspect, int WIDTH, int HEIGHT, double r_s);

void render(Vec3 &CameraPos, double &yaw, double &pitch,int WIDTH, int HEIGHT, double r_s)
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

        threads.emplace_back(renderRows, startY, endY, CameraPos, forward, right, up, aspect, WIDTH, HEIGHT, r_s);
    }
    for (auto& t : threads)
    t.join();
}

void renderRows(int startY, int endY,Vec3 CameraPos, Vec3 forward, Vec3 right, Vec3 up, float aspect, int WIDTH, int HEIGHT, double r_s)
{
        for (int y_axis = startY; y_axis < endY; y_axis++)
    {
        for (int x_axis = 0; x_axis < WIDTH; x_axis++)
        {
            
            // ndc = Normalized Device Coordinates (Center of the screen)
            float ndcX = (2.0f * x_axis / WIDTH - 1.0f) * aspect;

            float ndcY = 2.0f * y_axis / HEIGHT - 1.0f;

            // Direction of the Ray from Camera and the actual Ray:

            Vec3 dir = (forward + right * ndcX + up * ndcY).normalize();

            GRRay New_Ray(CameraPos, dir);
            GRRay temp_ray = New_Ray;
            bool showDisk = true;
            double InnerDisk = 3.0;
            double OuterDisk = 10.0;
            double EscapeR = 100.0;
            double stepsize = 1.0 * (New_Ray.r/EscapeR);
            double maxSteps = 2750;

            bool painted = false;
            double k1[6], k2[6], k3[6], k4[6];
            New_Ray.syncCartesian();
            for(int step = 0; step < maxSteps; step++)
            {
                double prevX = New_Ray.x;
                double prevY = New_Ray.y;
                double prevZ = New_Ray.z;

                // First step:
                GeodesicRHS(New_Ray,k1,r_s);

                // Second Step:
                temp_ray = New_Ray;
                temp_ray.r +=      0.5 * stepsize * k1[0];
                temp_ray.theta +=  0.5 * stepsize * k1[1];
                temp_ray.phi +=    0.5 * stepsize * k1[2];
                temp_ray.dr +=     0.5 * stepsize * k1[3];
                temp_ray.dtheta += 0.5 * stepsize * k1[4];
                temp_ray.dphi +=   0.5 * stepsize * k1[5];
                GeodesicRHS(temp_ray,k2,r_s);

                // Third Step:
                temp_ray = New_Ray;
                temp_ray.r +=      0.5 * stepsize * k2[0];
                temp_ray.theta +=  0.5 * stepsize * k2[1];
                temp_ray.phi +=    0.5 * stepsize * k2[2];
                temp_ray.dr +=     0.5 * stepsize * k2[3];
                temp_ray.dtheta += 0.5 * stepsize * k2[4];
                temp_ray.dphi +=   0.5 * stepsize * k2[5];
                GeodesicRHS(temp_ray,k3,r_s);

                // Fourth Step:
                temp_ray = New_Ray;
                temp_ray.r +=      stepsize * k3[0];
                temp_ray.theta +=  stepsize * k3[1];
                temp_ray.phi +=    stepsize * k3[2];
                temp_ray.dr +=     stepsize * k3[3];
                temp_ray.dtheta += stepsize * k3[4];
                temp_ray.dphi +=   stepsize * k3[5];
                GeodesicRHS(temp_ray,k4,r_s);

                New_Ray.r      += (stepsize/6.0) * (k1[0] + 2*k2[0] + 2*k3[0] + k4[0]);
                New_Ray.theta  += (stepsize/6.0) * (k1[1] + 2*k2[1] + 2*k3[1] + k4[1]);
                New_Ray.phi    += (stepsize/6.0) * (k1[2] + 2*k2[2] + 2*k3[2] + k4[2]);
                New_Ray.dr     += (stepsize/6.0) * (k1[3] + 2*k2[3] + 2*k3[3] + k4[3]);
                New_Ray.dtheta += (stepsize/6.0) * (k1[4] + 2*k2[4] + 2*k3[4] + k4[4]);
                New_Ray.dphi   += (stepsize/6.0) * (k1[5] + 2*k2[5] + 2*k3[5] + k4[5]);

                if(New_Ray.theta < 1e-4) New_Ray.theta = 1e-4;
                if(New_Ray.theta > M_PI - 1e-4) New_Ray.theta = M_PI - 1e-4;

                New_Ray.syncCartesian();
                if(New_Ray.r <= r_s * 1.05)
                {
                    setPixel(x_axis, y_axis,0.0,0.0,0.0);
                    painted = true;
                    break;
                }
                if(showDisk && prevY * New_Ray.y < 0.0)
                {
                    double R = sqrt(New_Ray.x*New_Ray.x + New_Ray.z*New_Ray.z);
                    if(R >= InnerDisk && R <= OuterDisk)
                    {   
                        float t = (float)(R-InnerDisk)/(0.5 *(OuterDisk - InnerDisk));

                        float gg = t*0.5f;
                        float bb = 0.0f;

                        setPixel(x_axis, y_axis, 1.0, gg, bb);
                        painted = true;
                        break;
                    }
                }
                float starR, starG, starB;
                if (hitStar(New_Ray.x, New_Ray.y, New_Ray.z, starR, starG, starB))
                {
                    setPixel(x_axis,y_axis,starR,starG,starB);
                    painted = true;
                    break;
                }
                if(New_Ray.r > EscapeR) 
                {
                    double ddx = New_Ray.x - prevX;
                    double ddy = New_Ray.y - prevY;
                    double ddz = New_Ray.z - prevZ;
                    double len = std::sqrt(ddx*ddx + ddy*ddy + ddz*ddz); 
                    ddx /= len; ddy /= len; ddz/= len;

                    float sr, sg, sb;
                    SpaceScene((float)ddx, (float)ddy, (float)ddz, sr, sg, sb);
                    setPixel(x_axis, y_axis, sr,sg,sb);  
                    painted = true;
                    break; 
                }
            }
            if (!painted) 
            { 
                setPixel(x_axis, y_axis, 0.0f,0.0f,0.0f);
            }
        }
    }
}