#include <iostream>
#include <stdio.h>
#include <typeinfo>

#include "ToneCurve.h"
#include "Image.h"

using namespace std;

double filter(int i, int j, int height, int width)
{
    return (double)255 * j / width;
}

int main()
{
    Image* img = new Image(256, 256, 0);
    img->filter(filter);
    img->save("left_lenear.pgm");

    delete img;
    return 0;
}
