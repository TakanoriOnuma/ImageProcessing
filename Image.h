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
    char* arr;

public:
    // --- コンストラクタ --- //
    Image();
    Image(const char filename[]);
    Image(const Image& img);

    // initial method
    void init();

    // --- デストラクタ --- //
    ~Image();

    // --- ゲッター --- //
    int getMinBright() const;
    int getMaxBright() const;
    void getBright(int* min, int* max) const;
    double getContrast() const;

    void load(const char filename[]);
    void save(const char filename[]);

    void readHeader(std::ifstream& fin);
    void writeHeader(std::ofstream& fout);
    void printHeader() const;
    std::string readLine(std::ifstream& fin);
    void inverse();
    void transCurve(ToneCurve& toneCurve);
    void alphaBlending(const Image* img);
    Image* makeHistgram() const;

    // --- static method --- //
    static Image* makeToneCurve(ToneCurve& toneCurve);
    static Image* emboss(const Image* img, int shift = 1);
};
