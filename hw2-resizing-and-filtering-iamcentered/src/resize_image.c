#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    int nw_x = (int)roundf(x);
    int nw_y = (int)roundf(y);

    return get_pixel(im, nw_x, nw_y, c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image resized = make_image(w, h, im.c);

    float w_scale = (float)im.w / w;
    float h_scale = (float)im.h / h;

    for (int k = 0; k < im.c; ++k)
    {
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                float x = (i + 0.5) * w_scale - 0.5;
                float y = (j + 0.5) * h_scale - 0.5;

                float value = nn_interpolate(im, x, y, k);
                set_pixel(resized, i, j, k, value);
            }
        }
    }

    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    int x1 = (int)floorf(x);
    int x2 = x1 + 1;
    int y1 = (int)floorf(y);
    int y2 = y1 + 1;

    float q1 = (x2 - x) * (y2 - y);
    float q2 = (x - x1) * (y2 - y);
    float q3 = (x2 - x) * (y - y1);
    float q4 = (x - x1) * (y - y1);

    float v1 = get_pixel(im, x1, y1, c);
    float v2 = get_pixel(im, x2, y1, c);
    float v3 = get_pixel(im, x1, y2, c);
    float v4 = get_pixel(im, x2, y2, c);

    return q1 * v1 + q2 * v2 + q3 * v3 + q4 * v4;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image resized = make_image(w, h, im.c);

    float w_scale = (float)im.w / w;
    float h_scale = (float)im.h / h;

    for (int k = 0; k < im.c; ++k)
    {
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                float x = (i + 0.5) * w_scale - 0.5;
                float y = (j + 0.5) * h_scale - 0.5;

                float val = bilinear_interpolate(im, x, y, k);
                set_pixel(resized, i, j, k, val);
            }
        }
    }

    return resized;
}