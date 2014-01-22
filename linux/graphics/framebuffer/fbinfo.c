#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/ioctl.h>

#define ERROR(msg) do { \
    fprintf(stderr, "%s: %s\n", msg, strerror(errno)); \
    exit(-1); \
} while (0)

static void print_var_screen_info(struct fb_var_screeninfo *varinfo)
{
    printf("Variable screen Info\n");
    printf("====================\n");
    printf("Resolution: %dx%d\n", varinfo->xres, varinfo->yres);
    printf("Virtual Resolution: %dx%d\n", varinfo->xres_virtual, varinfo->yres_virtual);
    printf("Offset: %d %d\n", varinfo->xoffset, varinfo->yoffset);
    printf("Bits Per Pixel: %d\n", varinfo->bits_per_pixel);
    printf("Gray Scale: %d\n", varinfo->grayscale);
    printf("Non standard: %d\n", varinfo->nonstd);
    printf("Activate: %d\n", varinfo->activate);
    printf("Dimension: %dx%d\n", varinfo->width, varinfo->height);
    printf("Accelerator Flags: %d\n", varinfo->accel_flags);
    printf("Pixel Clock: %d\n", varinfo->pixclock);
    printf("Margin: %d %d %d %d\n", varinfo->upper_margin, varinfo->right_margin,
            varinfo->lower_margin, varinfo->left_margin);
    printf("Sync Length: %d %d\n", varinfo->hsync_len, varinfo->vsync_len);
    printf("Sync: %d\n", varinfo->sync);
    printf("VMode: %d\n", varinfo->vmode);
    printf("Rotate: %d\n", varinfo->rotate);
    printf("\n");
}

static void print_fix_screen_info(struct fb_fix_screeninfo *fixinfo)
{
    printf("Fixed Screen Info\n");
    printf("====================\n");
    printf("Id: %s\n", fixinfo->id);
    printf("Type: %d\n", fixinfo->type);
    printf("Visual: %d\n", fixinfo->visual);
    printf("X Pan Step: %d\n", fixinfo->xpanstep);
    printf("Y Pan Step: %d\n", fixinfo->ypanstep);
    printf("Y Wrap Step: %d\n", fixinfo->ywrapstep);
    printf("Line Length: %d\n", fixinfo->line_length);
    printf("Accelerator: %d\n", fixinfo->accel);
    printf("\n");
}

int main(int argc, char *argv[])
{
    int fb0;
    struct fb_fix_screeninfo fixinfo;
    struct fb_var_screeninfo varinfo;

    if ((fb0 = open("/dev/fb0", O_RDWR)) < 0) {
        ERROR("Open /dev/fb0 error");
    }

    if (ioctl(fb0, FBIOGET_VSCREENINFO, &varinfo) < 0) {
        close(fb0);
        ERROR("Get variable screen info error");
    }

    print_var_screen_info(&varinfo);

    if (ioctl(fb0, FBIOGET_FSCREENINFO, &fixinfo) < 0) {
        close(fb0);
        ERROR("Get fixed screen info error");
    }

    print_fix_screen_info(&fixinfo);

    return close(fb0);
}
