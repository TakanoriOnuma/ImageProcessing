#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "ToneCurve.h"
#include "Matrix.h"

class Image {
    int format;
    std::string comment;
    int height;
    int width;
    int max;
    unsigned char* arr;

    // --- private method --- //
    void readHeader(std::ifstream& fin);
    void writeHeader(std::ofstream& fout);
    std::string readLine(std::ifstream& fin);

public:
    // --- コンストラクタ --- //
    Image();
    Image(int height, int width, int fill = 0);
    Image(const char filename[]);
    Image(const Image& img);

    // initial method
    void init();

    // --- デストラクタ --- //
    ~Image();

    // --- ゲッター --- //
    int getHeight() const { return this->height; }
    int getWidth()  const { return this->width;  }
    int getMinBright() const;
    int getMaxBright() const;
    void getBright(int* min, int* max) const;
    double getContrast() const;

    unsigned char* operator[](int i) { return arr + i * width; }
    unsigned char* operator[](int i) const { return arr + i * width; }

    void load(const char filename[]);
    void save(const char filename[]);

    void printHeader() const;
    void inverse();
    void transCurve(ToneCurve& toneCurve);
    void alphaBlending(const Image* img);
    Image* makeHistgram() const;

    template <typename T>
    Image* filter(const Matrix<T>& matFil, int offset = 128);
    void filter(double (*func)(int i, int j, int height, int width));

    // --- static method --- //
    static Image* makeToneCurve(ToneCurve& toneCurve);
    static Image* emboss(const Image* img, int shift = 1);
};

template <typename T>
Image* Image::filter(const Matrix<T>& matFil, int offset) {
    int matHalfHeight = matFil.getHeight() / 2;
    int matHalfWidth  = matFil.getWidth() / 2;
    Image* img = new Image(this->height - 2 * matHalfHeight, this->width - 2 * matHalfWidth, 0);
    for(int i = matHalfHeight; i < this->height - matHalfHeight; i++) {
        for(int j = matHalfWidth; j < this->width - matHalfWidth; j++) {
            // 畳み込みを行う
            int value = offset;
            for(int m = 0; m < matFil.getHeight(); m++) {
                for(int n = 0; n < matFil.getWidth(); n++) {
                    int voffset = m - matHalfHeight;
                    int hoffset = n - matHalfWidth;
                    value += (*this)[i + voffset][j + hoffset] * matFil[m][n];
                }
            }
            value = (value < 0) ? 0 : (value <= 255) ? value : 255;
            (*img)[i - matHalfHeight][j - matHalfWidth] = (unsigned char)value;
        }
    }
    return img;
}
