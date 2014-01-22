#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define ERROR(msg) do { \
    fprintf(stderr, "%s: %s\n", msg, strerror(errno)); \
    exit(-1); \
} while (0)

static void dump_to_pgm(void *data, struct fb_fix_screeninfo *fixinfo, struct fb_var_screeninfo *varinfo)
{
    int x;
    int y;
    int w;
    int h;
    int pgm;
    int stride;
    char *pixels;

    if ((pgm = open("fb0.pgm", O_RDWR | O_CREAT)) < 0) {
        ERROR("Open fb0.pgm error");
    }

    w = varinfo->xres;
    h = varinfo->yres;
    dprintf(pgm, "P5\n%u %u 255\n", w, h);

    for (y = 0; y < h; y++) {
        stride = (y + varinfo->yoffset) * fixinfo->line_length; 

        for (x = 0; x < w; x++) {
            pixels = (char*) (data + (stride + (x + varinfo->xoffset) * varinfo->bits_per_pixel / 8));
            write(pgm, pixels, sizeof(char));
        }
    }

    close(pgm);
}

int main(int argc, char *argv[])
{
    int fb0;
    int bufsiz;
    void *fbdata;
    struct fb_fix_screeninfo fixinfo;
    struct fb_var_screeninfo varinfo;

    if ((fb0 = open("/dev/fb0", O_RDWR)) < 0) {
        ERROR("Open /dev/fb0 error");
    }

    if (ioctl(fb0, FBIOGET_VSCREENINFO, &varinfo) < 0) {
        close(fb0);
        ERROR("Get variable screen info error");
    }

    varinfo.xres = 1024;
    varinfo.yres = 768;

    if (ioctl(fb0, FBIOPUT_VSCREENINFO, &varinfo) < 0) {
        close(fb0);
        ERROR("Get variable screen info error");
    }

    if (ioctl(fb0, FBIOGET_FSCREENINFO, &fixinfo) < 0) {
        close(fb0);
        ERROR("Get fixed screen info error");
    }

    fbdata = mmap(NULL, fixinfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb0, 0);
    if (((void*) -1) == fbdata) {
        close(fb0);
        ERROR("Map memory error");
    }

    dump_to_pgm(fbdata, &fixinfo, &varinfo);
    munmap(fbdata, fixinfo.smem_len);
    close(fb0);

    return 0;
}
