#include <catch2/catch_test_macros.hpp>

#include "tuple.h"
#include "point.h"
#include "color.h"
#include "vector.h"
#include "tools.h"
#include "canvas.h"
#include "matrix.h"
#include "transformations.h"
#include "ray.h"
#include "shapes.h"
#include "materials.h"
#include "intersection.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <array>
#include <type_traits>

TEST_CASE("Creating a sphere","[sphere][shape]")
{
    Sphere s;
    Matrix I(4,4); 
    I.setIdentity(); 
    REQUIRE(s.getTransform() == I); 

    Shape* shapePtr = &s;
    REQUIRE(dynamic_cast<Sphere*>(shapePtr) != nullptr);
}

TEST_CASE("Sphere default transform","[sphere][shape]")
{
    Sphere s;
    Matrix m(4,4); 
    m.setIdentity(); 
    REQUIRE(s.getTransform() == m); 
}

TEST_CASE("Changing a sphere's transform","[sphere][shape]")
{
    Sphere s;
    Matrix t = translation(2,3,4); 
    s.setTransform(t); 
}

TEST_CASE("Intersecting a scaled sphere with a ray","[sphere][shape]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere s; 
    s.setTransform(scaling(2,2,2)); 
    std::vector<Intersection> xs = s.intersect(r); 

    REQUIRE(xs.size() == 2); 
    REQUIRE(xs[0].t == 3); 
    REQUIRE(xs[1].t == 7); 
}

TEST_CASE("Intersecting a translated sphere with a ray","[sphere][shape]")
{
    Ray r(Point(0,0,-5),Vector(0,0,1)); 
    Sphere s; 
    s.setTransform(translation(5,0,0)); 
    std::vector<Intersection> xs = s.intersect(r); 

    REQUIRE(xs.size() == 0); 
}

TEST_CASE("Computing sphere normal vectors","[sphere][shape]")
{
    Sphere s; 

    SECTION("Normal at point on x-axis")
    {
        Vector n = s.normal_at(Point(1.,0.,0.)); 
        REQUIRE(n == Vector(1.,0.,0.)); 
    }
    SECTION("Normal at point on y-axis")
    {
        Vector n = s.normal_at(Point(0.,1.,0.)); 
        REQUIRE(n == Vector(0.,1.,0.)); 
    }
    SECTION("Normal at point on z-axis")
    {
        Vector n = s.normal_at(Point(0.,0.,1.)); 
        REQUIRE(n == Vector(0.,0.,1.)); 
    }
    SECTION("Normal at non-axial point")
    {
        Vector n = s.normal_at(Point(sqrt(3)/3.f,sqrt(3)/3.f,sqrt(3)/3.f)); 
        REQUIRE(n == Vector(sqrt(3)/3.f,sqrt(3)/3.f,sqrt(3)/3.f)); 
    }
    SECTION("Normal is a normalized vector")
    {
        Vector n = s.normal_at(Point(sqrt(3)/3.f,sqrt(3)/3.f,sqrt(3)/3.f)); 
        REQUIRE(equal_double(n.magnitude(),1.f)); 
    }
}

TEST_CASE("Normal on transformed sphere","[sphere][shape]")
{
    Sphere s;
    SECTION("Normal on translated sphere")
    {
        s.setTransform(translation(0.,1.,0.)); 
        Vector n = s.normal_at(Point(0.f,1.70711,-0.70711)); 
        REQUIRE(n == Vector(0.f,0.70711,-0.70711)); 
    }
    SECTION("Normal on transformed sphere")
    {
        Matrix m = scaling(1,0.5,1) * rotation_z(M_PI/5.f); 
        s.setTransform(m); 
        Vector n = s.normal_at(Point(0.f,sqrt(2)/2.f,-sqrt(2)/2.f)); 
        REQUIRE(n == Vector(0.f,0.97014,-0.24254)); 
    }
}

