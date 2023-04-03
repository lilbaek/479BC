#ifndef NOISE_INTERP_H
#define NOISE_INTERP_H

  inline double LinearInterp (double n0, double n1, double a)
  {
    return ((1.0 - a) * n0) + (a * n1);
  }

  inline double SCurve3 (double a)
  {
    return (a * a * (3.0 - 2.0 * a));
  }

  inline double SCurve5 (double a)
  {
    double a3 = a * a * a;
    double a4 = a3 * a;
    double a5 = a4 * a;
    return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
  }

#endif
