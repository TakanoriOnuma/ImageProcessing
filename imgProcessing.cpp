#include <iostream>
#include <stdio.h>
#include <typeinfo>

#include "ToneCurve.h"
#include "Image.h"

using namespace std;

int main()
{
    Image* img = new Image(256, 256, 0);

    for(int i = img->getHeight() / 4; i < img->getHeight() * 3 / 4; i++) {
        for(int j = img->getWidth() / 4; j < img->getWidth() * 3 / 4; j++) {
            (*img)[i][j] = (unsigned char)200;
        }
    }
    img->save("quarter_rect.pgm");

    delete img;
    return 0;
}
