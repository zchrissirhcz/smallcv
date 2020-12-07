#include <stdio.h>
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

typedef unsigned char uchar;

struct Shape2d {
    size_t height;
    size_t width;
};

struct Shape3d {
    size_t height;
    size_t width;
    size_t channels;
};


namespace sv {
    class Mat {
        // public member functions
        public:
            Mat(Shape3d shape, uchar* data);
            ~Mat();
            size_t get_height() const { return height; }
            size_t get_width() const { return width; }
            size_t get_channels() const { return channels; }

        // public data members
        public:
            uchar* data;

        // private data members
        private:
            bool data_raii; // if data created in ctor, data_raii should be true

            size_t height;
            size_t width;
            size_t channels;
    };

    Mat::Mat(Shape3d shape, uchar* _data)
    {
        height = shape.height;
        width = shape.width;
        channels = shape.channels;
        data = _data;
        data_raii = false;
    }

    Mat::~Mat()
    {
        if (data_raii) {
            free(data);
            data = NULL;
        }
    }

}

void rgb_bgr_swap_inplace(sv::Mat& image);
sv::Mat imread(const char* image_path);
sv::Mat imread(const std::string& image_path);
void imwrite(const char* save_path, const sv::Mat& image);
void imwrite(const std::string& save_path, const sv::Mat& image);

sv::Mat imread(const char* image_path)
{
    // TODO: check if file image_path exist
    int height;
    int width;
    int channels;
    unsigned char* data = stbi_load(image_path, &width, &height, &channels, 0);
    assert(data!=NULL);

    Shape3d shape;
    shape.height = static_cast<size_t>(height);
    shape.width = static_cast<size_t>(width);
    shape.channels = static_cast<size_t>(channels);
    sv::Mat image(shape, data);
    return image;
}

sv::Mat imread(const std::string& image_path)
{
    return imread(image_path.c_str());
}

void imwrite(const char* filename, const sv::Mat& image)
{
    if (strlen(filename) < 5) {
        fprintf(stderr, "filename too short\n");
        return;
    }
    const char* ext = filename + strlen(filename) - 4;
    int width = static_cast<int>(image.get_width());
    int height = static_cast<int>(image.get_height());
    assert(image.get_channels()==3); // TODO: support gray image
    int save_status = 0;
    if (0==strcmp(ext, ".jpg")) {
        int quality = 100;
        save_status = stbi_write_jpg(filename, width, height, 3, image.data, quality);
        assert(save_status!=0);
    }
    else if (0==strcmp(ext, ".png")) {
        int stride_in_bytes = width * 3; // TODO: Mat may use line alignment
        save_status = stbi_write_png(filename, width, height, 3, image.data, stride_in_bytes);
        assert(save_status!=0);
    }
    else if (0==strcmp(ext, ".bmp")) {
        save_status = stbi_write_bmp(filename, width, height, 3, image.data);
        assert(save_status!=0);
    }
    else {
        fprintf(stderr, "%s format is not supported yet!\n", ext);
    }
    // TODO: more types checking required
}

void imwrite(const std::string& save_path, const sv::Mat& image)
{
    imwrite(save_path.c_str(), image);
}

void rgb_bgr_swap_inplace(sv::Mat& image)
{
    size_t h = image.get_height();
    size_t w = image.get_width();
    size_t c = image.get_channels();
    assert(c==3);
    for (size_t i=0; i<h; i++) {
        for (size_t j=0; j<w; j++) {
            size_t idx = i*w*3 + j*3;
            unsigned char t = image.data[idx];
            image.data[idx] = image.data[idx+2];
            image.data[idx+2] = t;
        }
    }
}

int main() {
    std::string image_path = "mingren.jpg";
    sv::Mat image = imread(image_path);
    rgb_bgr_swap_inplace(image);
    imwrite("mingren_swap.jpg", image);
    imwrite("mingren_swap.bmp", image);

    return 0;
}
