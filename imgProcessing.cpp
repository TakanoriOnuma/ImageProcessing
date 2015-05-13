#include <iostream>

#include "ToneCurve.h"
#include "Image.h"

using namespace std;

int main()
{
    Image* img = new Image("images/D.pgm");
    Image* img2 = new Image("images/C.pgm");

    img->alphaBlending(img2);
    img->save("D_C_alphablend.pgm");
    Image* imgAlphaBlendHist = img->makeHistgram();
    imgAlphaBlendHist->save("D_C_alphablend_hist.pgm");

    delete imgAlphaBlendHist;
    delete img;
    delete img2;
    return 0;
}
