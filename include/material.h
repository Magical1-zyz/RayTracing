//
// Created by ASUS on 2025/2/18.
//

#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "hittable.h"

class material {
    // The material class is an abstract base class for materials.
    // It provides a scatter function that returns a scattered ray
    // or indicates that the ray was absorbed.
public:
    virtual ~material() = default;

    virtual bool scatter (
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
            ) const {
        return false;
    }
};


#endif //RAYTRACINGINONEWEEKEND_MATERIAL_H
