#include <metal_stdlib>
#include "shader_methods.hpp"

using namespace metal;

#define epsilon 1.192093E-07

Metal_Intersection intersectSphere(Metal_Ray ray, simd::float3 sphereCenter, float sphereRadius){
    float3 offsetRayOrigin = ray.origin - sphereCenter;

    float a = dot(ray.direction, ray.direction);
    float b = 2 * dot(offsetRayOrigin, ray.direction);
    float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        float dst = (-b - sqrt(discriminant)) / (2 * a);

        if (dst >= 0) {
            return {
                .hit = true,
                .hitPoint = ray.origin + (ray.direction * dst),
                .normal = normalize(ray.origin + (ray.direction * dst) - sphereCenter),
                .distance = dst
            };
        }
    }
    return {
        .hit = false,
        .distance = INFINITY
    };
}

Metal_Intersection intersectTriangle(Metal_LocalRay ray, float3 triangle[3], float3 customNormal) {
    float3 edgeAB = triangle[1] - triangle[0];
    float3 edgeAC = triangle[2] - triangle[0];
    float3 normal = cross(edgeAB, edgeAC);
    float3 ao = ray.origin - triangle[0];
    float3 dao = cross(ao, ray.direction);
    
    float det = -dot(ray.direction, normal);
    float invDet = 1.0f / det;
    
    float dst = dot(ao, normal) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1 - u - v;
    
    return {
        .hit = det > epsilon && dst >= epsilon && u >= 0 && v >= 0 && w >= 0,
        .hitPoint = ray.origin + (ray.direction * dst),
        .normal = customNormal,
        .distance = dst
    };
}

Metal_Intersection intersectTrianglesInBox(Metal_LocalRay ray, Metal_MeshRayTraceableObject meshObject, Metal_NestedBoundingBox boundingBox, device int* meshIndices, device float3* meshVertices, device float3* meshNormals, device Metal_NestedBoundingBox* boundingBoxes){
    if(!intersectsBoundingBox(ray, boundingBox))
        return {
            .hit = false,
            .distance = INFINITY
        };
    
    /// if bounding box is not a leaf node, check children
    if(boundingBox.indicesOffset == -1){
        Metal_Intersection left = {.hit = false, .distance = INFINITY};
        Metal_Intersection right = {.hit = false, .distance = INFINITY};
        if(boundingBox.childLeftIndex != -1 )
            /// recursive call required because no dynamic memory for a variable array can be allocated
            left = intersectTrianglesInBox(ray, meshObject, boundingBoxes[boundingBox.childLeftIndex], meshIndices, meshNormals, meshVertices, boundingBoxes);
        if(boundingBox.childRightIndex != -1)
            right = intersectTrianglesInBox(ray, meshObject, boundingBoxes[boundingBox.childRightIndex], meshIndices, meshNormals, meshVertices, boundingBoxes);
        if(left.distance < right.distance)
            return left;
        else
            return right;
    }
    
    /// bounding box is leaf node, check all triangles
    Metal_Intersection nearestIntersection = { .hit = false, .distance = INFINITY };
    int vertexOffset = meshObject.vertexOffset;
    for(unsigned i = 0; i < boundingBox.triangleCount; i++){
        int startIndicesIndex = boundingBox.indicesOffset + (i * 3);
        float3 triangle[3] = {
            meshVertices[vertexOffset + meshIndices[startIndicesIndex + 0]],
            meshVertices[vertexOffset + meshIndices[startIndicesIndex + 1]],
            meshVertices[vertexOffset + meshIndices[startIndicesIndex + 2]],
        };
        Metal_Intersection cur = intersectTriangle(ray, triangle, meshNormals[boundingBox.normalsOffset + i]);
        if(cur.hit && cur.distance < nearestIntersection.distance){
            nearestIntersection = cur;
        }
    }
    
    return nearestIntersection;
}

bool intersectsBoundingBox(Metal_LocalRay ray, Metal_NestedBoundingBox box) {
    float tmin = -INFINITY;
    float tmax = INFINITY;

    if(ray.origin.x >= box.min.x && ray.origin.x <= box.max.x &&
       ray.origin.y >= box.min.y && ray.origin.y <= box.max.y &&
       ray.origin.z >= box.min.z && ray.origin.z <= box.max.z) {
        return true;
    }

    for (int i = 0; i < 3; i++) {
        if (abs(ray.direction[i]) < epsilon) {
            if (ray.origin[i] < box.min[i] || ray.origin[i] > box.max[i]) {
                return false;
            }
        } else {
            float invD = 1.0f / ray.direction[i];
            float t0 = (box.min[i] - ray.origin[i]) * invD;
            float t1 = (box.max[i] - ray.origin[i]) * invD;
            if (invD < 0.0f) {
                float temp = t0;
                t0 = t1;
                t1 = temp;
            }
            tmin = max(tmin, t0);
            tmax = min(tmax, t1);
            if (tmax <= tmin) {
                return false;
            }
        }
    }
    return tmax >= tmin && tmin >= 0;
}

Metal_LocalRay toLocalRay(Metal_Ray ray, float4x4 inverseTransform, float4x4 inverseRotate, float4x4 inverseScale) {
    float4 origin4 = inverseTransform * float4(ray.origin, 1);
    float4 direction4 = inverseRotate * inverseScale * float4(ray.direction, 1);
    return {
        .origin = float3(origin4.x, origin4.y, origin4.z),
        .direction = float3(direction4.x, direction4.y, direction4.z)
    };
}

