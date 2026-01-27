/*
    ParallelRenderer.hpp
    Multi-threaded renderer using OpenMP
    Distributes scanlines across CPU cores for faster rendering
*/

#ifndef PARALLEL_RENDERER_HPP
#define PARALLEL_RENDERER_HPP

#include "Renderer.hpp"
#include "../../lights/hpp/Light_list.hpp"

// OpenMP-accelerated multi-threaded renderer
class ParallelRenderer : public Renderer {
public:
    ParallelRenderer();
    ParallelRenderer(const ParallelRenderer& other);
    ~ParallelRenderer() override = default;
    
    // Main render function (parallelized)
    void Render(const Scene& scene, Image& image) override;
    
private:
    // Ray color with lighting (thread-safe)
    Vector3 RayColor(const Ray& r, const hittable_list& world, const Light_list& lights, int depth);
};

// Alias for backward compatibility
using RendererOpenMP = ParallelRenderer;

#endif
