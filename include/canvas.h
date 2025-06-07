#include <vector>
#include "color.h"
#include <iostream>
#include <fstream>
#include <string> 

class Canvas
{
    public: 

        //fields
        int width; 
        int height; 
        std::vector<Color> pixel_map;   

        //Constructor
        Canvas(int width,int height);

        //Getter and Setter for the pixel map
        Color GetPixel(int row, int col);  
        void SetPixel(int row, int col, Color newColor); 

        //Set all pixels at once
        void Canvas::SetAllPixels(Color newColor); 

        //Convert Canvas to PPM
        void CanvasToPPM(std::string fileName); 

    
}; 