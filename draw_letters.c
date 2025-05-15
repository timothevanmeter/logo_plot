#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <string.h>

// ------------------------------------------
#define WIDTH 800
#define HEIGHT 300
#define MAX_FONT_SIZE 60.0
#define INITIAL_X 10
#define INITIAL_Y HEIGHT

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

// ------------------------------------------
void drawLetter(graphPar **gP, nucl *N, float freq) {
  int offset = 0;
  // Calculate the font size based on frequency
  cairo_set_font_size((*gP)->cr, MAX_FONT_SIZE * freq);
  // Graphical parameters
  cairo_text_extents_t extents;
  cairo_text_extents ((*gP)->cr, N->L, &extents);
  cairo_set_source_rgb((*gP)->cr, N->rgb[0], N->rgb[1], N->rgb[2]);
  offset = (int)((*gP)->maxWidth - extents.width) / 2;
  cairo_move_to((*gP)->cr, (*gP)->xE + offset, (*gP)->yE);
  cairo_show_text((*gP)->cr, N->L);
  // Update the extent
  (*gP)->yE -= (int)extents.height;
  // printf("\n\tyE = %d |extents.height = %f\n", (*gP)->yE, extents.height);
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
  memcpy(A->rgb, (int[]){1,0,0}, sizeof A->rgb); // red
  nucl *T = malloc(sizeof(nucl));
  T->L = "T";
  memcpy(T->rgb, (int[]){0,1,0}, sizeof T->rgb); // green
  nucl *G = malloc(sizeof(nucl));
  G->L = "G";
  memcpy(G->rgb, (int[]){0,0,1}, sizeof G->rgb); // blue
  nucl *C = malloc(sizeof(nucl));
  C->L = "C";
  memcpy(C->rgb, (int[]){1,1,0}, sizeof C->rgb); // yellow
  
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
  graphP->maxWidth = extents.width;
  
  // ------------------------------------------
  // Draw red letter 'T'
  drawLetter(gP, T, 0.9);
  
  // ------------------------------------------
  // Draw blue letter 'C'
  drawLetter(gP, C, 0.5);
  
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
