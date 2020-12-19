#include "imdraw.hpp"

namespace sv {

static void draw_solid_rect(Mat& im, int dx1, int dy1, int dx2, int dy2, const Scalar color)
{
    int channels = im.get_channels();
    int linebytes = im.get_width() * channels;
    uchar* data = im.data.get();
    uchar b = static_cast<uchar>(color.get_v0());
    uchar g = static_cast<uchar>(color.get_v1());
    uchar r = static_cast<uchar>(color.get_v2());
    for (int h=dy1; h<=dy2; h++) {
        for (int w=dx1; w<=dx2; w++) {
            int idx = h*linebytes + w*channels;
            data[idx] = b;
            data[idx+1] = g;
            data[idx+2] = r;
        }
    }
}

void rectangle(Mat& im, const RectI& rect, const Scalar color, int thickness)
{
    int image_height = im.get_height();
    int image_width = im.get_width();
    int x1 = rect.x;
    int y1 = rect.y;
    int x2 = rect.x2();
    int y2 = rect.y2();
    
    if (x1>image_width-1 || y1>image_height-1 || x2<0 || y2<0 || x1>x2 || y1>y2) {
        return;
    }
    if (x1<0) x1 = 0;
    if (y1<0) y1 = 0;
    if (x2>image_width-1-thickness) x2 = image_width-thickness;
    if (y2>image_height-1-thickness) y2 = image_height-thickness;

    int dx1, dx2, dy1, dy2;
    int half_thick_small = thickness / 2;
    int half_thick_big = thickness - half_thick_small;

    // up horizontal line segment
    dy1 = y1 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = y1 + half_thick_big;
    if (dy2 >= image_height - 1) {
        dy2 = image_height - 1;
    }

    dx1 = x1 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = x2 + half_thick_big;
    if (dx2 >= image_width-1) {
        dx2 = image_width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);
    
    // down horizontal line
    dy1 = y2 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = y2 + half_thick_big;
    if (dy2 >= image_height - 1) {
        dy2 = image_height - 1;
    }

    dx1 = x1 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = x2 + half_thick_big;
    if (dx2 >= image_width - 1) {
        dx2 = image_width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // left vertical line
    dy1 = y1 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = y2 + half_thick_big;
    if (dy2 >= image_height - 1) {
        dy2 = image_height - 1;
    }
    dx1 = x1 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = x1 + half_thick_big;
    if (dx2 >= image_width - 1) {
        dx2 = image_width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);

    // right vertical line
    dy1 = y1 - half_thick_small;
    if (dy1 < 0) {
        dy1 = 0;
    }
    dy2 = y2 + half_thick_big;
    if (dy2 >= image_height - 1) {
        dy2 = image_height - 1;
    }

    dx1 = x2 - half_thick_small;
    if (dx1 < 0) {
        dx1 = 0;
    }
    dx2 = x2 + half_thick_big;
    if (dx2 >= image_width - 1) {
        dx2 = image_width - 1;
    }
    draw_solid_rect(im, dx1, dy1, dx2, dy2, color);
}

}
