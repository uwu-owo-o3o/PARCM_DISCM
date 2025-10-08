#pragma once

#include "RTThread.h"
#include "IExecutionEvent.h"
#include "ThreadPool.h"

class camera : public IExecutionEvent {

private:
    double pixel_samples_scale;  // Color scale factor for a sum of pixel samples
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3   u, v, w;              // Camera frame basis vectors
    vec3   defocus_disk_u;       // Defocus disk horizontal radius
    vec3   defocus_disk_v;       // Defocus disk vertical radius

public:

    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width = 100;  // Rendered image width in pixel count
    int    image_height = 100;   // Rendered image height
    int    samples_per_pixel = 10;   // Count of random samples for each pixel
    int    max_depth = 10;   // Maximum number of ray bounces into scene
    double vfov = 90;  // Vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, 0);   // Point camera is looking from
    point3 lookat = point3(0, 0, -1);  // Point camera is looking at
    vec3   vup = vec3(0, 1, 0);     // Camera-relative "up" direction
    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    PPMConverter ppmConverter = PPMConverter();
    hittable* world;

    ThreadPool* pool;
    std::vector<bool> isFinished = std::vector<bool>();
    int done_num = 0;
    bool closeFlag = false;

    void render(hittable* world) {
        initialize();
        ppmConverter.initialize(this->image_width, this->image_height);
        this->world = world;

        for (int i = 0; i < isFinished.size(); i++) {
            this->isFinished[i] = false;
        }

        int workerCount = 8;

        this->isFinished.resize(workerCount);

        pool = new ThreadPool(workerCount);
        pool->StartScheduling();

        int linesPerWorker = this->image_height / workerCount;
        int remainingLines = this->image_height % workerCount;
        int start_line = 0;
        int end_line = 0;
        int extra = 0;
        for (int i = 0; i < workerCount; i++) {
            if (i < remainingLines) {
                extra = 1;
            }
            else {
                extra = 0;
            }
            end_line = start_line + linesPerWorker + extra;
            std::cout << "LINES PER WORKER: " << linesPerWorker << std::endl;
            std::cout << "STARTLINE: " << start_line << std::endl;
            std::cout << "ENDLINE: " << end_line << std::endl;
            this->makeRTThread(start_line, end_line);
            start_line = end_line;
        }

        while (!closeFlag) {

        }
    }

    void makeRTThread(int start_line, int end_line) {
        RTThread* thread = new RTThread(this);
        thread->start_line = start_line;
        thread->end_line = end_line;
        thread->image_width = this->image_width;
        thread->samples_per_pixel = this->samples_per_pixel;
        thread->max_depth = this->max_depth;

        thread->pixel_samples_scale = this->pixel_samples_scale;
        thread->defocus_angle = this->defocus_angle;

        thread->center = this->center;
        thread->pixel00_loc = this->pixel00_loc;
        thread->pixel_delta_u = this->pixel_delta_u;
        thread->pixel_delta_v = this->pixel_delta_v;
        thread->u = this->u;
        thread->v = this->v;
        thread->w = this->w;
        thread->defocus_disk_u = this->defocus_disk_u;
        thread->defocus_disk_v = this->defocus_disk_v;

        thread->ppmConverter = &ppmConverter;
        thread->world = world;

        pool->ScheduleTask(thread);
    }

    void OnFinishedExecution()
    {
        std::cout << "attempt to call on finish." << std::endl;
        this->isFinished[done_num] = true;
        this->done_num++;

        int count = 0;
        for (bool value : this->isFinished) {
            if (value) {
                count++;
            }
        }
        std::cout << "count: " << count << " size: " << this->isFinished.size() << std::endl;
        if (count == this->isFinished.size()) {
            std::cout << "Drawing..." << std::endl;
            ppmConverter.draw();
            closeFlag = true;
        }
    }

private:
    void initialize() {
        //image_height = int(image_width / aspect_ratio);
        //image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

};
