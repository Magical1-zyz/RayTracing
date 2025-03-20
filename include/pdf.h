//
// Created by ASUS on 2025/3/14.
//

#ifndef RAYTRACINGINONEWEEKEND_INCLUDE_PDF_H_
#define RAYTRACINGINONEWEEKEND_INCLUDE_PDF_H_


#include "rtweekend.h"

#include "hittable_list.h"
#include "onb.h"


class pdf {
 public:
  virtual ~pdf() = default;

  [[nodiscard]] virtual double value(const vec3& direction) const = 0;
  [[nodiscard]] virtual vec3 generate() const = 0;
};

class sphere_pdf : public pdf {
  // The sphere_pdf class represents a probability density function for a uniform density over the unit sphere.
 public:
  sphere_pdf() = default;

  // The value function returns the probability density function value for the given direction.
  [[nodiscard]] double value(const vec3& direction) const override {
    return 1 / (4 * pi);
  }

  // The generate function returns a random direction on the unit sphere.
  [[nodiscard]] vec3 generate() const override {
    return random_unit_vector();
  }
};


class cosine_pdf : public pdf {
  // The cosine_pdf class represents a probability density function for a cosine-weighted hemisphere.
 public:
  explicit cosine_pdf(const vec3& w) : uvw(w) {}

  // The value function returns the probability density function value for the given direction.
  [[nodiscard]] double value(const vec3& direction) const override {
    auto cosine_theta = dot(unit_vector(direction), uvw.w());
    return (cosine_theta <= 0) ? 0 : cosine_theta / pi;
  }

  // Generates a random direction vector towards the cosine-weighted hemisphere
  [[nodiscard]] vec3 generate() const override {
    return uvw.transform(random_cosine_direction());
  }

 private:
  onb uvw;
};

class hittable_pdf : public pdf {
  // The hittable_pdf class represents a probability density function for a hittable object.
 public:
  hittable_pdf(const hittable& objects, const point3& origin) : objects(objects), origin(origin) {}

  // Returns the PDF value for the given direction.
  [[nodiscard]] double value(const vec3& direction) const override {
    return objects.pdf_value(origin, direction);
  }

  // Generates a random direction vector towards the hittable objects
  [[nodiscard]] vec3 generate() const override {
    return objects.random(origin);
  }

 private:
  const hittable& objects;
  point3 origin;
};



class mixture_pdf : public pdf {
 public:
  // The mixture_pdf class represents a mixture of two probability density functions
  // It combines two PDFs with equal weights (50% each) for importance sampling

  mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
    p[0] = std::move(p0);
    p[1] = std::move(p1);
  }

  // Returns the weighted sum of PDF values from both components
  [[nodiscard]] double value(const vec3& direction) const override {
    return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
  }

  // Randomly generates a direction by selecting one of the two PDFs
  [[nodiscard]] vec3 generate() const override {
    if (random_double() < 0.5)
      return p[0]->generate();
    else
      return p[1]->generate();
  }

 private:
  shared_ptr<pdf> p[2];  // Array storing the two component PDFs
};

#endif //RAYTRACINGINONEWEEKEND_INCLUDE_PDF_H_
