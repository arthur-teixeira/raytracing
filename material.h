#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "utils.h"
#include "vec3.h"
#include <math.h>

class hit_record;

class material {
public:
  virtual ~material() = default;

  virtual bool scatter(const ray &in, const hit_record &rec, color &attenuation,
                       ray &scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const color &a) : albedo(a) {}

  bool scatter(const ray &in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    auto scatter_direction = rec.normal + random_unit_vec();
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    }

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;

    return true;
  }

private:
  color albedo;
};

class metal : public material {
public:
  metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  bool scatter(const ray &in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    vec3 reflected = reflect(unit_vec(in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + fuzz * random_unit_vec());
    attenuation = albedo;

    return (dot(scattered.direction(), rec.normal) > 0);
  }

private:
  color albedo;
  double fuzz;
};

class dieletric : public material {
public:
  dieletric(double refraction_index) : ir(refraction_index) {}

  bool scatter(const ray &in, const hit_record &rec, color &attenuation,
               ray &scattered) const override {
    attenuation = color(1, 1, 1);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = unit_vec(in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool should_reflect = refraction_ratio * sin_theta > 1.0;

    vec3 direction;
    if (should_reflect || reflectance(cos_theta, refraction_ratio) > random_double()) {
      direction = reflect(unit_direction, rec.normal);
    } else {
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = ray(rec.p, direction);

    return true;
  }

private:
  double ir;

  static double reflectance(double cosine, double ref_idx) {
    // Schlicks approximation
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

#endif // MATERIAL_H
