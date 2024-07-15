#pragma once
#include <iostream>
#include <vector>

struct Color {
    float r, g, b;
    Color();
    Color(int r, int g, int b);
};

struct Image {
    uint16_t img_width;
    uint16_t img_height;
    std::vector<Color> img_colors;
    Image(int width, int height);
    Color GetColor(int x, int y);
    void SetColor(const Color& color, int x, int y);
    void Export(const std::string& path);
};
