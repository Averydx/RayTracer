#include "canvas.h"


Canvas::Canvas(int width,int height)
{
    this->width = width; 
    this->height = height; 

    this->pixel_map = std::vector<Color>(); 

    for(int i = 0; i < width * height;i++)
    {
        this->pixel_map.push_back(Color(0.,0.,0.)); 
    }
}

Color Canvas::GetPixel(int x, int y)
{
    return pixel_map.at(y * this->width + x); 
}

void Canvas::SetPixel(int x, int y, Color newColor)
{
    pixel_map.at(y * this->width + x) = newColor; 
}

void Canvas::SetAllPixels(Color newColor)
{
    for(int i = 0; i < pixel_map.size();i++)
    {
        pixel_map[i] = newColor; 
    }
}

void Canvas::CanvasToPPM(std::string fileName)
{
    int maxLineLength = 70; 
    std::ofstream myfile;
    myfile.open(fileName);
    myfile << "P3\n";
    myfile << this->width << " " << this->height<<"\n"; 
    myfile<<255<<"\n"; 

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Color pixelColor = GetPixel(j,i); 
            pixelColor =  pixelColor * 255; 
            pixelColor.clamp(0,255); 
            myfile<<pixelColor.x <<" "<<pixelColor.y <<" "<<pixelColor.z<<" "; 
            
        }
        myfile<<"\n"; 
    }

    myfile.close();
}