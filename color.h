#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color c, int samples_per_pixel) {
  double r = c.x();
  double g = c.y();
  double b = c.z();

  double scale = 1.0 / samples_per_pixel;

  r *= scale;
  g *= scale;
  b *= scale;

  static const interval intensity(0.000, 0.999);

  out << static_cast<int>(256 * intensity.clamp(r)) << ' ' 
      << static_cast<int>(256 * intensity.clamp(g)) << ' '
      << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif // COLOR_H
