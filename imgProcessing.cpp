#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

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

class ToneCurve {
public:
    virtual int f(int x) = 0;
};

struct Point {
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class LinearToneCurve : public ToneCurve {
    vector<Point> points;

public:
    // xについて昇順にする
    LinearToneCurve(vector<Point>& points) : points(points) { }

    int f(int x) {
        int y = -1;
        for(int i = 1; i < this->points.size(); i++) {
            if(x <= this->points[i].x) {
                if(this->points[i].x == this->points[i - 1].x) {
                    y = this->points[i].y;
                    break;
                }
                double lean = ((double)this->points[i].y - this->points[i - 1].y) / (this->points[i].x - this->points[i - 1].x);
                y = lean * (x - this->points[i].x) + this->points[i].y;
                break;
            }
        }
        return y;
    }
};

class GammaCurve : public ToneCurve {
    double gamma;
    int min;
    int max;

public:
    GammaCurve(double gamma, int min = 0, int max = 255) {
        this->gamma = gamma;
        this->min = min;
        this->max = max;
    }

    int f(int x) {
        return (int)((double)(max - min) * pow((double)x / 255, 1 / gamma)) + min;
    }
};

class CosCurve : public ToneCurve {
    int A;
    double freq;

public:
    CosCurve(int A = 127, double freq = 1.0) {
        this->A = A;
        this->freq = freq;
    }

    int f(int x) {
        return (int)(A * cos(2 * freq * M_PI * x / 255) + A);
    }
};

class Image {
    int format;
    string comment;
    int height;
    int width;
    int max;
    char* arr;

public:
    Image() {
        init();
    }
    Image(const char filename[]) {
        init();
        load(filename);
    }
    Image(const Image* img) {
        init();
        this->format = img->format;
        this->comment = img->comment;
        if(this->arr != NULL) {
            this->height = img->height;
            this->width  = img->width;
            this->max    = img->max;
            this->arr    = new char[this->height * this->width];
            for(int i = 0; i < this->height; i++) {
                for(int j = 0; j < this->width; j++) {
                    int idx = i * this->width + j;
                    this->arr[idx] = img->arr[idx];
                }
            }
        }
    }
    ~Image() {
        if(arr != NULL) {
            delete arr;
        }
    }

    int getMinBright() const {
        int min;
        int max;
        getBright(&min, &max);
        return min;
    }

    int getMaxBright() const {
        int min;
        int max;
        getBright(&min, &max);
        return max;
    }

    void getBright(int* min, int* max) const {
        *min = 255;
        *max = 0;
        for(int i = 0; i < this->height; i++) {
            for(int j = 0; j < this->width; j++) {
                if(*min > (unsigned char)this->arr[i * this->width + j]) {
                    *min = (unsigned char)this->arr[i * this->width + j];
                }
                if(*max < (unsigned char)this->arr[i * this->width + j]) {
                    *max = (unsigned char)this->arr[i * this->width + j];
                }
            }
        }
    }

    double getContrast() const {
        int min;
        int max;
        getBright(&min, &max);
        cout << min << ", " << max << endl;
        return ((double)(max - min)) / (max + min);
    }

    void init() {
        arr = NULL;
        comment = "";
    }

    void load(const char filename[]) {
        ifstream fin(filename, ios::in | ios::binary);

        readHeader(fin);
        if(arr != NULL) {
            delete arr;
        }
        arr = new char[height * width]();
        fin.read(arr, height * width);

        /*
        for(int i = 0; i < 16; i++) {
            printf("%hhu ", arr[i]);
        }
        cout << '\n';
        */

        fin.close();
    }

    void save(const char filename[]) {
        ofstream fout(filename, ios::out | ios::binary);

        writeHeader(fout);
        fout.write(this->arr, this->height * this->width);
        fout.close();
    }

    void readHeader(ifstream& fin) {
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

    void writeHeader(ofstream& fout) {
        stringstream ss;
        ss << 'P' << format << '\n';
        ss << "# " << comment << '\n';
        ss << this->width << ' ' << this->height << '\n';
        ss << this->max << '\n';
        string str = ss.str();
        fout.write(str.c_str(), str.size());
    }

    void printHeader() {
        cout << "format: P" << format << endl;
        cout << "comment: " << comment << endl;
        cout << "size: " << width << ", " << height << endl;
        cout << "max: " << max << endl;
    }

    string readLine(ifstream& fin) {
        string line = "";
        char ch;
        fin.read(&ch, sizeof(char));
        while(ch != '\n') {
            line += ch;
            fin.read(&ch, sizeof(char));
        }
        return line;
    }

    void inverse() {
        for(int i = 0; i < this->height; i++) {
            for(int j = 0; j < this->width; j++) {
                this->arr[i * this->width + j] = (unsigned char)(255 - this->arr[i * this->width + j]);
            }
        }
    }

    void transCurve(ToneCurve& toneCurve) {
        unsigned char table[256];
        for(int i = 0; i < 256; i++) {
            table[i] = (unsigned char)toneCurve.f(i);
        }
        for(int i = 0; i < this->height; i++) {
            for(int j = 0; j < this->width; j++) {
                int idx = i * this->width + j;
                this->arr[idx] = table[(unsigned char)this->arr[idx]];
            }
        }
    }

    // 横にαブレンディング
    void alphaBlending(const Image* img) {
        for(int j = 0; j < this->width; j++) {
            double alpha = (double)j / this->width;
            for(int i = 0; i < this->height; i++) {
                int idx = i * this->width + j;
                this->arr[idx] = alpha * (unsigned char)this->arr[idx] + (1.0 - alpha) * (unsigned char)img->arr[idx];
            }
        }
    }

    Image* makeHistgram() {
        double hist[256] = {0};
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                hist[(unsigned char)this->arr[i * width + j]] += 1;
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
        imgHist->arr = new char[imgHist->width * imgHist->height];
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

    static Image* makeToneCurve(ToneCurve& toneCurve) {
        int table[256];
        for(int i = 0; i < 256; i++) {
            table[i] = toneCurve.f(i);
        }
        Image* imgCurve = new Image();
        imgCurve->format = 5;
        imgCurve->width  = 256;
        imgCurve->height = 256;
        imgCurve->max = 255;
        imgCurve->arr = new char[imgCurve->height * imgCurve->height];
        for(int i = 0; i < imgCurve->height; i++) {
            for(int j = 0; j < imgCurve->width; j++) {
                imgCurve->arr[i * imgCurve->width + j] = (table[j] == 255 - i) ? 0x00 : 0xff;
            }
        }
        return imgCurve;
    }

    static Image* emboss(const Image* img, int shift = 1) {
        Image* imgEmboss = new Image();
        imgEmboss->format = 5;
        imgEmboss->width  = img->width - shift;
        imgEmboss->height = img->height - shift;
        imgEmboss->max = 255;
        imgEmboss->arr = new char[imgEmboss->height * imgEmboss->width];

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
};

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
