#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../dependencies/scene/hpp/scene.hpp"
#include "../dependencies/utils/hpp/Image.hpp"
#include "../dependencies/utils/hpp/Vector3.hpp"

class Renderer {
public:
    Renderer();
    void Render(const Scene& scene, Image& image);

private:
    Vector3 RayColor(const Ray& r, const hittable_list& world, int depth);
    int max_depth;
    int samples_per_pixel;
};

#endif