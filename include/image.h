#pragma once

#include <vector>
#include <string>

struct Pixel {
    int R;
    int G;
    int B;
    int A;
};

class Image {
    public:
        Image();
        ~Image();
        
        // load the image from file using lodepng
        bool load_image(std::string filepath);

        // write the image to file using lodepng
        bool save_image(std::string filepath);

    private:
        std::vector<Pixel*> data;
        int width;
        int height;

};
