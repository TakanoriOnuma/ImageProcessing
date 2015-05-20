#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <typeinfo>
#include <math.h>

#include "ToneCurve.h"
#include "Image.h"
#include "Matrix.h"

using namespace std;

double square(int x, int f, int T)
{
    int sign = 1;
    double interval = (double)T / (2 * f);
    while(x >= interval) {
        x -= interval;
        sign *= -1;
    }
    return sign;
}

double filter(int i, int j, int height, int width)
{
    return 30 * sin(4 * 2 * M_PI * j / width) +
        30 * sin(32 * 2 * M_PI * i / height);
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
        cout << "Usage: imgFileName filterFileName" << endl;
        return -1;
    }
    string imgFileName = argv[1];
    string filterFileName = argv[2];

    ifstream ifs(filterFileName.c_str());
    int height, width;
    ifs >> height >> width;

    Matrix<double> matFil(height, width);
    for(int i = 0; i < matFil.getHeight(); i++) {
        for(int j = 0; j < matFil.getWidth(); j++) {
            ifs >> matFil[i][j];
        }
    }

    Image* img = new Image(imgFileName.c_str());
    Image* imgFil = img->filter(matFil);

    imgFileName.erase(imgFileName.size() - 4);
    imgFileName += "_filter.pgm";

    imgFil->save(imgFileName.c_str());

    delete img;
    delete imgFil;

    return 0;
}
