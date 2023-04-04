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
