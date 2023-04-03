#ifndef NOISE_NOISEGEN_H
#define NOISE_NOISEGEN_H

double make_int_32_range (double n);
double gradient_coherent_noise3D (double x, double y, double z, int seed, int noiseQuality);
double gradient_noise3D (double fx, double fy, double fz, int ix,
                         int iy, int iz, int seed);
#endif //NOISE_NOISEGEN_H
