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
    cout << "test" << endl;
    vector< complex<double> > testVec(256);
    testVec[100] = 255;
    for(int i = 0; i < testVec.size(); i++) {
        cout << testVec[i] << ", ";
    }
    cout << '\n';
    testVec = fft(testVec);
    for(int i = 0; i < testVec.size(); i++) {
        cout << testVec[i] << ", ";
    }
    cout << '\n';
    testVec = ifft(testVec);
    for(int i = 0; i < testVec.size(); i++) {
        cout << testVec[i] << ", ";
    }
    cout << '\n';
    return 0;


    Image* img = new Image(16, 16);
    for(int i = 0; i < img->getHeight(); i++) {
        for(int j = 0; j < img->getWidth(); j++) {
            (*img)[i][j] = 127 * sin(j * M_PI / 4) + 127;
        }
    }
    img->save("test.pgm");

    vector< vector< complex<double> > > vec = fft(img);
    Image* fftImg = new Image(vec);
    fftImg->save("fftImg.pgm");

    /*
    vector< vector< complex<double> > > ifftVec = ifft(vec);
    for(int i = 0; i < ifftVec.size(); i++) {
        for(int j = 0; j < ifftVec[i].size(); j++) {
            cout << ifftVec[i][j] << ", ";
        }
        cout << '\n';
    }
    Image* ifftImg = new Image(ifftVec);
    ifftImg->save("ifftImg.pgm");
    */


    delete img;
    if(fftImg != NULL) {
        delete fftImg;
    }
    // delete ifftImg;

    return 0;
}