TEST_CASE("Vector reflection about the normal","[sphere][shape]")
{
    SECTION("reflection of vector approaching at 45 deg")
    {
        Vector v(1.,-1,0); 
        Vector n(0,1,0); 
        Vector r = v.reflect_vector(n); 

        REQUIRE(r == Vector(1,1,0)); 
    }

    SECTION("Reflection off slanted surface")
    {
        Vector v(0.,-1,0); 
        Vector n(sqrt(2)/2.f,sqrt(2)/2.f,0); 

        Vector r = v.reflect_vector(n); 
        REQUIRE(r == Vector(1,0,0)); 
    }
}

TEST_CASE("Sphere material properties","[sphere][shape]")
{
    Sphere s; 
    SECTION("default material")
    {
        Material m = s.getMaterial(); 
        REQUIRE(equal_double(m.ambient,0.1)); 
        REQUIRE(equal_double(m.diffuse,0.9)); 
        REQUIRE(equal_double(m.specular,0.9)); 
        REQUIRE(equal_double(m.shininess,200.0)); 
        REQUIRE(m.mat_color == Color(1,1,1)); 
    }
    SECTION("material assignment")
    {
        Material m(0.1,0.8,0.8,190,Color(1,0,0)); 
        s.setMaterial(m); 

        Material s_m = s.getMaterial(); 
        REQUIRE(equal_double(s_m.ambient,0.1)); 
        REQUIRE(equal_double(s_m.diffuse,0.8)); 
        REQUIRE(equal_double(s_m.specular,0.8)); 
        REQUIRE(equal_double(s_m.shininess,190.0));
        REQUIRE(s_m.mat_color == Color(1,0,0));  
    }
}

TEST_CASE("The normal of a plane is constant everywhere","[plane][shape]")
{
    Plane p; 
    Vector n1 = p.local_normal_at(Point(0,0,0)); 
    Vector n2 = p.local_normal_at(Point(10,0,-10)); 
    Vector n3 = p.local_normal_at(Point(-5,0,150)); 

    REQUIRE(n1 == Vector(0,1,0)); 
    REQUIRE(n2 == Vector(0,1,0)); 
    REQUIRE(n3 == Vector(0,1,0)); 
}

TEST_CASE("Ray plane intersections","[plane][shape]")
{
    Plane p;
    SECTION("Intersect with a ray parallel to the plane")
    {
        Ray r(Point(0,10,0),Vector(0,0,1)); 
        std::vector<Intersection> xs = p.local_intersect(r); 

        REQUIRE(xs.size() == 0); 
    }

    SECTION("Intersect with a coplanar ray")
    {
        Ray r(Point(0,0,0),Vector(0,0,1)); 
        std::vector<Intersection> xs = p.local_intersect(r); 

        REQUIRE(xs.size() == 0); 
    }
    SECTION("A ray intersecting a plane from above")
    {
        Ray r(Point(0,1,0),Vector(0,-1,0)); 
        std::vector<Intersection> xs = p.local_intersect(r); 

        REQUIRE(xs.size() == 1); 
        REQUIRE(xs[0].t == 1); 
        REQUIRE(xs[0].s == &p); 
    }
        SECTION("A ray intersecting a plane from below")
    {
        Ray r(Point(0,-1,0),Vector(0,1,0)); 
        std::vector<Intersection> xs = p.local_intersect(r); 

        REQUIRE(xs.size() == 1); 
        REQUIRE(xs[0].t == 1); 
        REQUIRE(xs[0].s == &p); 
    }


}

TEST_CASE("A helper for producing a glass sphere","[shapes][sphere]")
{
    Sphere* s = glass_sphere(); 
    Matrix I(4,4); 
    I.setIdentity(); 
    REQUIRE(s->transform == I); 
    REQUIRE(equal_double(s->mat.transparency,1));  
    REQUIRE(equal_double(s->mat.refractive_index,1.5));  
}

