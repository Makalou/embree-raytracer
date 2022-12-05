#pragma once

#include <utility>
#include "core/vec3.h"

NAMESPACE_BEGIN(ert)
    class texture {
    public:
        virtual color value(ERTFloat u, ERTFloat v, const point3 &p) const = 0;

        virtual ~texture() = default;

        virtual size_t hash() const = 0;

        virtual bool operator==(const texture &m) const = 0;
    };

    NAMESPACE_BEGIN(tex)
        class solid_color : public texture {
        public:
            solid_color() = default;

            explicit solid_color(color c) : color_value(c) {}

            solid_color(ERTFloat r, ERTFloat g, ERTFloat b) : solid_color(color(r, g, b)) {}

            color value(ERTFloat u, ERTFloat v, const point3 &p) const override {
                return color_value;
            }

            size_t hash() const override {
                return std::hash<ERTFloat>()(color_value[0]) ^
                       std::hash<ERTFloat>()(color_value[1]) ^
                       std::hash<ERTFloat>()(color_value[2]);
            }

            bool operator==(const texture &t) const override {
                if (typeid(*this) != typeid(t)) {
                    return false;
                } else {
                    return color_value == dynamic_cast<const solid_color &>(t).color_value;
                }
            }

        private:
            color color_value;
        };

        class checker_texture : public texture {
        public:
            checker_texture() = default;

            checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
                    : even(std::move(_even)), odd(std::move(_odd)) {}

            checker_texture(color c1, color c2)
                    : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

            color value(ERTFloat u, ERTFloat v, const point3 &p) const override;

            size_t hash() const override {
                return 0;
            }

            bool operator==(const texture &t) const override {
                if (typeid(*this) != typeid(t)) {
                    return false;
                } else {
                    return scale == dynamic_cast<const checker_texture &>(t).scale;
                }
            }

        private:
            shared_ptr<texture> odd;
            shared_ptr<texture> even;
            float scale = 10;
        };

        class image_texture : public texture {
        public:
            const static int bytes_per_pixel = 3;

            image_texture()
                    : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

            explicit image_texture(const char *filename);

            color value(ERTFloat u, ERTFloat v, const vec3 &p) const override;

            size_t hash() const override {
                return 0;
            }

            ~image_texture() override {
                delete data;
            }

            bool operator==(const texture &t) const override {
                if (typeid(*this) != typeid(t)) {
                    return false;
                } else {
                    return data == dynamic_cast<const image_texture &>(t).data;
                }
            }

        private:
            unsigned char *data;
            int width, height;
            int bytes_per_scanline;
        };
    NAMESPACE_END(tex)
NAMESPACE_END(ert)