#include "lights.h"
#include "vector.h"
#include <iostream> 

pointLight::pointLight(Color light_inten, Point light_pos)
{
    this->intensity = light_inten; 
    this->position = light_pos; 
}

pointLight::pointLight()
{
    this->intensity = Color(1.,1,1); 
    this->position = Point(0,0,0); 
}

Color lighting(const Material& mat, const Shape* shape, const pointLight& light, const Point& point, const Vector& eye_vec, const Vector& normal_vec,bool in_shadow)
{
    Color color = mat.mat_color; 
    if(mat.pattern != nullptr)
        color = mat.pattern->color_at_object(shape, point); 

    if(in_shadow)
        return color * mat.ambient; 

    //combine surface color with the light's color/intensity
    Color effective_color = color * light.intensity; 

    //find the direction to the light source
    Vector light_vec = light.position - point;
    light_vec = light_vec.normalize();

    //compute the ambient contribution
    Color ambient = effective_color * mat.ambient; 

    /*light_dot_normal represents the cosine of the angle between the
    light vector and the normal vector. A negative number means the 
    light is on the other side of the surface. 
    */ 

    double light_dot_normal = light_vec * normal_vec; 

    Color diffuse(0.0,0.0,0.0); 
    Color specular(0.0,0.0,0.0); 
    if(light_dot_normal > 0)
    {
        //Compute the diffuse contribution
        diffuse = effective_color * mat.diffuse * light_dot_normal; 
    }

    /*reflect_dot_eye represents the cosine of the angle between the
    reflection vector and the eye vector. A negative number means the 
    light reflects away from the eye. 
    */

    Vector neg_light_vec = -light_vec; 
    Vector reflectv = neg_light_vec.reflect_vector(normal_vec); 
    double reflect_dot_eye = reflectv * eye_vec; 

    if(reflect_dot_eye > 0)
    {
        //Compute the specular contribution
        double factor = pow(reflect_dot_eye,mat.shininess); 
        specular = light.intensity * mat.specular * factor; 
    }

    //Compute the attenuation factor 
    double d = light_vec.magnitude(); 
    double k_c = 1; 
    double k_i = 0.22; 
    double k_q = 0.2; 
    double F_att = 1/(k_c + k_i*d + k_q * d * d); 

    return (ambient + diffuse + specular) * F_att; 

}