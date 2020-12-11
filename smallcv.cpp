#include "smallcv.h"

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


namespace sv {
    Mat::Mat(Shape3d shape)
    {
        height = shape.height;
        width = shape.width;
        channels = shape.channels;
        size_t size = height * width * channels * sizeof(unsigned char);
        data.reset(new uchar[size], [](uchar* p) {
            delete[] p;
        });
    }

    Mat::~Mat()
    {
        data.reset();
    }

    bool Mat::empty() const
    {
        return data == nullptr;
    }


    Mat imread(const char* image_path)
    {
        // TODO: check if file image_path exist
        int height;
        int width;
        int channels;
        unsigned char* raw_data = stbi_load(image_path, &width, &height, &channels, 0);
        assert(raw_data != NULL);
        assert(channels == 3); // TODO: support gray image and detect 4-channel RGBA image
        Shape3d shape;
        shape.height = static_cast<size_t>(height);
        shape.width = static_cast<size_t>(width);
        shape.channels = static_cast<size_t>(channels);
        Mat image(shape);
        uchar* image_data = image.data.get();
        size_t size = static_cast<size_t>(height * width * channels) * sizeof(unsigned char);
        memcpy(image_data, raw_data, size);
        free(raw_data);

        return image;
    }

    Mat imread(const std::string& image_path)
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
        uchar* data = image.data.get();
        assert(image.get_channels() == 3); // TODO: support gray image
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

    void imwrite(const std::string& save_path, const sv::Mat& image)
    {
        imwrite(save_path.c_str(), image);
    }

    void rgb_bgr_swap_inplace(sv::Mat& image)
    {
        size_t h = image.get_height();
        size_t w = image.get_width();
        size_t c = image.get_channels();
        assert(c == 3);
        uchar* data = image.data.get();
        for (size_t i = 0; i < h; i++) {
            for (size_t j = 0; j < w; j++) {
                size_t idx = i * w * 3 + j * 3;
                unsigned char t = data[idx];
                data[idx] = data[idx + 2];
                data[idx + 2] = t;
            }
        }
    }

    void line(Mat& image, Point2i pt1, Point2i pt2, const Scalar& color, int thickness)
    {
        
    }

    void line(Mat& image, Point2f pt1, Point2f pt2, const Scalar& color, int thickness)
    {
        Point2i ipt1(static_cast<int>(pt1.x), static_cast<int>(pt1.y));
        Point2i ipt2(static_cast<int>(pt2.x), static_cast<int>(pt2.y));
        line(image, ipt1, ipt2, color, thickness);
    }

    void circle(Mat& image, Point2i center, int radius, const Scalar& color, int thickness)
    {
        // TODO
    }

    void circle(Mat& image, Point2f center, int radius, const Scalar& color, int thickness)
    {
        Point2i icenter(static_cast<int>(center.x), static_cast<int>(center.y));
        circle(image, icenter, radius, color, thickness);
    }

    template<typename _Tp>
    Rect_<_Tp>::Rect_(): 
        x(0), y(0), width(0), height(0)
    {
    }

    template<typename _Tp>
    Rect_<_Tp>::Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height): 
        x(_x), y(_y), width(_width), height(_height)
    {
    }

    template<typename _Tp>
    Rect_<_Tp>::Rect_(const Rect_<_Tp>& r): 
        x(r.x), y(r.y), width(r.width), height(r.height)
    {
    }

    template<typename _Tp>
    Rect_<_Tp>::Rect_(Rect_<_Tp>&& r) noexcept: 
        x(std::move(r.x)), y(std::move(r.y)), width(std::move(r.width)), height(std::move(r.height))
    {
    }
    
    template<typename _Tp>
    Rect_<_Tp>& Rect_<_Tp>::operator= (const Rect_<_Tp>& r)
    {
        x = r.x;
        y = r.y;
        width = r.width;
        height = r.height;
        return *this;
    }

    template<typename _Tp>
    Rect_<_Tp>& Rect_<_Tp>::operator=(Rect_<_Tp>&& r) noexcept
    {
        x = std::move(r.x);
        y = std::move(r.y);
        width = std::move(r.width);
        height = std::move(r.height);
        return *this;
    }

    template<typename _Tp>
    _Tp Rect_<_Tp>::area() const
    {
        const _Tp result = width * height;

        // make sure result fits in the return value
        assert(!std::numeric_limits<_Tp>::is_integer
               || width == 0 || result / width == height);
        return result;
    }

}
