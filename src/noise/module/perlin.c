#include "perlin.h"
#include "noise/noisegen.h"

static struct {
    double frequency;
    double lacunarity;
    double octave_count;
    double persistence;
    int quality;
    int seed;
    double octave;
} data;

/*
 * Quality = 0 = fast, low quality
 * Quality = 1 = standard quality
 * Quality = 2 = best quality
 */
void perlin_setup(double frequency, double lacunarity, double octaves, double persist, int seed, int quality) {
    data.frequency = frequency;
    data.lacunarity = lacunarity;
    data.octave_count = octaves;
    data.persistence = persist;
    data.quality = quality;
    data.seed = seed;
}

double perlin_get_value(double x, double y, double z) {
    double value = 0.0;
    double signal = 0.0;
    double curPersistence = 1.0;
    double nx, ny, nz;
    int seed;

    x *= data.frequency;
    y *= data.frequency;
    z *= data.frequency;

    for (int curOctave = 0; curOctave < data.octave_count; curOctave++) {
        nx = make_int_32_range(x);
        ny = make_int_32_range(y);
        nz = make_int_32_range(z);

        seed = (data.seed + curOctave) & 0xffffffff;
        signal = gradient_coherent_noise3D(nx, ny, nz, seed, data.quality);
        value += signal * curPersistence;

        x *= data.lacunarity;
        y *= data.lacunarity;
        z *= data.lacunarity;
        curPersistence *= data.persistence;
    }

    return value;
}