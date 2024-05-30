 
#include "image.h"

#include <string>
#include <vector>
#include <iostream>

#include "lodepng.h"

Image::Image() {

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
        this->data.push_back(new Pixel{image[i], image[i+1], image[i+2], image[i+3]});
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
