#include <iostream>
#include <sstream>

#include "Image.h"

using namespace std;

// トリミング関数
string trim(const string& str, const char* trimCharList = "\t\v\r\n")
{
    string result;

    // 左からトリムする文字以外を探す
    string::size_type left = str.find_first_not_of(trimCharList);

    // 開始位置かどうかチェック
    if(left != string::npos) {
        // 右からも検索
        string::size_type right = str.find_last_not_of(trimCharList);

        result = str.substr(left, right);
    }
    return result;
}

// --- コンストラクタ --- //
Image::Image() {
    init();
}

Image::Image(const char filename[]) {
    init();
    load(filename);
}

Image::Image(const Image& img) {
    init();
    this->format = img.format;
    this->comment = img.comment;
    if(this->arr != NULL) {
        this->height = img.height;
        this->width  = img.width;
        this->max    = img.max;
        this->arr    = new unsigned char[this->height * this->width];
        for(int i = 0; i < this->height; i++) {
            for(int j = 0; j < this->width; j++) {
                int idx = i * this->width + j;
                this->arr[idx] = img.arr[idx];
            }
        }
    }
}

void Image::init() {
    arr = NULL;
    comment = "";
}

// --- デストラクタ --- //
Image::~Image() {
    if(arr != NULL) {
        delete arr;
    }
}

// --- ゲッター --- //
int Image::getMinBright() const {
    int min;
    int max;
    getBright(&min, &max);
    return min;
}
int Image::getMaxBright() const {
    int min;
    int max;
    getBright(&min, &max);
    return max;
}
void Image::getBright(int* min, int* max) const {
    *min = 255;
    *max = 0;
    for(int i = 0; i < this->height; i++) {
        for(int j = 0; j < this->width; j++) {
            if(*min > this->arr[i * this->width + j]) {
                *min = this->arr[i * this->width + j];
            }
            if(*max < this->arr[i * this->width + j]) {
                *max = this->arr[i * this->width + j];
            }
        }
    }
}

double Image::getContrast() const {
    int min;
    int max;
    getBright(&min, &max);
    return ((double)(max - min)) / (max + min);
}


void Image::load(const char filename[]) {
    ifstream fin(filename, ios::in | ios::binary);

    readHeader(fin);
    if(arr != NULL) {
        delete arr;
    }
    arr = new unsigned char[height * width]();
    fin.read((char*)arr, height * width);

    fin.close();
}

void Image::save(const char filename[]) {
    ofstream fout(filename, ios::out | ios::binary);

    writeHeader(fout);
    fout.write((char*)this->arr, this->height * this->width);
    fout.close();
}

void Image::readHeader(ifstream& fin) {
    string line = readLine(fin);
    format = line[1] - '0';

    line = readLine(fin);
    if(line[0] == '#') {
        comment = trim(line, "# ");
        line = readLine(fin);
    }

    sscanf(line.c_str(), "%d %d", &width, &height);

    line = readLine(fin);
    sscanf(line.c_str(), "%d", &max);
}

void Image::writeHeader(ofstream& fout) {
    stringstream ss;
    ss << 'P' << format << '\n';
    ss << "# " << comment << '\n';
    ss << this->width << ' ' << this->height << '\n';
    ss << this->max << '\n';
    string str = ss.str();
    fout.write(str.c_str(), str.size());
}

void Image::printHeader() const {
    cout << "format: P" << format << endl;
    cout << "comment: " << comment << endl;
    cout << "size: " << width << ", " << height << endl;
    cout << "max: " << max << endl;
}

string Image::readLine(ifstream& fin) {
    string line = "";
    char ch;
    fin.read(&ch, sizeof(char));
    while(ch != '\n') {
        line += ch;
        fin.read(&ch, sizeof(char));
    }
    return line;
}


void Image::inverse() {
    for(int i = 0; i < this->height; i++) {
        for(int j = 0; j < this->width; j++) {
            this->arr[i * this->width + j] = (255 - this->arr[i * this->width + j]);
        }
    }
}


void Image::transCurve(ToneCurve& toneCurve) {
    unsigned char table[256];
    for(int i = 0; i < 256; i++) {
        table[i] = (unsigned char)toneCurve.f(i);
    }
    for(int i = 0; i < this->height; i++) {
        for(int j = 0; j < this->width; j++) {
            int idx = i * this->width + j;
            this->arr[idx] = table[this->arr[idx]];
        }
    }
}

void Image::alphaBlending(const Image* img) {
    for(int j = 0; j < this->width; j++) {
        double alpha = (double)j / this->width;
        for(int i = 0; i < this->height; i++) {
            int idx = i * this->width + j;
            this->arr[idx] = alpha * this->arr[idx] + (1.0 - alpha) * img->arr[idx];
        }
    }
}

Image* Image::makeHistgram() const {
    double hist[256] = {0};
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            hist[this->arr[i * width + j]] += 1;
        }
    }

    double max = 0;
    for(int i = 0; i < 256; i++) {
        if(hist[i] > max) {
            max = hist[i];
        }
    }
    for(int i = 0; i < 256; i++) {
        hist[i] = hist[i] * 256 / max;
    }

    Image* imgHist = new Image();
    imgHist->format = 5;
    imgHist->width  = 256;
    imgHist->height = 256;
    imgHist->max = 255;
    imgHist->arr = new unsigned char[imgHist->width * imgHist->height];
    for(int i = 0; i < imgHist->height; i++) {
        for(int j = 0; j < imgHist->width; j++) {
            if(256 - i > hist[j]) {
                imgHist->arr[i * imgHist->width + j] = 0xff;
            }
            else {
                imgHist->arr[i * imgHist->width + j] = 0x00;
            }
        }
    }

    return imgHist;
}


// --- static method --- //
Image* Image::makeToneCurve(ToneCurve& toneCurve) {
    int table[256];
    for(int i = 0; i < 256; i++) {
        table[i] = toneCurve.f(i);
    }
    Image* imgCurve = new Image();
    imgCurve->format = 5;
    imgCurve->width  = 256;
    imgCurve->height = 256;
    imgCurve->max = 255;
    imgCurve->arr = new unsigned char[imgCurve->height * imgCurve->height];
    for(int i = 0; i < imgCurve->height; i++) {
        for(int j = 0; j < imgCurve->width; j++) {
            imgCurve->arr[i * imgCurve->width + j] = (table[j] == 255 - i) ? 0x00 : 0xff;
        }
    }
    return imgCurve;
}

Image* Image::emboss(const Image* img, int shift) {
    Image* imgEmboss = new Image();
    imgEmboss->format = 5;
    imgEmboss->width  = img->width - shift;
    imgEmboss->height = img->height - shift;
    imgEmboss->max = 255;
    imgEmboss->arr = new unsigned char[imgEmboss->height * imgEmboss->width];

    for(int i = 0; i < imgEmboss->height; i++) {
        for(int j = 0; j < imgEmboss->width; j++) {
            int idx = i * imgEmboss->width + j;
            int img_idx = i * img->width + j;
            int shift_idx = (i + shift) * img->width + (j + shift);
            imgEmboss->arr[idx] = img->arr[img_idx] + (255 - img->arr[shift_idx]) - 128;
        }
    }

    return imgEmboss;
}
