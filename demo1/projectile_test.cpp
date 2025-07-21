#include "point.h"
#include "vector.h"
#include <iostream>
#include "canvas.h"
#include <math.h>

class Projectile
{
    public: 
        Point loc; 
        Vector velocity; 

        Projectile(Point init_loc, Vector init_velocity)
        {
            loc = init_loc; 
            velocity = init_velocity; 
        }
}; 

class Environment
{
    public: 
        Vector wind; 
        Vector gravity; 

        Environment(Vector init_wind, Vector init_gravity)
        {
            wind = init_wind; 
            gravity = init_gravity; 
        }


}; 

void tick(Projectile& proj, const Environment& env)
{
    proj.loc = proj.loc + proj.velocity; 
    proj.velocity = proj.velocity + env.wind + env.gravity; 
}

int main(int argc, char *argv[])
{
    Vector v1 = Vector(1,1.8,0); 
    v1.normalize(); 
    v1 = v1 * 11.25; 
    Point p1 = Point(0.,1.,0.); 
    Projectile proj(p1,v1); 
    Environment env(Vector(0.,-0.1,0.),Vector(-0.01,0.,0.)); 

    int width = 900; 
    int height = 550; 
    Canvas c = Canvas(width,height); 
    while(proj.loc.y >= 0)
    {
        tick(proj,env); 

        int x = round(std::max(proj.loc.x,0.)); 
        int y = round(std::max(height - proj.loc.y,0.));

        if((y > 0 && y < height)&&(x > 0 && x < width))
            c.SetPixel(round(std::max(proj.loc.x,0.)),round(std::max(height - proj.loc.y,0.)), Color(1,0.,0.)); 
    }

    c.CanvasToPPM("demo1.ppm"); 
}
