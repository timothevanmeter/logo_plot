#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "matrixIterator.h"
#include "getlines.h"

//--------------------------------------------------------
// USAGE:

// COMPILATION:
// gcc -Wall -pedantic -o op_variance.o one-pass_variance.c

// RUNNING THE EXECUTABLE:
// ./op_variance.o <input_file> <output_file>

// EXAMPLE:
// ./op_variance.o qscore.dat qscore_out.dat
//--------------------------------------------------------

// -------------------------------------------
int getFields(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("\n\tERROR: OPENING FILE.\n");
    exit(1);
  }
  char line[1024];
  int numFields = 0;
  fgets(line, sizeof(line), file);
  // Tokenize the line based on commas
  char *token = strtok(line, ",");
  numFields++;
  while (token != NULL) {
    token = strtok(NULL, ",");
    numFields++;
  }
  fclose(file);
  return numFields;
}

// -------------------------------------------
float ** getArray(const char *filename, int lineNumber) {
  int totalLines = getlines(filename);
  int fields = getFields(filename);
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("\n\tERROR: OPENING FILE.\n");
    exit(1);
  }
  int lineCount = 1;
  char line[1024];
  char *pend;
  float *array = malloc(sizeof(float) * (fields-2));
  float **a = &array;
  int i = 0;
  bool lineExist = false;
  // We do not use the header in the file
  fgets(line, sizeof(line), file);
  while(lineCount <= totalLines) {
    fgets(line, sizeof(line), file);
    if(lineNumber == lineCount) {
      lineExist = true;
      // Tokenize the line based on commas
      char *token = strtok(line, ",");
      // The first token is not used it is the position integer
      while (i < fields -2) {
	token = strtok(NULL, ",");
	array[i] = strtof(token, &pend);
	i++;
      }
      break;
    }
    lineCount++;
  }
  if(!lineExist) {
    printf("\n\tERROR: LINE NUMBER OUT OF BOUND.\n");
    exit(1);    
  }
  fclose(file);
  return a;
}

// -------------------------------------------
struct matrixIterator {
  float *array;
  const char *filename;
  int arrayNum;
  int fields;
  int lines;
};

// -------------------------------------------
mIterator * init(const char *filename) {
  mIterator *newStruct = malloc(sizeof(mIterator));
  newStruct->filename = filename;
  newStruct->arrayNum = 1;
  newStruct->fields = getFields(filename);
  newStruct->lines = getlines(filename);
  newStruct->array = (*getArray(filename, 1));
  return newStruct;
}

bool next(mIterator *mIt) {
  bool next = false;
  if(mIt->arrayNum +1 <= mIt->lines) {
    next = true;
    mIt->arrayNum++;
    mIt->array = (*getArray(mIt->filename, mIt->arrayNum));
  }
  return next;
}

void print(mIterator *mIt) {
  int i = 0;
  while(i < mIt->fields -2) {
    printf("%f\n", mIt->array[i]);
    i++;
  }
}

// -------------------------------------------
int main(int argc, char *argv[]) {

  const char *filename = argv[1];

  mIterator *mIt = init(filename);
  print(mIt);
  while(next(mIt)) {
    print(mIt);
  }
  
  return 0;
}
