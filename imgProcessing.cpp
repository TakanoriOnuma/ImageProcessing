#include <iostream>
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
    return 50 * square(i, 16, height);
}

int main(int argc, char* argv[])
{
    Matrix<double> matFil(3, 3);
    matFil[0][0] = 0;   matFil[0][1] = 1;   matFil[0][2] = 0;
    matFil[1][0] = 1;   matFil[1][1] = -4;  matFil[1][2] = 1;
    matFil[2][0] = 0;   matFil[2][1] = 1;   matFil[2][2] = 0;
    Image* img = new Image("H.pgm");
    Image* copyImg = img->filter(matFil, 100);
    copyImg->save("test.pgm");

    delete img;
    delete copyImg;
    return 0;
}
