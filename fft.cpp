#include <vector>
#include "fft.h"

using namespace std;

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
            cout << "idx:" << idx << endl;
            for(int n = 0; n < step; n++, idx++) {
                complex<double> buf = fftVec[idx] + fftVec[idx + step];
                fftVec[idx + step] = w[b * n] * (fftVec[idx] - fftVec[idx + step]);
                fftVec[idx] = buf;
            }
            idx += step;
        }

        cout << "---------- step" <<  b << " ------------" << endl;
        for(int i = 0; i < fftVec.size(); i++) {
            cout << fftVec[i] << endl;
        }
    }

    // ビットリバース

    return fftVec;
}

Image* fft(const Image* img)
{
    return NULL;
}