TEST_CASE("A ray intersects a cube","[shapes][cube]")
{
    Cube c;
    std::vector<std::pair<double,double>> ts = {
    std::pair<double, double>{4, 6},
    std::pair<double, double>{4, 6},
    std::pair<double, double>{4, 6},
    std::pair<double, double>{4, 6},
    std::pair<double, double>{4, 6},
    std::pair<double, double>{4, 6},
    std::pair<double, double>{-1, 1},
                            }; 

    std::vector<std::pair<Point,Vector>> rays = {
    std::pair<Point, Vector>{Point(5,0.5,0), Vector(-1,0,0)},
    std::pair<Point, Vector>{Point(-5,0.5,0), Vector(1,0,0)},
    std::pair<Point, Vector>{Point(0.5,5,0), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(0.5,-5,0), Vector(0,1,0)},
    std::pair<Point, Vector>{Point(0.5,0,5), Vector(0,0,-1)},
    std::pair<Point, Vector>{Point(0.5,0,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0,0.5,0), Vector(0,0,1)},
    }; 

    for(int i = 0; i < ts.size(); i++)
    {
        Ray r(rays[i].first,rays[i].second); 
        std::vector<Intersection> xs = c.local_intersect(r); 
        REQUIRE(xs.size() == 2); 
        REQUIRE(xs[0].t == ts[i].first); 
        REQUIRE(xs[1].t == ts[i].second); 
    }
}

TEST_CASE("A ray misses a cube","[shapes][cube]")
{
    Cube c;
    std::vector<std::pair<Point,Vector>> rays = {
    std::pair<Point, Vector>{Point(-2,0,0), Vector(0.2673,0.5345,0.8018)},
    std::pair<Point, Vector>{Point(0,-2,0), Vector(0.8018,0.2673,0.5345)},
    std::pair<Point, Vector>{Point(0.,0,-2), Vector(0.5345,0.8018,0.2673)},
    std::pair<Point, Vector>{Point(2,0,2), Vector(0,0,-1)},
    std::pair<Point, Vector>{Point(0,2,2), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(2,2,0), Vector(-1,0,0)}
    }; 

    for(int i = 0; i < rays.size(); i++)
    {
        Ray r(rays[i].first,rays[i].second); 
        std::vector<Intersection> xs = c.local_intersect(r); 
        REQUIRE(xs.size() == 0); 
    }
}

TEST_CASE("The normal on the surface of a cube","[shapes][cube]")
{
    Cube c; 
    std::vector<std::pair<Point,Vector>> p_norms = {
    std::pair<Point, Vector>{Point(1,0.5,-0.8), Vector(1,0,0.0)},
    std::pair<Point, Vector>{Point(-1,-0.2,0.9), Vector(-1,0,0)},
    std::pair<Point, Vector>{Point(-0.4,1,-0.1), Vector(0,1,0)},
    std::pair<Point, Vector>{Point(0.3,-1,-0.7), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(-0.6,0.3,1), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0.4,0.4,-1), Vector(0,0,-1)},
    std::pair<Point, Vector>{Point(1,1,1), Vector(1,0,0)},
    std::pair<Point, Vector>{Point(-1,-1,-1), Vector(-1,0,0)}
    }; 

    for(int i = 0; i < p_norms.size();i++)
    {
        Vector normal = c.local_normal_at(p_norms[i].first); 
        REQUIRE(normal == p_norms[i].second); 
    }
}

TEST_CASE("A ray misses a cylinder","[shapes][cylinder]")
{
    Cylinder cyl; 
    std::vector<std::pair<Point,Vector>> pvs = {
    std::pair<Point, Vector>{Point(1,0,0), Vector(0,1,0)},
    std::pair<Point, Vector>{Point(0,0,0), Vector(0,1,0)},
    std::pair<Point, Vector>{Point(0,0,-5), Vector(1,1,1)}}; 


    for(int i = 0; i < pvs.size(); i++)
    {
        Vector direction = pvs[i].second.normalize(); 
        Ray r(pvs[i].first,direction); 
        std::vector<Intersection> xs = cyl.local_intersect(r); 
        REQUIRE(xs.size() == 0); 
    }
}

TEST_CASE("A ray strikes a cylinder","[shapes][cylinder]")
{
    Cylinder cyl; 
    std::vector<std::pair<Point,Vector>> pvs = {
    std::pair<Point, Vector>{Point(1,0,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0,0,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0.5,0,-5), Vector(0.1,1,1)}}; 

    std::vector<std::pair<double,double>> ts = {
    std::pair<double, double>{5, 5},
    std::pair<double, double>{4, 6},
    std::pair<double, double>{6.80798, 7.08872},
                            }; 

    for(int i = 0; i < pvs.size(); i++)
    {
        Vector direction = pvs[i].second.normalize(); 
        Ray r(pvs[i].first,direction); 
        std::vector<Intersection> xs = cyl.local_intersect(r); 
        REQUIRE(xs.size() == 2);
        REQUIRE(equal_double(xs[0].t,ts[i].first)); 
        REQUIRE(equal_double(xs[1].t,ts[i].second)); 
    }
}

