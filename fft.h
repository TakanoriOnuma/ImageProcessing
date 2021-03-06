#pragma once

#include <complex>
#include <vector>
#include "Image.h"

std::vector< std::complex<double> > fft(const std::vector< std::complex<double> >& vec);
std::vector< std::complex<double> > ifft(const std::vector< std::complex<double> >& vec);
std::vector< std::vector< std::complex<double> > > fft(const Image* img);
std::vector< std::vector< std::complex<double> > > fft(const std::vector< std::vector< std::complex<double> > >& vec);
std::vector< std::vector< std::complex<double> > > ifft(const std::vector< std::vector< std::complex<double> > >& vec);
void swap(std::vector< std::vector< std::complex<double> > >& vec);
