#pragma once

#include <vector>
#include <string>
#include <functional>
#include <set>

namespace image {
    const std::set<std::string> FILTERS = {
                                        "median",
                                        "average"
                                        };
}

struct Pixel {
    int R;
    int G;
    int B;
    int A;
};

class Image {
    public:
        Image(const std::string& filter);
        ~Image();
        
        // load the image from file using lodepng
        bool load_image(std::string filepath);

        // write the image to file using lodepng
        bool save_image(std::string filepath);

        // apply filter to the image
        void filter(int filter_size);

    private:
        std::vector<Pixel*> data;
        std::vector<Pixel> unmodified_data;
        int width;
        int height;
        
        std::function<void(int)> filter_function;

        std::vector<Pixel*> get_pixels_under_mask(int filter_size, int pixel);

        void median_filter(int filter_size);
        void median_filter_thread(int filter_size, int thread_num);

        void average_filter(int filter_size);
        void average_filter_thread(int filter_size, int thread_num);

};
