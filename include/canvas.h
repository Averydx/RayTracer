#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include "color.h"
#include <iostream>
#include <fstream>
#include <string> 

// This file defines the Canvas class for representing a 2D pixel grid
// It includes methods for setting pixel colors, converting to PPM format, and saving to a file
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

#endif