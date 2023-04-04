//
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

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