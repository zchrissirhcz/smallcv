#include "imshow.hpp"
#include "improc_private.hpp"

namespace cv {

static void fc_glfw_show_image(const char* title, const Mat& im);
static void fc_glfw_wait_key(int decay); // milliseconds
static void fc_glfw_set_window_resizable(bool b);

void imshow(const char* winname, const Mat& im)
{
    fc_glfw_show_image(winname, im);
}

void waitKey(int delay)
{
    fc_glfw_wait_key(delay);
}

} // namespace cv


//modified from: https://www.cnblogs.com/charlee44/p/5745127.html
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "fc_log.h"
//#define OFFSET 50

// compatible with older GLFW
#ifndef GLFW_TRUE
#define GLFW_TRUE 1
#endif

#ifndef GLFW_FALSE
#define GLFW_FALSE 0
#endif

namespace cv {
// struct definitions and function implmentations
//--------------------------------------------------
//GLFWwindow* window = NULL;
//GLuint texture;

typedef enum FcWindowFlag {
    FC_WINDOW_FIXED = 0,     // not OK
    FC_WINDOW_RESIZEABLE = 1 //OK
} FcWindowFlag;

typedef struct FcGlfwWindow FcGlfwWindow;
struct FcGlfwWindow {
    GLFWwindow* glfw_window;
    FcWindowFlag flag;
    GLuint texture;
    //const char* title;
    char title[256];
    FcGlfwWindow* next;
};

// global variables. start with `g_fc_` as prefix
static FcWindowFlag g_fc_window_flag = FC_WINDOW_FIXED; //on default, no resize
static FcGlfwWindow* g_fc_windows = NULL;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static FcGlfwWindow* fc_glfw_get_window(const char* title, int width, int height)
{
    FcGlfwWindow* win; // points to the windows that will show the specified image
    if (g_fc_windows == NULL) {
        win = (FcGlfwWindow*)malloc(sizeof(FcGlfwWindow));
        if (win == NULL)
            return NULL;
        win->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!win->glfw_window)
            return NULL;
        win->flag = g_fc_window_flag;
        //win->title = title;
        strcpy(win->title, title);
        win->next = NULL;
        g_fc_windows = win;
    }
    else {
        bool exist = false;
        FcGlfwWindow* cur_win = g_fc_windows;
        FcGlfwWindow* prev_win = NULL;
        while (exist != true && cur_win != NULL)
        {
            if (strcmp(cur_win->title, title) == 0) {
                exist = true;
                win = cur_win;
            }
            prev_win = cur_win;
            cur_win = cur_win->next;
        }
        if (!exist) {
            win = (FcGlfwWindow*)malloc(sizeof(FcGlfwWindow));
            if (win == NULL)
                return NULL;

            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            win->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
            if (!win->glfw_window)
                return NULL;
            win->flag = g_fc_window_flag;
            //win->title = title;
            strcpy(win->title, title);
            win->next = NULL;
            prev_win->next = win;
        }
    }
    return win;
}


//static GLuint create_texture(const char* im_pth)
static unsigned int create_texture(const Mat& im)
{
    int x, y;
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    Size size;
    size.height = im.rows;
    size.width = im.cols;
    Mat im_upsd(size, CV_8UC3);
    image_upside_down(im, im_upsd);

    //TODO: set alignment here
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, im->align);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im_upsd.cols, im_upsd.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, im_upsd.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

static void draw_quad(GLuint texture)
{
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.f, 1.f, 0.f, 1.f, 0.f, 1.f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBegin(GL_QUADS);

    glTexCoord2f(0.f, 0.f);
    glVertex2f(0.f, 0.f);

    glTexCoord2f(1.f, 0.f);
    glVertex2f(1.f, 0.f);

    glTexCoord2f(1.f, 1.f);
    glVertex2f(1.f, 1.f);

    glTexCoord2f(0.f, 1.f);
    glVertex2f(0.f, 1.f);

    glEnd();
}

static inline int fc_glfw_get_random(int a, int b)
{
    float t = rand() / (1.f + static_cast<float>(RAND_MAX));
    int res = t * (b - a) + a;
    return res;
}

