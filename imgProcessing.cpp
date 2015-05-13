#include <iostream>

#include "ToneCurve.h"
#include "Image.h"

using namespace std;

int main()
{
    Image* img = new Image(256, 256, 100);
    img->save("fill100.pgm");

    delete img;
    return 0;
}
