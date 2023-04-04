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

#include "noisegen.h"
#include "vectortable.h"
#include "interp.h"
#include "math.h"

const int X_NOISE_GEN = 1619;
const int Y_NOISE_GEN = 31337;
const int Z_NOISE_GEN = 6971;
const int SEED_NOISE_GEN = 1013;
const int SHIFT_NOISE_GEN = 8;

double gradient_coherent_noise3D (double x, double y, double z, int seed, int noiseQuality)
{
    int x0 = (x > 0.0? (int)x: (int)x - 1);
    int x1 = x0 + 1;
    int y0 = (y > 0.0? (int)y: (int)y - 1);
    int y1 = y0 + 1;
    int z0 = (z > 0.0? (int)z: (int)z - 1);
    int z1 = z0 + 1;

    double xs = 0, ys = 0, zs = 0;
    switch (noiseQuality) {
        case 0: // QUALITY_FAST
            xs = (x - (double)x0);
            ys = (y - (double)y0);
            zs = (z - (double)z0);
            break;
        case 1: // QUALITY_STD
            xs = SCurve3 (x - (double)x0);
            ys = SCurve3 (y - (double)y0);
            zs = SCurve3 (z - (double)z0);
            break;
        case 2: // QUALITY_BEST
            xs = SCurve5 (x - (double)x0);
            ys = SCurve5 (y - (double)y0);
            zs = SCurve5 (z - (double)z0);
            break;
    }

    double n0, n1, ix0, ix1, iy0, iy1;
    n0   = gradient_noise3D(x, y, z, x0, y0, z0, seed);
    n1   = gradient_noise3D(x, y, z, x1, y0, z0, seed);
    ix0  = LinearInterp (n0, n1, xs);
    n0   = gradient_noise3D(x, y, z, x0, y1, z0, seed);
    n1   = gradient_noise3D(x, y, z, x1, y1, z0, seed);
    ix1  = LinearInterp (n0, n1, xs);
    iy0  = LinearInterp (ix0, ix1, ys);
    n0   = gradient_noise3D(x, y, z, x0, y0, z1, seed);
    n1   = gradient_noise3D(x, y, z, x1, y0, z1, seed);
    ix0  = LinearInterp (n0, n1, xs);
    n0   = gradient_noise3D(x, y, z, x0, y1, z1, seed);
    n1   = gradient_noise3D(x, y, z, x1, y1, z1, seed);
    ix1  = LinearInterp (n0, n1, xs);
    iy1  = LinearInterp (ix0, ix1, ys);

    return LinearInterp (iy0, iy1, zs);
}

double gradient_noise3D (double fx, double fy, double fz, int ix,
                         int iy, int iz, int seed)
{
    int vectorIndex = (
                              X_NOISE_GEN    * ix
                              + Y_NOISE_GEN    * iy
                              + Z_NOISE_GEN    * iz
                              + SEED_NOISE_GEN * seed)
                      & 0xffffffff;
    vectorIndex ^= (vectorIndex >> SHIFT_NOISE_GEN);
    vectorIndex &= 0xff;

    double xvGradient = g_randomVectors[(vectorIndex << 2)    ];
    double yvGradient = g_randomVectors[(vectorIndex << 2) + 1];
    double zvGradient = g_randomVectors[(vectorIndex << 2) + 2];

    double xvPoint = (fx - (double)ix);
    double yvPoint = (fy - (double)iy);
    double zvPoint = (fz - (double)iz);

    return ((xvGradient * xvPoint)
            + (yvGradient * yvPoint)
            + (zvGradient * zvPoint)) * 2.12;
}

double make_int_32_range(double n)
{
    if (n >= 1073741824.0) {
        return (2.0 * fmod (n, 1073741824.0)) - 1073741824.0;
    } else if (n <= -1073741824.0) {
        return (2.0 * fmod (n, 1073741824.0)) + 1073741824.0;
    } else {
        return n;
    }
}