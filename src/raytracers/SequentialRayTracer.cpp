#include "SequentialRayTracer.hpp"

#include "SFML/System/Vector2.hpp"

namespace RayTracing {
    SequentialRayTracer::SequentialRayTracer(unsigned width, unsigned height, unsigned bounces,
                                             unsigned samplesPerPixel) : RayTracer(
        width, height, bounces, samplesPerPixel) {
    }

    Image *SequentialRayTracer::uvTest() {
        auto *image = new Image(width, height);

        for (unsigned y = 0; y < height; y++) {
            for (unsigned x = 0; x < width; x++) {
                sf::Vector2 uv = {y / (double) height, x / (double) width};
                image->setPixel(x, y, {uv.x * 255, uv.y * 255, 0});
            }
        }

        return image;
    }


    Image *SequentialRayTracer::raytrace(Scene scene) {
        auto *image = new Image(width, height);
        auto rays = calculateStartingRays();

        for (auto &ray: rays) {
            for (int b = 0; b < bounces; b++) {
                Ray::HitInfo currentHit{.hit = false, .dst = MAXFLOAT};
                Vec3 currentRotatedNormal;
                RGBf currentColor;

                for (const auto &object: scene.objects) {
                    auto rotMat = object.getRotationMatrix();
                    for (int i = 0; i < object.mesh->numTriangles; i++) {
                        Vec3 *startingPoint = &object.mesh->points[i * 3];
                        Vec3 triangle[3] = {
                            object.getTranslatedRotatedPoint(rotMat, startingPoint[0]),
                            object.getTranslatedRotatedPoint(rotMat, startingPoint[1]),
                            object.getTranslatedRotatedPoint(rotMat, startingPoint[2])
                        };
                        auto intersection = ray.intersectTriangle(triangle, object.mesh->normals[i]);
                        if (intersection.hit && intersection.dst < currentHit.dst) {
                            currentHit = intersection;
                            currentRotatedNormal = RayTraceableObject::getRotatedNormal(rotMat, intersection.normal);
                            currentColor = object.color;
                        }
                    }
                }

                for (const auto &sphere: scene.spheres) {
                    auto intersection = ray.intersectSphere(sphere.position * -1, sphere.radius);
                    if (intersection.hit && intersection.dst < currentHit.dst) {
                        auto rotMat = sphere.getRotationMatrix();
                        currentHit = intersection;
                        currentRotatedNormal = RayTraceableObject::getRotatedNormal(rotMat, intersection.normal);
                        currentColor = sphere.color;
                    }
                }
                if (currentHit.hit) {
                    // hacky way to get some shading without light sources
                    //auto dot = Vec3::dot(ray.direction, currentRotatedNormal);
                    //ray.colors.push_back((Vec4(currentColor) * (cos(dot))).asColor());
                    ray.colors.emplace_back(currentColor);
                    ray.reflectAt(currentHit.hitPoint, currentRotatedNormal);
                    ray.dst += currentHit.dst;
                }
            }
        }

        resolveRays(image, rays);

        return image;
    }
}
