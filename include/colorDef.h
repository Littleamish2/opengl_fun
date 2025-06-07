// Helpful functions for working with the 
// normalized RGB values in OpenGL

#include <array>


// Converts RGB values into OpenGL RGB normalized values (0-1)
float normalizeRGB(float rgb) {
    return rgb / (float)(256);
}

// Generator that creates an RGB color array
std::array<float, 3> createColor(float r, float g, float b) {
    std::array<float, 3> arr = {
        normalizeRGB(r),
        normalizeRGB(g),
        normalizeRGB(b)
    };

    return arr;
}

// Colors

std::array<float, 3> 
    pink = createColor(255, 192, 203);

std::array<float, 3> 
    white = createColor(255, 255, 255);

std::array<float, 3> 
    black = createColor(0, 0, 0);