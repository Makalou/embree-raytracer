//
// Created by 王泽远 on 2022/4/14.
//

#ifndef EMBREE_RAYTRACER_MATERIAL_H
#define EMBREE_RAYTRACER_MATERIAL_H

#include <utility>
#include "core/utility.h"
#include "core/vec3.h"
#include "core/texture.h"
#include "embree3/rtcore.h"
#include "core/hit_record.h"
#include "shader.h"

NAMESPACE_BEGIN(ert)

    class hit_record;

    class material {
    public:
        material() = default;

        virtual ~material() = default;

        virtual color emitted(ERTFloat u, ERTFloat v, const point3 &p) const {
            return {0, 0, 0};
        }

        virtual bool scatter(
                const RTCRay &r_in, const hit_record &rec, color &attenuation, RTCRay &scattered
        ) const = 0;

        virtual double scattering_pdf(const RTCRay &r_in, const hit_record &rec, const RTCRay &scattered) const = 0;

        virtual size_t hash() const = 0;

        virtual bool operator==(const material &m) const = 0;

        bool is_specular;

    protected:
        std::shared_ptr<shader> mp_shader;
    };

    NAMESPACE_BEGIN(mat)

        class lambertian : public material {
        public:
            explicit lambertian(const color &a) : albedo(std::make_shared<tex::solid_color>(a)) {
                is_specular = false;
            }

            explicit lambertian(shared_ptr<texture> a) : albedo(std::move(a)) {
                is_specular = false;
            }

            bool scatter(
                    const RTCRay &r_in, const hit_record &rec, color &alb, RTCRay &scattered
            ) const override {
                auto scatter_direction = unit_vector(rec.normal + random_unit_vector<ERTFloat>());

                if (scatter_direction.near_zero()) {
                    //scatter_direction = unit_vector(rec.normal);
                    scatter_direction = rec.normal;
                }

                memcpy(&scattered.org_x, rec.p.e, 96);
                memcpy(&scattered.dir_x, scatter_direction.e, 96);

                alb = albedo->value(rec.u, rec.v, rec.p);
                //pdf = dot(rec.normal,scatter_direction)/pi;

                return true;
            }

            double scattering_pdf(const RTCRay &r_in, const hit_record &rec, const RTCRay &scattered) const override {
                auto cosine = dot(rec.normal, {scattered.dir_x, scattered.dir_y, scattered.dir_z});
                return cosine < 0.0 ? 0.0 : cosine / pi;
            }

            size_t hash() const override {
                return albedo->hash();
            }

            bool operator==(const material &m) const override {
                if (typeid(*this) != typeid(m)) {
                    return false;
                } else {
                    return *albedo == *dynamic_cast<const lambertian &>(m).albedo;
                }
                //return albedo==l.albedo;
            }

        public:
            shared_ptr<texture> albedo;
        };

        class metal : public material {
        public:
            metal(const color &a, ERTFloat f) : albedo(make_shared<tex::solid_color>(a)), fuzz(f < 1 ? f : 1) {
                is_specular = true;
            }

            metal(shared_ptr<texture> a, ERTFloat f) : albedo(std::move(a)), fuzz(f < 1 ? f : 1) {
                is_specular = true;
            }

            bool scatter(
                    const RTCRay &r_in, const hit_record &rec, color &attenuation, RTCRay &scattered
            ) const override {
                vec3 reflected = reflect(unit_vector<ERTFloat>({r_in.dir_x, r_in.dir_y, r_in.dir_z}), rec.normal);
                //scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());

                scattered.org_x = rec.p.x();
                scattered.org_y = rec.p.y();
                scattered.org_z = rec.p.z();
                auto scatter_direction = reflected + fuzz * random_in_unit_sphere<ERTFloat>();
                scattered.dir_x = scatter_direction.x();
                scattered.dir_y = scatter_direction.y();
                scattered.dir_z = scatter_direction.z();
                attenuation = albedo->value(rec.u, rec.v, rec.p);
                return (dot({scattered.dir_x, scattered.dir_y, scattered.dir_z}, rec.normal) > 0);
            }

            double scattering_pdf(const RTCRay &r_in, const hit_record &rec, const RTCRay &scattered) const override {
                return 0;
            }

            size_t hash() const override {
                return albedo->hash();
            }

            bool operator==(const material &m) const override {
                if (typeid(*this) != typeid(m)) {
                    return false;
                } else {
                    return *albedo == *dynamic_cast<const metal &>(m).albedo;
                }
                //return albedo==l.albedo;
            }

        public:
            shared_ptr<texture> albedo;
            ERTFloat fuzz;
        };

        class dielectric : public material {
        public:
            explicit dielectric(ERTFloat index_of_refraction) : ir(index_of_refraction) {
                is_specular = true;
            }

            bool scatter(
                    const RTCRay &r_in, const hit_record &rec, color &attenuation, RTCRay &scattered
            ) const override {
                attenuation = color(1.0, 1.0, 1.0);
                //the refraction rate matters face orientation
                ERTFloat refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

                vec3 unit_direction = unit_vector<ERTFloat>({r_in.dir_x, r_in.dir_y, r_in.dir_z});
                ERTFloat cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
                ERTFloat sin_theta = sqrt(1.0 - cos_theta * cos_theta);

                bool cannot_refract = refraction_ratio * sin_theta > 1.0;
                vec3 direction;
                if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_FLOAT())
                    direction = reflect(unit_direction, rec.normal);
                else
                    direction = refract(unit_direction, rec.normal, refraction_ratio);

                //scattered = ray(rec.p, direction);
                scattered.org_x = rec.p.x();
                scattered.org_y = rec.p.y();
                scattered.org_z = rec.p.z();
                scattered.dir_x = direction.x();
                scattered.dir_y = direction.y();
                scattered.dir_z = direction.z();
                return true;
            }

            double scattering_pdf(const RTCRay &r_in, const hit_record &rec, const RTCRay &scattered) const override {
                return 0;
            }

            size_t hash() const override {
                return std::hash<ERTFloat>()(ir);
            }

            bool operator==(const material &m) const override {
                if (typeid(*this) != typeid(m)) {
                    return false;
                } else {
                    return ir == dynamic_cast<const dielectric &>(m).ir;
                }
                //return albedo==l.albedo;
            }

        public:
            ERTFloat ir; // Index of Refraction
        private:
            static ERTFloat reflectance(ERTFloat cosine, ERTFloat ref_idx) {
                // Use Schlick's approximation for reflectance.
                auto r0 = (1 - ref_idx) / (1 + ref_idx);
                r0 = r0 * r0;
                return r0 + (1 - r0) * pow((1 - cosine), 5);
            }
        };

        class diffuse_light : public material {
        public:
            explicit diffuse_light(shared_ptr<texture> a) : emit(std::move(a)) {}

            explicit diffuse_light(color c) : emit(make_shared<tex::solid_color>(c)) {}

            bool scatter(
                    const RTCRay &r_in, const hit_record &rec, color &attenuation, RTCRay &scattered
            ) const override {
                return false;
            }

            color emitted(ERTFloat u, ERTFloat v, const point3 &p) const override {
                return emit->value(u, v, p);
            }

            double scattering_pdf(const RTCRay &r_in, const hit_record &rec, const RTCRay &scattered) const override {
                return 0;
            }

            size_t hash() const override {
                return std::hash<std::string>()("diffuse_light");
            }

            bool operator==(const material &m) const override {
                if (typeid(*this) != typeid(m)) {
                    return false;
                } else {
                    return emit == dynamic_cast<const diffuse_light &>(m).emit;
                }
            }

        public:
            shared_ptr<texture> emit;
        };
/*
class isotropic:public material
{
public:
    explicit isotropic(color c) : albedo(make_shared<tex::solid_color>(c)) {}
    explicit isotropic(shared_ptr<texture> a) : albedo(std::move(a)) {}

    bool scatter(
            const RTCRay& r_in, const hit_record& rec, color& attenuation, RTCRay& scattered,ERTFloat & pdf
    ) const override {
        //scattered = ray(rec.p, random_in_unit_sphere());
        auto scatter_direction = random_in_unit_sphere<ERTFloat>();
        scattered.org_x = rec.p.x();
        scattered.org_y = rec.p.y();
        scattered.org_z = rec.p.z();
        scattered.dir_x = scatter_direction.x();
        scattered.dir_y = scatter_direction.y();
        scattered.dir_z = scatter_direction.z();
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    double scattering_pdf(const RTCRay& r_in,const hit_record& rec, const RTCRay& scattered) const override{
        return 0;
    }

public:
    shared_ptr<texture> albedo;
};
*/
    NAMESPACE_END(mat)
NAMESPACE_END(ert)

#endif //EMBREE_RAYTRACER_MATERIAL_H
