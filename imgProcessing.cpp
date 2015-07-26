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
    Image* img = new Image("zui.pgm");
    int rhoHeight = (int)sqrt(img->getHeight() * img->getHeight() + img->getWidth() * img->getWidth());
    cout << "rhoHeight:" << rhoHeight << endl;
    vector< vector<int> > voteMat(2 * rhoHeight, vector<int>((int)(100 * M_PI), 0));

    for(int i = 0; i < img->getWidth(); i++) {
        for(int j = 0; j < img->getWidth(); j++) {
            // 点であればそれに対応した線を引く
            if((*img)[i][j] < 127) {
                for(int theta = 0; theta < voteMat[0].size(); theta++) {
                    double rad = theta * M_PI / voteMat[0].size();
                    int rho = (int)(i * sin(rad) + j * cos(rad)) + rhoHeight;
                    voteMat[rho][theta] += 1;
                }
            }
        }
    }

    // 代表点を見つける
    vector<int> maxVotes(3, 0);
    for(int rho = 0; rho < voteMat.size(); rho++) {
        for(int theta = 0; theta < voteMat[rho].size(); theta++) {
            for(int i = 0; i < maxVotes.size(); i++) {
                if(voteMat[rho][theta] > maxVotes[i]) {
                    for(int j = maxVotes.size() - 1; j > i; j--) {
                        maxVotes[j] = maxVotes[j - 1];
                    }
                    maxVotes[i] = voteMat[rho][theta];
                    break;
                }
            }
        }
    }
    for(int i = 0; i < maxVotes.size(); i++) {
        for(int rho = 0; rho < voteMat.size(); rho++) {
            for(int theta = 0; theta < voteMat[rho].size(); theta++) {
                if(voteMat[rho][theta] == maxVotes[i]) {
                    cout << "(" << rho << ", " << theta << "):" << voteMat[rho][theta] << endl;
                }
            }
        }
    }

    // ハフ変換の画像を作成する
    Image* houghImg = new Image(2 * rhoHeight, (int)(100 * M_PI));
    for(int i = 0; i < houghImg->getHeight(); i++) {
        for(int j = 0; j < houghImg->getWidth(); j++) {
            (*houghImg)[i][j] = (unsigned char)(255 * voteMat[i][j] / maxVotes[0]);
        }
    }
    houghImg->save("houghImg.pgm");
    delete img;
    delete houghImg;

    return 0;
}