TEST_CASE("Normal vector on a cylinder","[shapes][cylinder]")
{
    Cylinder cyl; 
    std::vector<std::pair<Point,Vector>> pvs = {
    std::pair<Point, Vector>{Point(1,0,0), Vector(1,0,0)},
    std::pair<Point, Vector>{Point(0,5,-1), Vector(0,0,-1)},
    std::pair<Point, Vector>{Point(0,-2,1), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(-1,1,0), Vector(-1,0,0)}
}; 

    for(int i = 0; i < pvs.size(); i++)
    {
        REQUIRE(cyl.local_normal_at(pvs[i].first) == pvs[i].second); 
    }
}

TEST_CASE("Intersecting a constrained cylinder","[shapes][cylinder]")
{
    Cylinder cyl(1,2); 
    std::vector<std::pair<Point,Vector>> pvs = {
    std::pair<Point, Vector>{Point(1,1.5,0), Vector(0.1,1,0)},
    std::pair<Point, Vector>{Point(0,3,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0,0,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0,2,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0,1,-5), Vector(0,0,1)},
    std::pair<Point, Vector>{Point(0,1.5,-2), Vector(0,0,1)}};
    
    REQUIRE(cyl.minimum == 1); 
    REQUIRE(cyl.maximum == 2); 

    std::vector<int> counts = {0,0,0,0,0,2}; 

    for(int i = 0; i < pvs.size(); i++)
    {
        Vector direction = pvs[i].second.normalize(); 
        Ray r(pvs[i].first,direction); 
        std::vector<Intersection> xs = cyl.local_intersect(r); 
        REQUIRE(xs.size() == counts[i]); 
    }
}

TEST_CASE("The default closed value for a cylinder","[shapes][cylinder]")
{
    Cylinder cyl; 
    REQUIRE(cyl.type == CYL_TYPE::OPEN); 
}

TEST_CASE("Intersecting a capped cylinder","[shapes][cylinder]")
{
    Cylinder cyl(1,2,CYL_TYPE::CLOSED); 
    std::vector<std::pair<Point,Vector>> pvs = {
    std::pair<Point, Vector>{Point(0,3,0), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(0,3,-2), Vector(0,-1,2)},
    std::pair<Point, Vector>{Point(0,4,-2), Vector(0,-1,1)},
    std::pair<Point, Vector>{Point(0,0,-2), Vector(0,1,2)},
    std::pair<Point, Vector>{Point(0,-1,-2), Vector(0,1,1)}};
    
    REQUIRE(cyl.minimum == 1); 
    REQUIRE(cyl.maximum == 2); 

    std::vector<int> counts = {2,2,2,2,2}; 

    for(int i = 0; i < pvs.size(); i++)
    {
        Vector direction = pvs[i].second.normalize(); 
        Ray r(pvs[i].first,direction); 
        std::vector<Intersection> xs = cyl.local_intersect(r); 
        REQUIRE(xs.size() == counts[i]); 
    }
}

TEST_CASE("The normal vector on a cylinders end caps","[shapes][cylinder]")
{
    Cylinder cyl(1,2,CYL_TYPE::CLOSED); 
    std::vector<std::pair<Point,Vector>> pvs = {
    std::pair<Point, Vector>{Point(0,1,0), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(0.5,1,0), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(0,1,0.5), Vector(0,-1,0)},
    std::pair<Point, Vector>{Point(0,2,0), Vector(0,1,0)},
    std::pair<Point, Vector>{Point(0.5,2,0), Vector(0,1,0)},
    std::pair<Point, Vector>{Point(0,2,0.5), Vector(0,1,0)}
};
    
    REQUIRE(cyl.minimum == 1); 
    REQUIRE(cyl.maximum == 2); 

    for(int i = 0; i < pvs.size(); i++)
    {
        Vector n = cyl.local_normal_at(pvs[i].first); 
        REQUIRE(n == pvs[i].second); 
    }
}

