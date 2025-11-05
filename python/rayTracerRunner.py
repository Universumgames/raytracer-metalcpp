import os
from enum import Enum

raytracer_work_dir = "../cmake-build-debug"
timelog_file = "../timelog.csv"
raytraced_output_dir = "./output"


class RayTracerImplementation(Enum):
    SEQUENTIAL = "--sequential"
    SHADER = "--shader"
    MULTI_THREADED = "--multi-threaded"

    def by_c_class_name(class_name: str):
        mapping = {
            "SequentialRayTracer": RayTracerImplementation.SEQUENTIAL,
            "MetalRayTracer": RayTracerImplementation.SHADER,
            "CudaRayTracer": RayTracerImplementation.SHADER,
            "OpenMPRayTracer": RayTracerImplementation.MULTI_THREADED
        }
        return mapping.get(class_name, None)

    def to_c_class_name(self) -> list[str]:
        mapping = {
            RayTracerImplementation.SEQUENTIAL: ["SequentialRayTracer"],
            RayTracerImplementation.SHADER: ["MetalRayTracer", "CudaRayTracer"],
            RayTracerImplementation.MULTI_THREADED: ["OpenMPRayTracer"]
        }
        return mapping.get(self, [])


def get_scene_files():
    files = os.listdir(raytracer_work_dir + "/scene")
    scene_files = [os.path.join("scene", f) for f in files if f.endswith('.json')]
    return scene_files


def get_window_sizes():
    return [(800, 600), (1280, 720), (1920, 1080)]


def get_samples_per_pixel_options():
    return [1, 2, 4, 6]


def get_bounces_options():
    return [1, 2, 3, 4, 6]


### get the cross product of every combination of window size, samples per pixel, and bounces
def get_cross_product():
    sizes = get_window_sizes()
    samples = get_samples_per_pixel_options()
    bounces = get_bounces_options()
    cross_product = []
    for size in sizes:
        for sample in samples:
            for bounce in bounces:
                cross_product.append({
                    "size": size,
                    "samples_per_pixel": sample,
                    "bounces": bounce
                })
    return cross_product


def run_ray_tracer(scene_file: str, width: int, height: int, samples_per_pixel: int, bounces: int,
                   implementation: RayTracerImplementation = RayTracerImplementation.SEQUENTIAL):
    os.mkdir(raytraced_output_dir) if not os.path.exists(raytraced_output_dir) else None
    absolute_output_dir = os.path.abspath(raytraced_output_dir)
    scene_file_name = os.path.basename(scene_file)
    raytraced_output = os.path.join(absolute_output_dir, implementation.name + "_" + scene_file_name.replace('.json',
                                                                                                             f'_{width}x{height}_s{samples_per_pixel}_b{bounces}.png'))
    runCommand = "cd {} && ./Raytracer -s {} --window-size {} {} --samples {} --bounces {} -b {} --no-window -of {} {}".format(
        raytracer_work_dir,
        scene_file,
        width,
        height,
        samples_per_pixel,
        bounces,
        timelog_file,
        raytraced_output,
        implementation.value
    )

    print("Running configuration: Scene: {}, Size: {}x{}, Samples: {}, Bounces: {}, Implementation: {}".format(
        scene_file,
        width,
        height,
        samples_per_pixel,
        bounces,
        implementation.name
    ))
    os.system(runCommand)


if __name__ == "__main__":
    scenes = get_scene_files()
    for scene in scenes:
        print(f"Rendering scene: {scene}")
        for combination in get_cross_product():
            for implementation in RayTracerImplementation:
                run_ray_tracer(scene, combination["size"][0], combination["size"][1], combination["samples_per_pixel"],
                               combination["bounces"], implementation)
