#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, color c) {
  int ir = static_cast<int>(255.999 * c.x());
  int ig = static_cast<int>(255.999 * c.y());
  int ib = static_cast<int>(255.999 * c.z());

  out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif // COLOR_H
