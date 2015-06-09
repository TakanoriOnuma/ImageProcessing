#pragma once

#include <complex>
#include <vector>
#include "Image.h"

std::vector< std::complex<double> > fft(const std::vector< std::complex<double> >& vec);
Image* fft(const Image* img);

