#ifndef VEC3_H
#define VEC3_H

#include "utils.h"
#include <cmath>
#include <cwchar>
#include <iostream>

using std::sqrt;

class vec3 {
public:
  double e[3];

  vec3() : e{0, 0, 0} {}
  vec3(double x, double y, double z) : e{x, y, z} {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
  }

  vec3 &operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;

    return *this;
  }

  vec3 &operator/=(double t) { return *this *= 1 / t; }

  double length() { return sqrt(length_squared()); }

  double length_squared() { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

  bool near_zero() const {
    auto s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
  }

  static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }

  static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
  }
};

using point3 = vec3;
using color = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double t, const vec3 &v) {
  return vec3(v.x() * t, v.y() * t, v.z() * t);
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }

inline vec3 operator+(const vec3 &v, double t) {
  return vec3(v.x() + t, v.y() + t, v.z() + t);
}

inline double dot(const vec3 &u, const vec3 &v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vec(vec3 v) { return v / v.length(); }

inline vec3 random_in_unit_sphere() {
  while (true) {
    vec3 p = vec3::random(-1, 1);
    if (p.length_squared() < 1) {
      return p;
    }
  }
}

inline vec3 random_unit_vec() { return unit_vec(random_in_unit_sphere()); }

inline vec3 random_on_hemisphere(const vec3 &normal) {
  vec3 on_unit_sphere = random_unit_vec();
  if (dot(on_unit_sphere, normal) > 0.0) {
    return on_unit_sphere;
  }

  return -on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v,n) * n;
}

#endif // VEC3_H
