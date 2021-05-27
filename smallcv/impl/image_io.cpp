#include "smallcv.hpp"
#include <stdlib.h>
#include <string.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace cv
{
    Mat imread(const char* image_path)
    {
        // TODO: check if file image_path exist
        int height;
        int width;
        int channels;
        unsigned char* raw_data = stbi_load(image_path, &width, &height, &channels, 0);
        assert(raw_data != NULL);
        assert(channels == 3); // TODO: support gray image and detect 4-channel RGBA image
        Size size;
        size.height = height;
        size.width = width;
        Mat image(size, CV_8UC3);
        uchar* image_data = image.data;
        size_t buf_size = static_cast<size_t>(height * width * channels) * sizeof(unsigned char);
        memcpy(image_data, raw_data, buf_size);
        free(raw_data);
        cvtColor(image, image, COLOR_RGB2BGR);

        return image;
    }

    Mat imread(const std::string& image_path)
    {
        return imread(image_path.c_str());
    }

    void imwrite(const char* filename, const cv::Mat& image)
    {
        if (strlen(filename) < 5) {
            fprintf(stderr, "filename too short\n");
            return;
        }
        const char* ext = filename + strlen(filename) - 4;
        int width = static_cast<int>(image.cols);
        int height = static_cast<int>(image.rows);
        
        assert(image.channels() == 3); // TODO: support gray image
        assert(image.data != NULL);

        //Size size = image.size();
        Size size;
        size.height = height;
        size.width = width;
        Mat shadow(size, CV_8UC3);
        memcpy(shadow.data, image.data, width*height*3);
        cvtColor(shadow, shadow, COLOR_BGR2RGB);

        uchar* data = shadow.data;

        if (0 == strcmp(ext, ".jpg")) {
            int quality = 100;
            stbi_write_jpg(filename, width, height, 3, data, quality);
        }
        else if (0 == strcmp(ext, ".png")) {
            int stride_in_bytes = width * 3; // TODO: Mat may use line alignment
            stbi_write_png(filename, width, height, 3, data, stride_in_bytes);
        }
        else if (0 == strcmp(ext, ".bmp")) {
            stbi_write_bmp(filename, width, height, 3, data);
        }
        else {
            fprintf(stderr, "%s format is not supported yet!\n", ext);
        }
        // TODO: more types checking required
    }

    void imwrite(const std::string& save_path, const cv::Mat& image)
    {
        imwrite(save_path.c_str(), image);
    }
}