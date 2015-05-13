#pragma once

#include <vector>
#include <math.h>
#include "Point.h"

class ToneCurve {
public:
    virtual int f(int x) = 0;
};

class LinearToneCurve : public ToneCurve {
    std::vector<Point> points;

public:
    // xについて昇順にする
    LinearToneCurve(std::vector<Point>& points) : points(points) { }

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
