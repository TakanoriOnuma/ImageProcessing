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
    // fftで使う行列の作成
    vector< complex<double> > vec(img->getHeight() * img->getWidth());
    for(int i = 0; i < img->getHeight(); i++) {
        for(int j = 0; j < img->getWidth(); j++) {
            vec[i * img->getWidth() + j] = (*img)[i][j];
        }
    }

    // 1行のfftを行う
    vector< complex<double> > fftVec(img->getWidth());

    // 横方向に行う
    for(int i = 0; i < img->getHeight(); i++) {
        // データのコピー
        for(int j = 0; j < img->getWidth(); j++) {
            fftVec[j] = vec[i * img->getWidth() + j];
        }
        cout << "input:";
        for(int k = 0; k < fftVec.size(); k++) {
            cout << fftVec[k] << ", ";
        }
        cout << '\n';
        // fftを行う
        fftVec = fft(fftVec);
        cout << "fft:";
        for(int k = 0; k < fftVec.size(); k++) {
            cout << fftVec[k] << ", ";
        }
        cout << '\n';
        // データを戻す
        for(int j = 0; j < img->getWidth(); j++) {
            vec[i * img->getWidth() + j] = fftVec[j];
        }
    }

/*
    // データを表示する
    for(int i = 0; i < vec.size(); i++) {
        if(i % img->getWidth() == 0) {
            cout << '\n';
        }
        cout << vec[i] << ", ";
    }
*/

    // 縦方向に行う
    for(int j = 0; j < img->getWidth(); j++) {
        // データのコピー
        for(int i = 0; i < img->getHeight(); i++) {
            fftVec[i] = vec[i * img->getWidth() + j];
        }
        cout << "input:";
        for(int k = 0; k < fftVec.size(); k++) {
            cout << fftVec[k] << ", ";
        }
        cout << '\n';
        // fftを行う
        fftVec = fft(fftVec);
        cout << "fft:";
        for(int k = 0; k < fftVec.size(); k++) {
            cout << fftVec[k] << ", ";
        }
        cout << '\n';
        // データを戻す
        for(int i = 0; i < img->getHeight(); i++) {
            vec[i * img->getWidth() + j] = fftVec[i];
        }
    }

    // データを表示する
    for(int i = 0; i < vec.size(); i++) {
        if(i % img->getWidth() == 0) {
            cout << '\n';
        }
        cout << vec[i] << ", ";
    }

    // 絶対値を求める
    double max = 0;
    vector<double> absVec(vec.size());
    for(int i = 0; i < vec.size(); i++) {
        absVec[i] = abs(vec[i]);
        if(absVec[i] > max) {
            max = absVec[i];
        }
    }

    // 画像を作る
    Image* fftImg = new Image(img->getHeight(), img->getWidth());
    for(int i = 0; i < fftImg->getHeight(); i++) {
        for(int j = 0; j < fftImg->getWidth(); j++) {
            (*fftImg)[i][j] = (unsigned char)(255 * absVec[i * img->getWidth() + j] / max);
        }
    }

    return fftImg;
}
