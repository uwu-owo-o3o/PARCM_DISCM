#include "RTThread.h"

RTThread::RTThread(IExecutionEvent* onFinishedTask)
{
	this->onFinished = onFinishedTask;
}

void RTThread::OnStartTask()
{	
    this->Render();
	this->onFinished->OnFinishedExecution();
	delete this;
}

void RTThread::Render() {

    for (int j = start_line; j < end_line; j++) {
        std::clog << "\rScanlines remaining: " << (end_line - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, *world);
            }
            //write_color(std::cout, pixel_samples_scale * pixel_color);
            ppmConverter->setPixels(i, j, pixel_color.x(), pixel_color.y(), pixel_color.z(), samples_per_pixel);
        }
    }
}

ray RTThread::get_ray(int i, int j) {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j.

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
        + ((i + offset.x()) * pixel_delta_u)
        + ((j + offset.y()) * pixel_delta_v);

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 RTThread::sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

point3 RTThread::defocus_disk_sample() {
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

color RTThread::ray_color(const ray& r, int depth, const hittable& world) {
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;

    if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, depth - 1, world);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
