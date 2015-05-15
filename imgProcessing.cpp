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
    while(x >= interval) {
        x -= interval;
        sign *= -1;
    }
    return sign;
}

double filter(int i, int j, int height, int width)
{
    return 30 * square(i, 2, width);
}

int main(int argc, char* argv[])
{
    Image* img = new Image(256, 256, 100);
    img->filter(filter);
    img->save("test.pgm");

    delete img;
    return 0;
}
