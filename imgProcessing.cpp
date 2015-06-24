#include <iostream>
#include <string>
#include <stdio.h>
#include <typeinfo>
#include <math.h>
#include <complex>

#include "ToneCurve.h"
#include "Image.h"
#include "Matrix.h"
#include "fft.h"

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
    return 50 * square(i, 16, height);
}

int main(int argc, char* argv[])
{
    Image* img = new Image(256, 256);
    for(int i = 0; i < img->getHeight(); i++) {
        for(int j = 0; j < img->getWidth(); j++) {
            (*img)[i][j] = 127 * sin(j * M_PI / 4) + 127;
        }
    }
    img->save("test.pgm");

    Image* fftImg = fft(img);
    fftImg->save("fftImg.pgm");

    delete img;
    if(fftImg != NULL) {
        delete fftImg;
    }

    return 0;
}
