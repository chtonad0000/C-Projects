#include "bitmapfile.h"
#include <iostream>
#include <fstream>

Color::Color()
        : r(0), g(0), b(0) {
}

Color::Color(int r, int g, int b)
        : r(r), g(g), b(b) {
}

Image::Image(int width, int height)
        : img_width(width), img_height(height), img_colors(std::vector<Color>(width * height)) {
}

Color Image::GetColor(int x, int y) {
    return img_colors[x + y * img_width];
}

void Image::SetColor(const Color& color, int x, int y) {
    img_colors[x + y * img_width].r = color.r;
    img_colors[x + y * img_width].g = color.g;
    img_colors[x + y * img_width].b = color.b;
}

void Image::Export(const std::string& path) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    unsigned char Pad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (img_width * 3) % 4) % 4);
    const int kHeaderSize = 14;
    const int kInformHeaderSize = 40;
    const int kFileSize = kHeaderSize + kInformHeaderSize + img_width * img_height * 3 + paddingAmount * img_width;
    unsigned char Header[kHeaderSize];
    Header[0] = 'B';
    Header[1] = 'M';
    Header[2] = kFileSize;
    Header[3] = kFileSize >> 8;
    Header[4] = kFileSize >> 16;
    Header[5] = kFileSize >> 24;
    Header[6] = 0;
    Header[7] = 0;
    Header[8] = 0;
    Header[9] = 0;
    Header[10] = kHeaderSize + kInformHeaderSize;
    Header[11] = 0;
    Header[12] = 0;
    Header[13] = 0;
    unsigned char InformationHeader[kInformHeaderSize];
    for (int i = 1; i < kInformHeaderSize - 1; i++) {
        InformationHeader[i] = 0;
    }
    InformationHeader[0] = kInformHeaderSize;
    InformationHeader[4] = img_width;
    InformationHeader[5] = img_width >> 8;
    InformationHeader[6] = img_width >> 16;
    InformationHeader[7] = img_width >> 24;
    InformationHeader[8] = img_height;
    InformationHeader[9] = img_height >> 8;
    InformationHeader[10] = img_height >> 16;
    InformationHeader[11] = img_height >> 24;
    InformationHeader[12] = 1;
    InformationHeader[14] = 24;
    file.write(reinterpret_cast<char *>(Header), kHeaderSize);
    file.write(reinterpret_cast<char *>(InformationHeader), kInformHeaderSize);
    for (int y = 0; y < img_height; y++) {
        for (int x = 0; x < img_width; x++) {
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);
            unsigned char color[] = {b, g, r};
            file.write(reinterpret_cast<char *>(color), 3);
        }
        file.write(reinterpret_cast<char *>(Pad), paddingAmount);
    }
    file.close();
}