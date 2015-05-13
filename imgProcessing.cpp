#include <iostream>
#include <string>
#include <stdio.h>
#include <typeinfo>
#include <math.h>

#include "ToneCurve.h"
#include "Image.h"

using namespace std;

double square(int x, int f, int T)
{
    int sign = 1;
    double interval = (double)T / (2 * f);
    while(x > interval) {
        x -= interval;
        sign *= -1;
    }
    return sign;
}

double filter(int i, int j, int height, int width)
{
    return 30 * square(i, 8, height) +
        30 * square(j, 8, width);
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "Usage: filename" << endl;
        return -1;
    }

    string filename = argv[1];
    Image* img = new Image(filename.c_str());
    Image* imgHist = img->makeHistgram();
    filename.erase(filename.size() - 4);
    filename += "_hist.pgm";
    imgHist->save(filename.c_str());

    delete img;
    delete imgHist;
    return 0;
}