void fc_glfw_show_image(const char* title, const Mat& im)
{
    int x, y, width;
    srand(time(0));
    // TODO: consider screen size
    int posX = fc_glfw_get_random(100, 400);
    int posY = fc_glfw_get_random(100, 400);

    //glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "glfwInit failed\n");
        exit(EXIT_FAILURE);
    }

    if (g_fc_window_flag == FC_WINDOW_FIXED) {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }

    FcGlfwWindow* win = fc_glfw_get_window(title, im.cols, im.rows);

    glfwMakeContextCurrent(win->glfw_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwSetWindowPos(win->glfw_window, posX, posY);
    glfwShowWindow(win->glfw_window);
    glfwSetKeyCallback(win->glfw_window, key_callback);

    win->texture = create_texture(im);

    //glfwGetWindowPos(win->glfw_window, &x, &y);
    //glfwGetWindowSize(win->glfw_window, &width, NULL);

    // Set drawing color for contexts
    glfwMakeContextCurrent(win->glfw_window);
    glColor3f(1.0f, 1.0f, 1.0f);

    //glfwMakeContextCurrent(win->glfw_window);
    glfwHideWindow(win->glfw_window);
}

void fc_glfw_set_window_resizable(bool b) {
    if (b) {
        g_fc_window_flag = FC_WINDOW_RESIZEABLE;
    }
    else {
        g_fc_window_flag = FC_WINDOW_FIXED;
    }
}

void fc_glfw_wait_key(int delay)
{
    long t_start = fc_gettime();

    // since we hide them in imshow(), we should show them now
    FcGlfwWindow* win = g_fc_windows;
    if (win == NULL) return;
    int win_cnt = 0;
    while (win != NULL) {
        if (!glfwWindowShouldClose(win->glfw_window)) {
            glfwShowWindow(win->glfw_window);
        }
        else {
            glfwHideWindow(win->glfw_window);
        }
        win = win->next;
        win_cnt++;
    }
    //printf("---- win_cnt is: %d\n", win_cnt);
    int closed_win_cnt = 0;

    if (delay <= 0) //wait for event infinitely
    {
        win = g_fc_windows;

        while (closed_win_cnt != win_cnt) {
            closed_win_cnt = 0;
            win = g_fc_windows;
            while (win != NULL) {
                if (!glfwWindowShouldClose(win->glfw_window))
                {
                    glfwMakeContextCurrent(win->glfw_window);
                    draw_quad(win->texture);
                    glfwSwapBuffers(win->glfw_window);
                }
                else {
                    closed_win_cnt++;
                    glfwHideWindow(win->glfw_window);
                }
                win = win->next;
                glfwPollEvents();
            }
        }

    }
    else // wait for event for the specified duration (delay)
    {
        long t_end = t_start + delay;
        for (; closed_win_cnt != win_cnt;) {
            fc_sleep(1);

            win = g_fc_windows;
            closed_win_cnt = 0;
            while (win != NULL) {
                if (!glfwWindowShouldClose(win->glfw_window))
                {
                    glfwMakeContextCurrent(win->glfw_window);
                    draw_quad(win->texture);
                    glfwSwapBuffers(win->glfw_window);
                }
                else {
                    glfwHideWindow(win->glfw_window);
                    closed_win_cnt++;
                }
                glfwWaitEvents();
                win = win->next;
            }
            long now = fc_gettime();
            if (now >= t_end) {
                return ; // no time rest
            }
        }
    }
    if (g_fc_windows) {
        free(g_fc_windows);
    }
    glfwTerminate();
}

/*
int fc_glfw_imshow_test_main(void)
{
    const char* im_pth;
    const char* title;

    im_pth = "F:/zhangzhuo/dev/libfc/imgs/op.png";
    Image im3 = fc_load_image(im_pth);
    title = "op";
    fc_glfw_show_image(title, &im3);

    im_pth = "F:/zhangzhuo/dev/libfc/imgs/Lena.png";
    Image im2 = fc_load_image(im_pth);
    title = "lena";
    fc_glfw_show_image(title, &im2);

    im_pth = "F:/zhangzhuo/dev/libfc/imgs/awesomeface.png";
    Image im4 = fc_load_image(im_pth);
    title = "awe";
    fc_glfw_show_image(title, &im4);

    im_pth = "F:/zhangzhuo/dev/libfc/imgs/fruits.jpg";
    Image im = fc_load_image(im_pth);
    title = "fruits";
    fc_glfw_show_image(title, &im);


    fc_glfw_wait_key(0);


    printf("hoho! you got it!\n");

    return 0;
}
*/

//int main() {
//  fc_glfw_imshow_test_main();
//
//  return 0;
//}

} // namespace cv

