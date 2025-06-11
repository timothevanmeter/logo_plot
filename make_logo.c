#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <string.h>
#include "read_matrix.h"
// ------------------------------------------
// ------------------------------------------

// --------- USAGE ---------
// COMPILATION:
// gcc -o draw_letters.o draw_letters.c `pkg-config --cflags --libs cairo`
// EXECUTION:
// ./draw_letters.o <frequencies_matrix.csv> <image_output.png>

// ------------------------------------------
// ------------------------------------------
#define SCALE 10
#define HEIGHT 500
// The value of 1.368 was calculated as the multiplicative constant
// between the font size and its height in pixel on the image. It
// insures that the letters drawed occupy almost the entirety of the
// vertical space of the image.
#define MAX_FONT_SIZE HEIGHT * 1.368
// #define SPACE_SIZE 10.0
#define INITIAL_X 0
#define INITIAL_Y HEIGHT
#define SEQ_POSITION_WIDTH 100.0
// #define WIDTH 32600

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
void drawLetter(graphPar **gP, nucl *N, float freq) {
    cairo_text_extents_t extents;
    double font_size = MAX_FONT_SIZE * freq;

    // Set font size
    cairo_set_font_size((*gP)->cr, font_size);
    // Get text extents
    cairo_text_extents((*gP)->cr, N->L, &extents);
    double text_width = extents.width;
    double text_height = extents.height;
    // Compute Y scale to fit HEIGHT
    // double yScale = (HEIGHT / text_height) * freq;
    // Compute X scale to fit SEQ_POSITION_WIDTH
    double xScale = SEQ_POSITION_WIDTH / text_width;
    cairo_move_to((*gP)->cr, (*gP)->xE, (*gP)->yE);    
    // Save context state
    cairo_save((*gP)->cr);
    // Scale horizontally
    cairo_scale((*gP)->cr, xScale, 1.0);
    // Set color using float [0.0, 1.0]
    cairo_set_source_rgb((*gP)->cr, N->rgb[0], N->rgb[1], N->rgb[2]);
    // Draw the letter
    cairo_show_text((*gP)->cr, N->L);
    // Restore context to undo scale/translate
    cairo_restore((*gP)->cr);
    // Move Y up based on the font size (approximate stacking)
    (*gP)->yE -= (int)(text_height);
}

// ------------------------------------------
int main(int argc, char *argv[]) {
  // Get input file 
  const char * filename = argv[1];
  int sequenceLength = getlines(filename);
  int width = (sequenceLength-1) * SEQ_POSITION_WIDTH;
  
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

  // Storing all the nucleotides structures in an array
  nucl * nucleotides[] = {A,T,G,C};
  // ------------------------------------------
  cairo_text_extents_t extents;
  cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, HEIGHT);
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
  // Instantiating matrix iterator
  mIterator *mIt = init(filename);
  // Iterates over every row in the matrix
  // int sizeseq = 1;
  while(next(mIt)) {
    int i = 0;
    // Loops over the matrix's columns
    while(i < mIt->fields -1) {
      if(mIt->array[i] != 0.0) {
	// printf("%s:%.2f ", nucleotides[i]->L, mIt->array[i]);      
	drawLetter(gP, nucleotides[i], mIt->array[i]);
      }
      i++;
    }
    graphP->xE += SEQ_POSITION_WIDTH;
    graphP->yE = INITIAL_Y;
    // printf("\n");
  }

  cairo_scale((*gP)->cr, SCALE, SCALE);
  
  // ------------------------------------------
  // Write to PNG
  cairo_surface_write_to_png(surface, argv[2]);
  // Clean up
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  
  // ------------------------------------------
  return 0;
}
