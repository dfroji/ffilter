 
#include "image.h"

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <algorithm>
#include <cmath>

#include "lodepng.h"

Image::Image(const std::string& filter) {
    if (filter == "") {
        this->filter_function = [](int size) {};
    }
    else if (filter == "median") {
        this->filter_function = std::bind(&Image::median_filter, this, std::placeholders::_1);
    }
    else if (filter == "average") {
        this->filter_function = std::bind(&Image::average_filter, this, std::placeholders::_1);
    }
}

Image::~Image() {
    for (Pixel* pixel : this->data) {
        delete pixel;
    }
}

bool Image::load_image(std::string filepath) {
    const char* cfilepath = filepath.c_str();

    std::vector<unsigned char> image;
    unsigned width, height;
    
    // try to decode the given file
    unsigned error = lodepng::decode(image, width, height, cfilepath);

    // if decoding fails, show error and return false
    if (error) {
        std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return false;
    }

    // create Pixels from the decoded file and push their pointers to this->data
    for (int i = 0; i < image.size(); i += 4) {
        Pixel* pixel = new Pixel{image[i], image[i+1], image[i+2], image[i+3]};
        this->data.push_back(pixel);
        this->unmodified_data.push_back(*pixel);
    }

    this->width = width;
    this->height = height;

    return true;
}

bool Image::save_image(std::string filepath) {
    const char* cfilepath = filepath.c_str();

    std::vector<unsigned char> image;

    // push the data to image
    for (Pixel* pixel : this->data) {
        image.push_back(pixel->R);
        image.push_back(pixel->G);
        image.push_back(pixel->B);
        image.push_back(pixel->A);
    }

    // try to encode the image
    unsigned error = lodepng::encode(cfilepath, image, this->width, this->height);

    // if encoding fails, show error and return false
    if (error) {
        std::cout << "Encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return false;
    }

    return true;
}

void Image::filter(int filter_size) {
    this->filter_function(filter_size);
}

  ///////////////
 /// PRIVATE ///
///////////////

std::vector<Pixel*> Image::get_pixels_under_mask(int filter_size, int pixel) {
    std::vector<Pixel*> pixels;

    // iterate through every pixel under the {filter_size}x{filter_size} mask
    for (int i = -(filter_size / 2); i <= filter_size / 2; i++) {
        for (int j = -(filter_size / 2); j <= filter_size / 2; j++) {

            // select the current pixel under the mask
            int pixel_under_mask = pixel + i * this->width + j;

            // check if the current pixel under the mask is within the image's bounds
            // add the pixel to pixels if it is
            if (pixel_under_mask - std::abs(i) * this->width >= 0 
                && pixel_under_mask % this->width < this->width) 
                {
                pixels.push_back(&this->unmodified_data[pixel_under_mask]);
            }
        }
    }

    return pixels;
}

void Image::median_filter(int filter_size) {
    std::vector<std::thread> threads;

    // create threads the number of which is the image's height
    // run median_filter_thread() on each thread
    for (int i = 0; i < this->height; i++) {
        threads.push_back(std::thread(
            std::bind(&Image::median_filter_thread, this, filter_size, i)
        ));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void Image::median_filter_thread(int filter_size, int thread_num) {
    // for every vertical pixel get the valid pixels under the mask
    // sort each color channel of the pixels under the mask and choose the medians to be used in this->data 
    for (int pixel = thread_num * this->width; pixel < thread_num * this->width + this->width; pixel++) {
        std::vector<Pixel*> pixels_under_mask = get_pixels_under_mask(filter_size, pixel);
        
        std::vector<int> R, G, B, A;

        for (Pixel* pixel : pixels_under_mask) {
            R.push_back(pixel->R);
            G.push_back(pixel->G);
            B.push_back(pixel->B);
            A.push_back(pixel->A);
        }

        std::sort(R.begin(), R.end());
        std::sort(G.begin(), G.end());
        std::sort(B.begin(), B.end());
        std::sort(A.begin(), A.end());

        this->data[pixel]->R = R[R.size() / 2];
        this->data[pixel]->G = G[G.size() / 2];
        this->data[pixel]->B = B[B.size() / 2];
        this->data[pixel]->A = A[A.size() / 2];
    }
}

void Image::average_filter(int filter_size) {
    std::vector<std::thread> threads;

    // create threads
    // run average_filter_thread on them
    for (int i = 0; i < this->height; i++) {
        threads.push_back(std::thread(
            std::bind(&Image::average_filter_thread, this, filter_size, i)
        ));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void Image::average_filter_thread(int filter_size, int thread_num) {
    for (int pixel = thread_num * this->width; pixel < thread_num * this->width + this->width; pixel++) {
        std::vector<Pixel*> pixels_under_mask = get_pixels_under_mask(filter_size, pixel);

        int R = 0;
        int G = 0; 
        int B = 0;
        int A = 0;

        for (Pixel* pixel : pixels_under_mask) {
            R += pixel->R;
            G += pixel->G;
            B += pixel->B;
            A += pixel->A;
        }

        R /= filter_size * filter_size;
        G /= filter_size * filter_size;
        B /= filter_size * filter_size;
        A /= filter_size * filter_size;

        this->data[pixel]->R = R;
        this->data[pixel]->G = G;
        this->data[pixel]->B = B;
        this->data[pixel]->A = A;
        
    }
}
