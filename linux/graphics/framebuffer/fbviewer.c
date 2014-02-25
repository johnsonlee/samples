#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <gtk/gtk.h>

#define FRAMEBUFFER "/dev/fb0"

#define ERROR(msg, ...) do { \
    fprintf(stderr, msg, ##__VA_ARGS__); \
    fprintf(stderr, ": %s\n", strerror(errno)); \
    exit(-1); \
} while (0)

static int fb0;
static int running;

static const char* get_default_framebuffer()
{
    const char *fb = getenv("FRAMEBUFFER");

    if (NULL != fb && fb != '\0')
        return fb;

    return FRAMEBUFFER;
}

static void get_screen_info(struct fb_var_screeninfo *var, struct fb_fix_screeninfo *fix)
{
    if (ioctl(fb0, FBIOGET_VSCREENINFO, var) < 0) {
        ERROR("Get variable screen info error");
    }

    if (ioctl(fb0, FBIOGET_FSCREENINFO, fix) < 0) {
        ERROR("Get fixed screen info error");
    }
}

static void* draw_framebuffer_data(GtkWidget *widget)
{
    char *pixels;
    struct fb_fix_screeninfo fix;
    struct fb_var_screeninfo var;

    get_screen_info(&var, &fix);
    gtk_widget_set_size_request(widget, var.xres, var.yres);
    pixels = mmap(NULL, fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb0, 0);

    switch (var.bits_per_pixel) {
    case 24:
        gdk_draw_rgb_image(widget->window,
                widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                var.xoffset, var.yoffset, var.xres, var.yres,
                GDK_RGB_DITHER_NONE, pixels, fix.line_length);
    case 32:
        gdk_draw_rgb_32_image(widget->window,
                widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                var.xoffset, var.yoffset, var.xres, var.yres,
                GDK_RGB_DITHER_NONE, pixels, fix.line_length);
        break;
    }

    munmap(pixels, fix.smem_len);
}

static gboolean on_canvas_expose(GtkWidget *widget, GdkEventExpose* event, gpointer data)
{
    draw_framebuffer_data(widget);
    return TRUE;
}

static void on_window_destroy(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    running = 0;
    g_thread_join(*((GThread**) data));
    gtk_main_quit();
}

static void* thread_queue_draw_framebuffer(GtkWidget *widget)
{
    int width, height;

    while (running) {
        gtk_widget_get_size_request(widget, &width, &height);
        gtk_widget_queue_draw_area(widget, 0, 0, width, height);
        g_usleep(62500);
    }

    return NULL;
}

static GtkWidget* create_viewer()
{
    GtkWidget *viewer;
    GtkWidget *hpanel;
    GtkWidget *canvas;
    GThread   *worker;

    // canvas to draw framebuffer
    canvas = gtk_drawing_area_new();
    g_signal_connect(canvas, "expose-event", G_CALLBACK(on_canvas_expose), NULL);

    // root container
    hpanel = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hpanel), canvas, TRUE, TRUE, 0);

    // window
    viewer = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_add(GTK_CONTAINER(viewer), hpanel);
    gtk_window_set_default_size(GTK_WINDOW(viewer), 640, 480);
    gtk_window_set_title(GTK_WINDOW(viewer), "Framebuffer Viewer");
    g_signal_connect(viewer, "delete-event", G_CALLBACK(on_window_destroy), &worker);

    // paint thread worker
    running = 1;
    worker = g_thread_new("Painter", (GThreadFunc) thread_queue_draw_framebuffer, canvas);

    return viewer;
}

int main(int argc, char *argv[])
{
    const char *dev = get_default_framebuffer();

    if ((fb0 = open(dev, O_RDWR)) < 0) {
        ERROR("Open %s error", dev);
    }

    gtk_init(&argc, &argv);
    gtk_widget_show_all(create_viewer());
    gtk_main();
    close(fb0);

    return 0;
}