TEST_CASE("Creating a new group","[shapes][group]")
{
    Group g;
    Matrix I(4,4); 
    I.setIdentity();  
    REQUIRE(g.transform == I); 
    REQUIRE(g.children.empty()); 
}

TEST_CASE("A shape has a parent attribute","[shapes][group]")
{
    Sphere s; 
    REQUIRE(s.parent == nullptr); 
}

TEST_CASE("Adding a child to a group","[shapes][group]")
{
    Group g; 
    Sphere* s = new Sphere(); 
    g.add_child(s); 
    REQUIRE(!g.children.empty()); 
    REQUIRE(g.children[0] == s); 
    REQUIRE(s->parent == &g); 
}

TEST_CASE("Intersecting a ray with an empty group","[shapes][group]")
{
    Group g; 
    Ray r(Point(0,0,0),Vector(0,0,1)); 
    std::vector<Intersection> xs = g.local_intersect(r); 

    REQUIRE(xs.empty()); 
}

TEST_CASE("Intersecting a ray with a nonempty group","[shapes][group]")
{
    Group g; 
    Sphere* s1 = new Sphere(); 

    Sphere* s2 = new Sphere(); 
    s2->transform = translation(0,0,-3); 

    Sphere* s3 = new Sphere(); 
    s3->transform = translation(5,0,0); 

    g.add_child(s1); 
    g.add_child(s2); 
    g.add_child(s3); 

    Ray r(Point(0,0,-5),Vector(0,0,1)); 

    std::vector<Intersection> xs = g.local_intersect(r); 

    REQUIRE(xs.size() == 4);
    REQUIRE(xs[0].s == s2); 
    REQUIRE(xs[1].s == s2);
    REQUIRE(xs[2].s == s1);
    REQUIRE(xs[3].s == s1);
}

TEST_CASE("Intersecting a transformed group","[shapes][group]")
{
    Group g; 
    Sphere* s1 = new Sphere(); 

    g.transform = scaling(2,2,2); 
    s1->transform = translation(5,0,0); 

    g.add_child(s1); 

    Ray r(Point(10,0,-10),Vector(0,0,1)); 
    std::vector<Intersection> xs = g.intersect(r); 

    REQUIRE(xs.size() == 2); 
}

TEST_CASE("Converting a point from world to object space","[shapes][group]")
{
    Group* g1 = new Group(); 
    g1->transform = rotation_y(M_PI/2.0); 

    Group* g2 = new Group(); 
    g2->transform = scaling(2,2,2); 

    g1->add_child(g2); 
    Sphere* s = new Sphere(); 

    s->transform = translation(5,0,0); 
    g2->add_child(s); 

    Point p = world_to_object(s,Point(-2,0,-10)); 

    REQUIRE(p == Point(0,0,-1)); 

    delete g1; 
}

TEST_CASE("Converting a normal from object to world space","[group][shapes]")
{
    Group* g1 = new Group(); 
    g1->transform = rotation_y(M_PI/2.0);

    Group* g2 = new Group(); 
    g2->transform = scaling(1,2,3); 

    g1->add_child(g2);

    Sphere* s = new Sphere(); 
    s->transform = translation(5,0,0); 

    g2->add_child(s);

    Vector n = normal_to_world(s,Vector(sqrt(3)/3.0,sqrt(3)/3.0,sqrt(3)/3.0));
    REQUIRE(n == Vector(0.2857,0.4286,-0.8571));

}

TEST_CASE("Finding the normal on a child object","[group][shapes]")
{
    Group* g1 = new Group(); 
    g1->transform = rotation_y(M_PI/2.0);

    Group* g2 = new Group(); 
    g2->transform = scaling(1,2,3); 

    g1->add_child(g2);

    Sphere* s = new Sphere(); 
    s->transform = translation(5,0,0); 

    g2->add_child(s);

    Vector n = s->normal_at(Point(1.73321,1.1547,-5.5774)); 
    REQUIRE(n == Vector(0.2857,0.4286,-0.8571)); 
}



