#ifndef NOISE_PERLIN_H
#define NOISE_PERLIN_H

void perlin_setup(double frequency, double lacunarity, double octaves, double persist, int seed, int quality) ;
double perlin_get_value(double x, double y, double z);
#endif //NOISE_PERLIN_H
