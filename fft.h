#pragma once

#include <complex>
#include <vector>
#include "Image.h"

std::vector< std::complex<double> > fft(const std::vector< std::complex<double> >& vec);
std::vector< std::vector< std::complex<double> > > fft(const Image* img);

