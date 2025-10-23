#include "SequentialRayTracer.hpp"

#include <iostream>

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
        auto rays = calculateStartingRays(scene.camera);

        for (auto& object : scene.objects) {
            object->updateBoundingBox();
            object->transform.update();
        }

        for (auto &ray: rays) {
            for (int b = 0; b < bounces; b++) {
                HitInfo currentHit{.hit = false, .distance = MAXFLOAT};
                Vec3 currentRotatedNormal;
                RGBf currentColor;

                // TODO switch from transforming every vertex to transforming the ray


                // check collision with complex objects
                for (const auto object: scene.objects) {
                    // TODO check for bounding box intersection
                    auto localRay = ray.toLocalRay(object->transform);
                    for (int i = 0; i < object->mesh->numTriangles; i++) {
                        int *startIndex = &object->mesh->indices[i * 3];
                        Vec3 triangle[3] = {
                            object->transform.getTransformedPosition(object->mesh->vertices[startIndex[0]]),
                            object->transform.getTransformedPosition(object->mesh->vertices[startIndex[1]]),
                            object->transform.getTransformedPosition(object->mesh->vertices[startIndex[2]])
                        };
                        auto intersection = localRay.intersectTriangle(triangle, object->mesh->normals[i]);
                        if (intersection.hit && intersection.distance < currentHit.distance) {
                            currentHit = intersection;
                            currentRotatedNormal = object->transform.getTransformedNormal(intersection.normal);
                            currentColor = object->color;
                        }
                    }
                }

                // check collision for spheres
                for (const auto sphere: scene.spheres) {
                    auto localRay = ray.toLocalRay(sphere->transform);
                    auto intersection = localRay.intersectSphere(sphere->transform.position, sphere->radius);
                    if (intersection.hit && intersection.distance < currentHit.distance) {
                        currentHit = intersection;
                        currentRotatedNormal = intersection.normal;
                        currentColor = sphere->color;
                    }
                }

                // check collision for light sources
                for (const auto& light : scene.lights) {
                    auto localRay = ray.toLocalRay(light->transform);
                    auto intersection = localRay.intersectSphere(light->transform.position, light->radius);
                    if (intersection.hit && intersection.distance < currentHit.distance) {
                        currentHit = intersection;
                        currentHit.isLight = true;
                        currentRotatedNormal = {};
                        currentColor = light->emittingColor;
                    }
                }

                if (currentHit.hit) {
                    // hacky way to get some shading without light sources
                    //auto dot = Vec3::dot(ray.direction, currentRotatedNormal);
                    //ray.colors.push_back((Vec4(currentColor) * (cos(dot))).asColor());
                    if (currentHit.isLight) {
                        ray.lightColor = currentColor;
                        b = bounces; // after ray intersects with light source, stop bouncing
                    }else {
                        ray.colors.emplace_back(currentColor);
                    }
                    ray.reflectAt(currentHit.hitPoint - ray.direction * 0.1f, currentRotatedNormal);
                    ray.totalDistance += currentHit.distance;
                }
            }
        }

        resolveRays(image, rays);

        return image;
    }

    Image *SequentialRayTracer::rayTest(Camera *camera) {
        auto *image = new Image(width, height);
        auto rays = calculateStartingRays(camera);

        for (auto &ray: rays) {
            auto dot = ray.direction.dot(Vec3::forward());
            dot = dot * dot * dot * dot;
            ray.colors.push_back(RGBf(dot, dot, dot, 1));
            ray.lightColor = RGBf(0, 0, 1, 1);
        }

        resolveRays(image, rays);

        return image;
    }
}
