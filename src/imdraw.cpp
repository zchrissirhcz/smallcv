#include "smallcv.hpp"
#include <cmath>

namespace cv {

//----------------------------------------
// static function declarations
//----------------------------------------
static void draw_solid_rect(Mat& im, int dx1, int dy1, int dx2, int dy2, const Scalar color);
static void bressenhan_line(Mat& image, int x1, int y1, int x2, int y2, const Scalar& color);

//----------------------------------------
// function implementations
//----------------------------------------
void line(Mat& image, Point pt1, Point pt2, const Scalar& color, int thickness)
{
    for (int i = -thickness; i <= thickness; i++) {
        int x1 = pt1.x + i;
        int y1 = pt1.y + i;
        int x2 = pt2.x + i;
        int y2 = pt2.y + i;
        bressenhan_line(image, x1, y1, x2, y2, color);
    }
}

void bressenhan_line(Mat& image, int x1, int y1, int x2, int y2, const Scalar& color)
{
    uchar* data = image.data;
    int linebytes = image.cols * 3;
    int v0 = color.val[0];
    int v1 = color.val[1];
    int v2 = color.val[2];
    int dx = abs(x2 - x1),
        dy = abs(y2 - y1),
        yy = 0;

    if (dx < dy) {
        yy = 1;
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(dx, dy);
    }

    int ix = (x2 - x1) > 0 ? 1 : -1,
        iy = (y2 - y1) > 0 ? 1 : -1,
        cx = x1,
        cy = y1,
        n2dy = dy * 2,
        n2dydx = (dy - dx) * 2,
        d = dy * 2 - dx;

    if (yy) { // if angle between x axix and line segment is greater than 45 degrees
        while (cx != x2) {
            if (d < 0) {
                d += n2dy;
            }
            else {
                cy += iy;
                d += n2dydx;
            }
            //putpixel(img, cy, cx, c);
            data[cx * linebytes + cy * 3] = v0;
            data[cx * linebytes + cy * 3 + 1] = v1;
            data[cx * linebytes + cy * 3 + 2] = v2;
            cx += ix;
        }
    }
    else {  // if angle between x axix and line segment is less than 45 degrees
        while (cx != x2) {
            if (d < 0) {
                d += n2dy;
            }
            else {
                cy += iy;
                d += n2dydx;
            }
            //putpixel(img, cx, cy, c);
            data[cy * linebytes + cx * 3] = v0;
            data[cy * linebytes + cx * 3 + 1] = v1;
            data[cy * linebytes + cx * 3 + 2] = v2;
            cx += ix;
        }
    }
}

void naive_line(Mat & image, int x0, int y0, int x1, int y1, const Scalar & color)
{
    uchar* data = image.data;
    bool steep = false;
    // if dx<dy, then swap them. We assume dx>dy and iterater according to dx
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int idx = 0;
    int linebytes = image.cols * 3;
    const int v0 = color.val[0];
    const int v1 = color.val[1];
    const int v2 = color.val[2];
    for (int x = x0; x <= x1; x++) {
        float t = 1.0 * (x - x0) / (x1 - x0); // lambda
        int y = y0 * (1.f - t) + y1 * t;
        if (steep) {
            //image[h=x,w=y] = color
            data[x * linebytes + y * 3] = v0;
            data[x * linebytes + y * 3 + 1] = v1;
            data[x * linebytes + y * 3 + 2] = v2;
        }
        else {
            //image[h=y,w=x] = color
            data[y * linebytes + x * 3] = v0;
            data[y * linebytes + x * 3 + 1] = v1;
            data[y * linebytes + x * 3 + 2] = v2;
        }
    }
}

void line(Mat& image, Point2f pt1, Point2f pt2, const Scalar& color, int thickness)
{
    Point ipt1(static_cast<int>(pt1.x), static_cast<int>(pt1.y));
    Point ipt2(static_cast<int>(pt2.x), static_cast<int>(pt2.y));
    line(image, ipt1, ipt2, color, thickness);
}

void circle(Mat& image, Point center, int radius, const Scalar& color, int thickness)
{
    int x0 = center.x - radius;
    int y0 = center.y - radius;
    int x1 = center.x + radius;
    int y1 = center.y + radius;
    uchar* data = image.data;
    int linebytes = image.cols * 3;
    int v0 = color.val[0];
    int v1 = color.val[1];
    int v2 = color.val[2];
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            float dist = std::hypot(y - center.y, x - center.x);
            if (dist <= radius) {
                // data[h=y,w=x] = color
                data[y * linebytes + x * 3] = v0;
                data[y * linebytes + x * 3 + 1] = v1;
                data[y * linebytes + x * 3 + 2] = v2;
            }
        }
    }
}

void circle(Mat& image, Point2f center, int radius, const Scalar& color, int thickness)
{
    Point icenter(static_cast<int>(center.x), static_cast<int>(center.y));
    circle(image, icenter, radius, color, thickness);
}

static void draw_solid_rect(Mat& im, int dx1, int dy1, int dx2, int dy2, const Scalar color)
{
    int channels = im.channels();
    int linebytes = im.cols * channels;
    uchar* data = im.data;
    uchar b = static_cast<uchar>(color.val[0]);
    uchar g = static_cast<uchar>(color.val[1]);
    uchar r = static_cast<uchar>(color.val[2]);
    for (int h=dy1; h<=dy2; h++) {
        for (int w=dx1; w<=dx2; w++) {
            int idx = h*linebytes + w*channels;
            data[idx] = b;
            data[idx+1] = g;
            data[idx+2] = r;
        }
    }
}

void rectangle(Mat& im, const Rect& rect, const Scalar& color, int thickness)
{
    int image_height = im.rows;
    int image_width = im.cols;
    int x1 = rect.x;
    int y1 = rect.y;
    int x2 = x1 + rect.width - 1;
    int y2 = y1 + rect.height - 1;
    
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
