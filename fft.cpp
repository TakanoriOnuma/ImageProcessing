#include <vector>
#include "fft.h"

using namespace std;

int reverse(unsigned int x, int bitNum)
{
    unsigned int rev = 0;
    for(int i = 0; i < bitNum; i++) {
        rev = (rev << 1) | (x & 1);
        x = x >> 1;
    }
    return rev;
}

int calcPower(int x, int base)
{
    int index = 0;
    while(x != 1) {
        x /= base;
        ++index;
    }
    return index;
}

vector< complex<double> > fft(const vector< complex<double> >& vec)
{
    vector< complex<double> > w(vec.size());
    vector< complex<double> > fftVec(vec);

    // Wテーブルの作成
    for(int i = 0; i < w.size(); i++) {
        double theta = -2 * M_PI * i / w.size();
        w[i] = complex<double>(cos(theta), sin(theta));
    }

    // fftを行う
    int b = 1;
    for(int step = vec.size() / 2; step >= 1; step /= 2, b++) {
        int idx = 0;
        while(idx < vec.size()) {
            for(int n = 0; n < step; n++, idx++) {
                complex<double> buf = fftVec[idx] + fftVec[idx + step];
                fftVec[idx + step] = w[b * n] * (fftVec[idx] - fftVec[idx + step]);
                fftVec[idx] = buf;
            }
            idx += step;
        }
    }

    // ビットリバース
    int bitNum = calcPower(fftVec.size(), 2);
    for(int i = 0; i < fftVec.size(); i++) {
        unsigned int rev = reverse((unsigned int)i, bitNum);
        if(rev < fftVec.size() / 2) {
            swap(fftVec[i], fftVec[rev]);
        }
    }

    return fftVec;
}

Image* fft(const Image* img)
{
    return NULL;
}
