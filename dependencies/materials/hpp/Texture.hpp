/*
    Texture.hpp
    Base class and simple procedural textures
*/

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <cmath>
#include "../../utils/hpp/Vector3.hpp"
#include "../../libs.hpp"

class Texture {
public:
    virtual ~Texture() = default;
    virtual Vector3 value(const Point3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor() : m_color(0, 0, 0) {}
    explicit SolidColor(const Vector3& color) : m_color(color) {}

    Vector3 value(const Point3& p) const override {
        (void)p;
        return m_color;
    }

private:
    Vector3 m_color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd, double scale)
        : m_even(std::move(even)), m_odd(std::move(odd)), m_scale(scale) {}

    Vector3 value(const Point3& p) const override {
        double s = std::sin(m_scale * p.x) * std::sin(m_scale * p.y) * std::sin(m_scale * p.z);
        return s < 0 ? m_odd->value(p) : m_even->value(p);
    }

private:
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;
    double m_scale;
};

class StripeTexture : public Texture {
public:
    StripeTexture(std::shared_ptr<Texture> a, std::shared_ptr<Texture> b, const Vector3& axis, double scale)
        : m_a(std::move(a)), m_b(std::move(b)), m_axis(axis), m_scale(scale) {
        if (m_axis.length() > 0) {
            m_axis = unit_vector(m_axis);
        } else {
            m_axis = Vector3(1, 0, 0);
        }
    }

    Vector3 value(const Point3& p) const override {
        double s = std::sin(m_scale * dot(p, m_axis));
        return s < 0 ? m_b->value(p) : m_a->value(p);
    }

private:
    std::shared_ptr<Texture> m_a;
    std::shared_ptr<Texture> m_b;
    Vector3 m_axis;
    double m_scale;
};

#endif
