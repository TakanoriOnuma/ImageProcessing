#pragma once

#include <fstream>
#include <string>

#include "ToneCurve.h"

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

    void load(const char filename[]);
    void save(const char filename[]);

    void printHeader() const;
    void inverse();
    void transCurve(ToneCurve& toneCurve);
    void alphaBlending(const Image* img);
    Image* makeHistgram() const;

    void filter(double (*func)(int i, int j, int height, int width));

    // --- static method --- //
    static Image* makeToneCurve(ToneCurve& toneCurve);
    static Image* emboss(const Image* img, int shift = 1);
};
