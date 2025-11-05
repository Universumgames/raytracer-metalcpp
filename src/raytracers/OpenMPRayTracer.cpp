#include "OpenMPRayTracer.hpp"
#include "../timing.hpp"

#include <iostream>

namespace RayTracing {
    OpenMPRayTracer::OpenMPRayTracer(const Vec2u &windowSize, unsigned bounces, unsigned samplesPerPixel)
        : SequentialRayTracer(windowSize, bounces, samplesPerPixel) {
    }

    Image *OpenMPRayTracer::raytrace(Scene scene) {
        TIMING_START(prepping)
        auto *image = new Image(getWindowSize());
        scene.prepareRender();
        TIMING_END(prepping)
        TIMING_LOG(prepping, RaytracingTimer::Component::SCENE_LOADING, "prepping scene for raytracing")
        TIMING_START(rays)
        auto rays = calculateStartingRays(scene.camera);
        TIMING_END(rays)
        TIMING_LOG(rays, RaytracingTimer::Component::ENCODING, "calculating starting rays")
        std::cout << "[" << identifier() << "] Starting raytrace with "
                << getRayCount() << " rays, "
                << scene.objects.size() << " mesh objects (" << scene.getTriangleCount() << " triangles), "
                << scene.spheres.size() << " spheres and "
                << scene.lights.size() << " light sources"
                << std::endl;
        std::cout << "[" << identifier() << "]" << " Maximum nested bounding box depth: " << scene.getNestingDepth() <<
                std::endl;

        TIMING_START(tracing)
        long iteration = 0;
        double progress = 0.0;
#pragma omp parallel for schedule(dynamic)
        for (auto &ray: rays) {
            for (unsigned b = 0; b < getBounces(); b++) {
                HitInfo currentHit{.hit = false, .distance = INFINITY};
                Vec3 currentRotatedNormal;
                RGBf currentColor;

                // check collision with complex objects
                for (const auto object: scene.objects) {
                    auto localRay = ray.toLocalRay(object->transform);

                    if (!localRay.intersectsBoundingBox(object->boundingBox)) {
                        continue;
                    }

                    std::vector<int> indices = object->mesh->indices;
                    /*std::vector<NestedBoundingBox *> boxesToCheck;
                    boxesToCheck.push_back(object->nestedBoundingBox);
                    while (!boxesToCheck.empty()) {
                        auto nestedBox = boxesToCheck.back();
                        boxesToCheck.pop_back();
                        if (!nestedBox->indices.empty()) {
                            indices.insert(indices.end(), nestedBox->indices.begin(), nestedBox->indices.end());
                            continue;
                        }
                        if (nestedBox->left != nullptr && localRay.intersectsBoundingBox(*nestedBox->left)) {
                            boxesToCheck.push_back(nestedBox->left);
                        }
                        if (nestedBox->right != nullptr && localRay.intersectsBoundingBox(*nestedBox->right)) {
                            boxesToCheck.push_back(nestedBox->right);
                        }
                    }*/

                    if (indices.empty()) continue;

                    for (int i = 0; i < indices.size() / 3; i++) {
                        int *startIndex = &indices[i * 3];
                        Vec3 triangle[3] = {
                            object->mesh->vertices[startIndex[0]],
                            object->mesh->vertices[startIndex[1]],
                            object->mesh->vertices[startIndex[2]]
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
                    auto intersection = ray.intersectSphere(sphere->transform.getTranslation(), sphere->radius);
                    if (intersection.hit && intersection.distance < currentHit.distance) {
                        currentHit = intersection;
                        currentRotatedNormal = intersection.normal;
                        currentColor = sphere->color;
                    }
                }

                // check collision for light sources
                for (const auto &light: scene.lights) {
                    auto intersection = ray.intersectSphere(light->transform.getTranslation(), light->radius);
                    if (intersection.hit && intersection.distance < currentHit.distance) {
                        currentHit = intersection;
                        currentHit.isLight = true;
                        currentRotatedNormal = {};
                        currentColor = light->emittingColor;
                    }
                }

                if (currentHit.hit) {
                    // hacky way to get some shading without light sources
                    if (currentHit.isLight) {
                        ray.lightColor = currentColor;
                        b = getBounces(); // after ray intersects with light source, stop bouncing
                    } else {
                        ray.colors.emplace_back(currentColor);
                    }
                    ray.reflectAt(currentHit.hitPoint - ray.direction * 0.1f, currentRotatedNormal);
                    ray.totalDistance += currentHit.distance;
                } else {
                    b = getBounces(); // no hit, stop bouncing
                }
            }
            iteration++;
            progress = iteration / (static_cast<double>(rays.size()));
            if (iteration % 10000 == 0) {
                std::cout << "\r" << iteration << "/" << rays.size() << " rays traced (" << (progress * 100.0) << "%)"
                        << std::flush;
            }
        }
        std::cout << '\r';
        TIMING_END(tracing)
        TIMING_LOG(tracing, RaytracingTimer::Component::RAYTRACING, "tracing rays")

        TIMING_START(resolve)
        resolveRays(image, rays);
        TIMING_END(resolve)
        TIMING_LOG(resolve, RaytracingTimer::Component::DECODING, "resolving rays into image")

        return image;
    }
}