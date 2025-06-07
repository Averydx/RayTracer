#include "point.h"
#include "vector.h"

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

}
