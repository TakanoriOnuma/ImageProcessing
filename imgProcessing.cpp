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
    vector< complex<double> >vec(8);
    for(int i = 0; i <= vec.size() / 2; i++) {
        vec[i] = i * M_PI / 4;
    }
    for(int i = vec.size() / 2 + 1; i < vec.size(); i++) {
        vec[i] = vec[vec.size() - i];
    }

    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << endl;
    }

    cout << "--------------------" << endl;
    vector< complex<double> > fftVec = fft(vec);
    for(int i = 0; i < fftVec.size(); i++) {
        cout << fftVec[i] << endl;
    }

    return 0;
}
