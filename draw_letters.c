#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <string.h>

// ------------------------------------------
#define WIDTH 2000
#define HEIGHT 300
#define MAX_FONT_SIZE 400.0
#define SPACE_SIZE 10.0
#define INITIAL_X 10
#define INITIAL_Y HEIGHT

#define SEQ_POSITION_WIDTH 200.0

// Structure to associate letters with rgb colors
typedef struct Nucleotide {
  const char *L;
  int rgb[3];
} nucl;

// Structure to keep all the graphical parameters
//  in one spot
typedef struct graphicalParameters {
  cairo_t *cr;
  int xE;
  int yE;
  double maxWidth;
} graphPar;

float MAX(float a, float b) {
  float temp = 0.0;
  if(a > b) {
    temp = a;
  }
  if(a < b) {
    temp = b;
  }
  return temp;
}

// ------------------------------------------
// void drawLetter(graphPar **gP, nucl *N, float freq) {
//   // int offset = 0;
//   double xScaling = 1.0;
//   // Calculate the font size based on frequency
//   cairo_set_font_size((*gP)->cr, MAX_FONT_SIZE * freq);
//   // cairo_set_font_size((*gP)->cr, SEQ_POSITION_WIDTH * freq);
//   // Graphical parameters
//   cairo_text_extents_t extents;
//   cairo_text_extents ((*gP)->cr, N->L, &extents);
//   cairo_set_source_rgb((*gP)->cr, N->rgb[0], N->rgb[1], N->rgb[2]);
//   // offset = (int)((*gP)->maxWidth - extents.width) / 2;
//   // offset = (int)(SEQ_POSITION_WIDTH - extents.width) / 2;
//   xScaling = SEQ_POSITION_WIDTH / extents.width;
//   cairo_save((*gP)->cr);
//   if(freq != 1.0) {
//     cairo_translate((*gP)->cr, 0.0, (*gP)->yE);
//   } else {
//     cairo_translate((*gP)->cr, (*gP)->xE, (*gP)->yE);
//   }
//   cairo_scale((*gP)->cr, xScaling, 1.0);
//   cairo_show_text((*gP)->cr, N->L);
//   cairo_restore((*gP)->cr);
//   // Update the extent
//   (*gP)->yE -= (int)extents.height;
// }
void drawLetter(graphPar **gP, nucl *N, float freq) {
    cairo_text_extents_t extents;
    double font_size = MAX_FONT_SIZE * freq;

    // Set font size
    cairo_set_font_size((*gP)->cr, font_size);

    // Get text extents
    cairo_text_extents((*gP)->cr, N->L, &extents);
    double text_width = extents.width;
    double text_height = extents.height;

    // Compute X scale to fit SEQ_POSITION_WIDTH
    double xScale = SEQ_POSITION_WIDTH / text_width;

    cairo_move_to((*gP)->cr, (*gP)->xE, (*gP)->yE);
    
    // Save context state
    cairo_save((*gP)->cr);

    // Move to the current drawing point
    // cairo_translate((*gP)->cr, (*gP)->xE, (*gP)->yE);

    // Scale horizontally
    cairo_scale((*gP)->cr, xScale, 1.0);

    // Set color using float [0.0, 1.0]
    cairo_set_source_rgb((*gP)->cr, N->rgb[0], N->rgb[1], N->rgb[2]);

    // Draw the letter
    cairo_show_text((*gP)->cr, N->L);

    // Restore context to undo scale/translate
    cairo_restore((*gP)->cr);

    // Move Y up based on the font size (approximate stacking)
    (*gP)->yE -= (int)(font_size);
}

// ------------------------------------------
int main() {
  // Initialising the graphical parameters structure    
  graphPar *graphP = malloc(sizeof(graphPar));
  graphPar **gP = &graphP;
  graphP->maxWidth = 0.0;
  graphP->xE = INITIAL_X;
  graphP->yE = INITIAL_Y;

  // Initialising the different nucleotides and
  //   their associated colors
  nucl *A = malloc(sizeof(nucl));
  A->L = "A";
  memcpy(A->rgb, (int[]){255,0,0}, sizeof A->rgb); // red
  nucl *T = malloc(sizeof(nucl));
  T->L = "T";
  memcpy(T->rgb, (int[]){0,255,0}, sizeof T->rgb); // green
  nucl *G = malloc(sizeof(nucl));
  G->L = "G";
  memcpy(G->rgb, (int[]){0,0,255}, sizeof G->rgb); // blue
  nucl *C = malloc(sizeof(nucl));
  C->L = "C";
  memcpy(C->rgb, (int[]){255,255,0}, sizeof C->rgb); // yellow
  
  // ------------------------------------------
  cairo_text_extents_t extents;
  cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
  cairo_t *cr = cairo_create(surface);
  graphP->cr = cr;
  // ------------------------------------------
  // Gray background
  cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
  cairo_paint(cr);
  // Set font properties
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, MAX_FONT_SIZE);
  // The letter A has the largest width, so we use it
  //   as the max reference.
  cairo_text_extents (cr, A->L, &extents);
  // printf("\n%f\n", extents.width);
  graphP->maxWidth = extents.width;
  
  // ------------------------------------------
  // Draw letters
  drawLetter(gP, A, 1.0);

  graphP->xE += graphP->maxWidth;
  graphP->yE = INITIAL_Y;

  // cairo_save((*gP)->cr);
  
  drawLetter(gP, A, 0.5);
  drawLetter(gP, G, 0.5);

  // float temp = MAX(xx, yy);

  // cairo_scale((*gP)->cr, temp, 1.0);
  // cairo_show_text((*gP)->cr, A->L);
  // cairo_show_text((*gP)->cr, G->L);
  // cairo_restore((*gP)->cr);
  // graphP->xE += graphP->maxWidth;
  // graphP->yE = INITIAL_Y;  
  // drawLetter(gP, A, 1.0);
  
  // drawLetter(gP, C, 0.5);
  // drawLetter(gP, G, 0.25);
  // drawLetter(gP, T, 0.5);
  
  // ------------------------------------------
  // Write to PNG
  cairo_surface_write_to_png(surface, "output.png");
  // Clean up
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  
  // ------------------------------------------
  return 0;
}

// gcc -o draw_letters.o draw_letters.c `pkg-config --cflags --libs cairo` ; ./draw_letters.o ; eog output.png 
