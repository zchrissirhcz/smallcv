#include "smallcv.hpp"
#include <stdlib.h>
#include <string.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

cv::Mat cv::imread(const char* image_path)
{
    // TODO: check if file image_path exist
    int height;
    int width;
    int channels;
    unsigned char* raw_data = stbi_load(image_path, &width, &height, &channels, 0);
    assert(raw_data != NULL);
    // TODO: detect 4-channel RGBA image
    Size size(width, height);
    Mat image(size, CV_8UC(channels));
    uchar* image_data = image.data;
    size_t buf_size = static_cast<size_t>(height * width * channels) * sizeof(unsigned char);
    memcpy(image_data, raw_data, buf_size);
    free(raw_data);
    if (channels == 3)
    {
        cvtColor(image, image, COLOR_RGB2BGR);
    }

    return image;
}

cv::Mat cv::imread(const std::string& image_path)
{
    return imread(image_path.c_str());
}

void cv::imwrite(const char* filename, const cv::Mat& image)
{
    if (strlen(filename) < 5) {
        fprintf(stderr, "filename too short\n");
        return;
    }
    const char* ext = filename + strlen(filename) - 4;
    int width = static_cast<int>(image.cols);
    int height = static_cast<int>(image.rows);
    const int comp = image.channels();
    
    assert(image.data != NULL);

    Size size = image.size();
    int type;
    const int channels = image.channels();
    switch (channels)
    {
    case 1:
        type = CV_8UC1;
        break;
    case 3:
        type = CV_8UC3;
        break;
    default:
        fprintf(stderr, "%s only support 1 or 3 channels\n", __FUNCTION__);
        abort();
    }
    Mat shadow;
    if (channels == 1)
    {
        shadow = image;
    }
    else
    {
        shadow.create(size, type);
        memcpy(shadow.data, image.data, width*height*channels);
        cvtColor(shadow, shadow, COLOR_BGR2RGB);
    }

    uchar* data = shadow.data;

    if (0 == strcmp(ext, ".jpg")) {
        int quality = 100;
        stbi_write_jpg(filename, width, height, comp, data, quality);
    }
    else if (0 == strcmp(ext, ".png")) {
        int stride_in_bytes = width * comp; // TODO: Mat may use line alignment
        stbi_write_png(filename, width, height, comp, data, stride_in_bytes);
    }
    else if (0 == strcmp(ext, ".bmp")) {
        stbi_write_bmp(filename, width, height, comp, data);
    }
    else {
        fprintf(stderr, "%s format is not supported yet!\n", ext);
    }
    // TODO: more types checking required
}

void cv::imwrite(const std::string& save_path, const cv::Mat& image)
{
    imwrite(save_path.c_str(), image);
}