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
    for(int step = vec.size() / 2; step >= 1; step /= 2, b *= 2) {
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
        if(i < rev) {
            swap(fftVec[i], fftVec[rev]);
        }
    }

    return fftVec;
}

vector< complex<double> > ifft(const vector< complex<double> >& vec)
{
    vector< complex<double> > ifftVec(vec);

    for(int i = 0; i < ifftVec.size(); i++) {
        ifftVec[i] = conj(ifftVec[i]);
    }

    ifftVec = fft(ifftVec);

    for(int i = 0; i < ifftVec.size(); i++) {
        ifftVec[i] = conj(ifftVec[i]) / complex<double>(ifftVec.size());
    }

    return ifftVec;
}

vector< vector< complex<double> > > fft(const Image* img)
{
    vector< vector< complex<double> > > vec(img->getHeight(), vector< complex<double> >(img->getWidth()));
    for(int i = 0; i < img->getHeight(); i++) {
        for(int j = 0; j < img->getWidth(); j++) {
            vec[i][j] = (*img)[i][j];
        }
    }

    return fft(vec);
}

vector< vector< complex<double> > > fft(const vector< vector< complex<double> > >& inpVec)
{
    vector< vector< complex<double> > > vec(inpVec);
    // 1行のfftを行う
    vector< complex<double> > fftVec(vec.size());

    // 横方向に行う
    for(int i = 0; i < vec.size(); i++) {
        // データのコピー
        for(int j = 0; j < vec[i].size(); j++) {
            fftVec[j] = vec[i][j];
        }
        // fftを行う
        fftVec = fft(fftVec);
        // データを戻す
        for(int j = 0; j < vec[i].size(); j++) {
            vec[i][j] = fftVec[j];
        }
    }

    // 縦方向に行う
    for(int j = 0; j < vec[0].size(); j++) {
        // データのコピー
        for(int i = 0; i < vec.size(); i++) {
            fftVec[i] = vec[i][j];
        }
        // fftを行う
        fftVec = fft(fftVec);
        // データを戻す
        for(int i = 0; i < vec.size(); i++) {
            vec[i][j] = fftVec[i];
        }
    }

    return vec;
}

vector< vector< complex<double> > > ifft(const vector< vector< complex<double> > >& inpVec)
{
    vector< vector< complex<double> > > vec(inpVec);
    // 1行のfftを行う
    vector< complex<double> > ifftVec(vec.size());

    // 横方向に行う
    for(int i = 0; i < vec.size(); i++) {
        // データのコピー
        for(int j = 0; j < vec[i].size(); j++) {
            ifftVec[j] = vec[i][j];
        }
        // ifftを行う
        ifftVec = ifft(ifftVec);
        // データを戻す
        for(int j = 0; j < vec[i].size(); j++) {
            vec[i][j] = ifftVec[j];
        }
    }

    // 縦方向に行う
    for(int j = 0; j < vec[0].size(); j++) {
        // データのコピー
        for(int i = 0; i < vec.size(); i++) {
            ifftVec[i] = vec[i][j];
        }
        // ifftを行う
        ifftVec = ifft(ifftVec);
        // データを戻す
        for(int i = 0; i < vec.size(); i++) {
            vec[i][j] = ifftVec[i];
        }
    }

    return vec;
}

// 低周波を中心に集まるように交換する
void swap(std::vector< std::vector< std::complex<double> > >& vec)
{
    int half = vec.size() / 2;
    for(int i = 0; i < half; i++) {
        for(int j = 0; j < half; j++) {
            swap(vec[i][j], vec[i + half][j + half]);
            swap(vec[i][j + half], vec[i + half][j]);
        }
    }
}
