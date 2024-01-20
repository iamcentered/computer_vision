#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float sum = 0;
    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                sum += get_pixel(im, x, y, c);
            }
        }
    }

    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                set_pixel(im, x, y, c, get_pixel(im, x, y, c) / sum);
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    image filter = make_image(w, w, 1);

    for (int y = 0; y < filter.h; y++)
    {
        for (int x = 0; x < filter.w; x++)
        {
            set_pixel(filter, x, y, 0, 1.0 / (w * w));
        }
    }
    return filter;
}

float give_convol(image im, image filter, int x, int y, int c, int padding)
{
    float val = 0;

    for (int i = 0; i < filter.w; i++)
    {
        for (int j = 0; j < filter.h; j++)
        {
            val += get_pixel(filter, i, j, 0) * get_pixel(im, x - (padding - i), y - (padding - j), c);
        }
    }

    return val;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == 1 || filter.c == im.c);

    int padding = filter.w / 2;
    // int width = im.w + 2 * padding;
    // int height = im.h + 2 * padding;
    // image padded = make_image(width, height, im.c);
    image convolved;

    if (preserve)
        convolved = make_image(im.w, im.h, im.c);
    else
        convolved = make_image(im.w, im.h, 1);

    if (preserve)
    {
        // for (int c = 0; c < im.c; c++)
        // {
        //     for (int y = 0; y < height; y++)
        //     {
        //         for (int x = 0; x < width; x++)
        //         {
        //             if (x < padding || x >= width - padding || y < padding || y >= height - padding)
        //             {
        //                 set_pixel(padded, x, y, c, 0);
        //             }
        //             else
        //             {
        //                 set_pixel(padded, x, y, c, get_pixel(im, x - padding, y - padding, c));
        //             }
        //         }
        //     }
        // }

        for (int c = 0; c < im.c; c++)
        {
            for (int y = 0; y < im.h; y++)
            {
                for (int x = 0; x < im.w; x++)
                {
                    // if (x < padding || x >= width - padding || y < padding || y >= height - padding)
                    // {
                    //     continue;
                    // }
                    // else
                    {
                        // float new_px = give_convol(padded, filter, x, y, c, padding); // with padding image
                        // set_pixel(convolved, x-padding, y-padding, c, new_px);  // with padding image

                        float new_px = give_convol(im, filter, x, y, c, padding);
                        set_pixel(convolved, x, y, c, new_px);
                    }
                }
            }
        }
    }
    else
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                float new_px = 0;
                for (int c = 0; c < im.c; c++)
                {
                    // if (x < padding || x >= width - padding || y < padding || y >= height - padding)
                    // {
                    //     continue;
                    // }
                    // else
                    {
                        // float new_px = give_convol(padded, filter, x, y, c, padding); // with padding image
                        new_px += give_convol(im, filter, x, y, c, padding);
                    }
                }
                // set_pixel(convolved, x-padding, y-padding, c, new_px);  // with padding image
                set_pixel(convolved, x, y, 0, new_px);
            }
        }
    }

    // free_image(padded);
    return convolved;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);
    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);
    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    int size = (int)ceilf(sigma * 6);
    if (size % 2 == 0)
        size++;
    image filter = make_image(size, size, 1);

    float sum = 0;
    for (int y = 0; y < filter.h; y++)
    {
        for (int x = 0; x < filter.w; x++)
        {
            float u = x - (size - 1) / 2;
            float v = y - (size - 1) / 2;
            float value = expf(-(u * u + v * v) / (2 * sigma * sigma));
            value = value / (TWOPI * sigma * sigma);
            set_pixel(filter, x, y, 0, value);
            sum += value;
        }
    }

    l1_normalize(filter);

    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image result = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++)
    {
        for (int y = 0; y < a.h; y++)
        {
            for (int x = 0; x < a.w; x++)
            {
                float val_a = get_pixel(a, x, y, c);
                float val_b = get_pixel(b, x, y, c);
                set_pixel(result, x, y, c, val_a + val_b);
            }
        }
    }

    return result;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image result = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++)
    {
        for (int y = 0; y < a.h; y++)
        {
            for (int x = 0; x < a.w; x++)
            {
                float val_a = get_pixel(a, x, y, c);
                float val_b = get_pixel(b, x, y, c);
                set_pixel(result, x, y, c, val_a - val_b);
            }
        }
    }

    return result;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float min_val = INFINITY;
    float max_val = -INFINITY;

    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                float val = get_pixel(im, x, y, c);
                if (val < min_val)
                    min_val = val;
                if (val > max_val)
                    max_val = val;
            }
        }
    }

    float range = max_val - min_val;

    if (range == 0)
        return;

    for (int c = 0; c < im.c; c++)
    {
        for (int y = 0; y < im.h; y++)
        {
            for (int x = 0; x < im.w; x++)
            {
                float val = get_pixel(im, x, y, c);
                set_pixel(im, x, y, c, (val - min_val) / range);
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image *im_new = (image *)calloc(2, sizeof(image));
    im_new[0] = make_image(im.w, im.h, im.c);
    im_new[1] = make_image(im.w, im.h, im.c);
    image gx = make_gx_filter();
    image gy = make_gy_filter();

    image im1 = convolve_image(im, gx, 1);
    image im2 = convolve_image(im, gy, 1);

    for (int i = 0; i < im.w * im.h * im.c; i++)
    {
        im_new[0].data[i] = pow(pow(im1.data[i], 2) + pow(im2.data[i], 2), 0.5);
        im_new[1].data[i] = atan2(im2.data[i], im1.data[i]);
    }

    return im_new;
}

image colorize_sobel(image im)
{
    // TODO
    image *sobel = sobel_image(im);

    image mag = make_image(im.w, im.h, 1);
    image theta = make_image(im.w, im.h, 1);

    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float gx = get_pixel(sobel[0], x, y, 0);
            float gy = get_pixel(sobel[1], x, y, 0);
            float magnitude = sqrtf(gx * gx + gy * gy);
            float angle = atan2f(gy, gx);
            set_pixel(mag, x, y, 0, magnitude);
            set_pixel(theta, x, y, 0, angle);
        }
    }

    feature_normalize(mag);

    image colorized = make_image(im.w, im.h, 3);

    for (int y = 0; y < im.h; y++)
    {
        for (int x = 0; x < im.w; x++)
        {
            float magnitude = get_pixel(mag, x, y, 0);
            float angle = get_pixel(theta, x, y, 0);
            set_pixel(colorized, x, y, 0, angle / TWOPI);
            set_pixel(colorized, x, y, 1, magnitude);
            set_pixel(colorized, x, y, 2, magnitude);
        }
    }

    hsv_to_rgb(colorized);

    free_image(*sobel);
    free_image(*(sobel + 1));
    free(sobel);

    free_image(mag);
    free_image(theta);

    return colorized;
}
