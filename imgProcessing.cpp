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
    Image* img = new Image("C.pgm");

    vector< vector< complex<double> > > vec = fft(img);
    swap(vec);
    Image* fftImg = new Image(vec, true);
    fftImg->save("fftImg.pgm");

    swap(vec);
    vector< vector< complex<double> > > ifftVec = ifft(vec);
    Image* ifftImg = new Image(ifftVec);
    ifftImg->save("ifftImg.pgm");

    delete img;
    if(fftImg != NULL) {
        delete fftImg;
    }
    delete ifftImg;

    return 0;
}
