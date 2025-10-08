#pragma once
#include "color.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "PPMConverter.h"
#include "IWorkerAction.h"
#include "IExecutionEvent.h"
#include "mutex"

class RTThread : public IWorkerAction
{	
	public:
        std::mutex converterMutex;

        RTThread(IExecutionEvent* onFinishedTask)
        {
            this->onFinished = onFinishedTask;
        }

        void OnStartTask()
        {
            this->Render();
            this->onFinished->OnFinishedExecution();
            delete this;
        }

        void Render() {
            std::cout << "starting read line: " << start_line << std::endl;
            

            for (int j = start_line; j < end_line; j++) {
                //std::cout << "curr read line: " << j << std::endl;
                //std::clog << "\rScanlines remaining: " << (end_line - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, *world);
                    }
                    //write_color(std::cout, pixel_samples_scale * pixel_color);
                    converterMutex.lock();
                    ppmConverter->setPixels(i, j, pixel_color.x(), pixel_color.y(), pixel_color.z(), samples_per_pixel);
                    converterMutex.unlock();
                }
            }
            std::cout << "Done reading." << std::endl;
        }

        ray get_ray(int i, int j) {
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

        vec3 sample_square() {
            // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() {
            // Returns a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int depth, const hittable& world) {
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

	public:
		IExecutionEvent* onFinished;

	public:
		int start_line = 0;
		int end_line = 0;
		int image_width = 0;
		int samples_per_pixel = 0;
		int max_depth = 0;

		double pixel_samples_scale;  
		double defocus_angle = 0;

		point3 center;         
		point3 pixel00_loc;    
		vec3   pixel_delta_u;  
		vec3   pixel_delta_v;  
		vec3   u, v, w;              
		vec3   defocus_disk_u;       
		vec3   defocus_disk_v;


		PPMConverter* ppmConverter;
		hittable* world;

};

