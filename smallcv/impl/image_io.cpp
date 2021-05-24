#include "smallcv/api/smallcv.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include <assert.h>
#include <cmath>
#include <limits>

namespace cv {

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
        int width = image.cols;
        int height = image.rows;
        uchar* data = image.data;
        assert(image.channels() == 3); // TODO: support gray image
        assert(data != NULL);
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

    void rgb_bgr_swap_inplace(cv::Mat& image)
    {
        size_t h = image.rows;
        size_t w = image.cols;
        size_t c = image.channels();
        assert(c == 3);
        uchar* data = image.data;
        for (size_t i = 0; i < h; i++) {
            for (size_t j = 0; j < w; j++) {
                size_t idx = i * w * 3 + j * 3;
                unsigned char t = data[idx];
                data[idx] = data[idx + 2];
                data[idx + 2] = t;
            }
        }
    }
}

