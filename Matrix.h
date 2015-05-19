#pragma once

template <typename T>
class Matrix {
    int height;
    int width;
    T* mat;

public:
    // --- コンストラクタ --- //
    Matrix(int height, int width) {
        this->height = height;
        this->width  = width;
        mat = new T[height * width]();
    }

    // --- デストラクタ --- //
    ~Matrix() {
        if(mat != NULL) {
            delete mat;
        }
    }

    // --- ゲッター --- //
    int getHeight() const { return height; }
    int getWidth()  const { return width;  }

    T* operator[] (int idx) { return mat + idx * width; } 
    const T* operator[] (int idx) const { return mat + idx * width; }
};
