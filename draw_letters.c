#include <cairo.h>
#include <cairo-pdf.h>
#include <string.h>

int main() {
    int width = 200, height = 300;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // White background
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Set font properties
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 60);

    // Draw red letter 'A'
    cairo_set_source_rgb(cr, 1, 0, 0); // red
    cairo_move_to(cr, 50, 80);
    cairo_show_text(cr, "A");

    // Draw green letter 'B'
    cairo_set_source_rgb(cr, 0, 1, 0); // green
    cairo_move_to(cr, 50, 160);
    cairo_show_text(cr, "B");

    // Draw blue letter 'C'
    cairo_set_source_rgb(cr, 0, 0, 1); // blue
    cairo_move_to(cr, 50, 240);
    cairo_show_text(cr, "C");

    // Write to PNG
    cairo_surface_write_to_png(surface, "output.png");

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return 0;
}
