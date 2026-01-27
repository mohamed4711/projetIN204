/*
    SimpleRenderer.hpp
    Standard single-threaded renderer with direct lighting support
    Computes both direct illumination from lights and indirect bounces
*/

#ifndef SIMPLE_RENDERER_HPP
#define SIMPLE_RENDERER_HPP

#include "Renderer.hpp"
#include "../../lights/hpp/Light_list.hpp"

// Single-threaded renderer with full lighting computation
class SimpleRenderer : public Renderer {
public:
    SimpleRenderer();
    SimpleRenderer(const SimpleRenderer& other);
    ~SimpleRenderer() override = default;
    
    // Main render function
    void Render(const Scene& scene, Image& image) override;
    
private:
    // Ray color with direct + indirect lighting
    Vector3 RayColor(const Ray& r, const hittable_list& world, const Light_list& lights, int depth);
};

// Alias for backward compatibility
using DefaultRenderer = SimpleRenderer;

#endif
