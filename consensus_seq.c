#include <zlib.h>
#include <stdio.h>
#include "kseq.h"
// -----------------------------------------------------------

// This script takes as input a fasta multiple sequence alignment
// and outputs a csv file containing a matrix with in rows the
// position in the MSA, p, and in columns the four nucleotides, n,
// in order A, T, G, C. Each entry in the matrix is the frequency,
// f(p,n).

// It can also with some trivial editing print to standard output
// the substition rate for each position, and the consensus
// sequence of the MSA, with an additional parameter: the
// frequency threshold under which the position is labelled, N,
// that is not informed.

// -----------------------------------------------------------
// ------------- USAGE ------------- 
// COMPILATION: 
// gcc -Wall -pedantic -o cons.o consensus_seq.c 
// COMPILATION FOR PERFORMANCE ANALYSIS: 
// gcc -Wall -pedantic -o cons.o consensus_seq.c -g -pg 
// EXECUTION: 
// ./cons.o <alignment_file_fasta_format> <output_file_name> 
// OR
// ./cons.o <alignment_file_fasta_format> <output_file_name> <frequency_threshold> 
// If the cons.o is called without the frequency_threshold the default threshold is 0.5
// -----------------------------------------------------------

KSEQ_INIT(int, read)
#define MAX_SEQ_SIZE 300  
// -----------------------------------------------------------
// -----------------------------------------------------------
  typedef struct consensusSeq {
    int position;
    int counts[4];
    struct consensusSeq * next;
  } conseq;

conseq * init() {
  conseq * cs = malloc(sizeof(conseq));
  cs->next = NULL;
  cs->counts[0] = 0;
  cs->counts[1] = 0;
  cs->counts[2] = 0;
  cs->counts[3] = 0;
  return cs;
}
// -----------------------------------------------------------
void add(conseq ** cs, char nucl) {
  // placeholder
  conseq * pl = NULL;
  if((*cs)->next == NULL) { 
    conseq * temp = init();
    temp->position = (*cs)->position +1;
    // Add it to the chained list
    (*cs)->next = temp;
  }
  pl = (*cs)->next;  
  // Increment the count for the corresponding nucleotide
  switch(nucl) {
  case 'a':
    pl->counts[0]++;
    break;
  case 't':
    pl->counts[1]++;
    break;
  case 'g':
    pl->counts[2]++;
    break;
  case 'c':
    pl->counts[3]++;
    break;
  default:
    break;
  }
  // Move the cursor to the most recent position
  (*cs) = pl;
}
// -----------------------------------------------------------
int maxNucl(int * array, float threshold) {
  int sum = 0;
  int res = 0;
  for(int i = 0; i < 4; i++) {
    sum += array[i];
  }
  // Converting the counts to frequencies
  float freq[] = {0.0,0.0,0.0,0.0};
  for(int i = 0; i < 4; i++) {
    if(sum != 0) {
      freq[i] = (float)array[i] / (float)sum;
    }    
  }
  // Finding the index with max value
  float temp = freq[0];
  for(int i = 1; i < 4; i++) {
    if(temp != freq[i]) {
      if(temp < freq[i]){
	temp = freq[i];
	res = i;
      }
    } else {
      res = 4;
    }
  }
  // Threshold for the consensus sequence
  if(res != 4 && freq[res] <= threshold) {
    res = 4;
  }
  return res;
}
// -----------------------------------------------------------
float substitutionRate(int * array) {
  int sum = 0;
  float subRate = 0.0;
  for(int i = 0; i < 4; i++) {
    sum += array[i];
  }
  // Converting the counts to frequencies
  float freq[] = {0.0,0.0,0.0,0.0};
  for(int i = 0; i < 4; i++) {
    if(sum != 0) {
      freq[i] = (float)array[i] / (float)sum;
    }    
  }
  // Finding the index with max value
  float temp = freq[0];
  for(int i = 1; i < 4; i++) {
    if(temp < freq[i]){
      temp = freq[i];
    }
  }
  subRate = 1.0 - temp;
  return subRate;
}

// -----------------------------------------------------------
char getCons(conseq * cs, float threshold) {
  char max = 'N';
  int index = maxNucl(cs->counts, threshold);
  switch(index) {
  case 0:
    max = 'A';
    break;
  case 1:
    max = 'T';
    break;
  case 2:
    max = 'G';
    break;
  case 3:
    max = 'C';
    break;
  case 4:
    max = 'N';
    break;
  default:
    break;
  }
  return max;
}

// -----------------------------------------------------------
void printCons(conseq * head, float threshold) {
  conseq * temp = NULL;
  if(head == NULL) {
    exit(1);
  }
  temp = head->next;
  while(temp) {
    printf("%c", getCons(temp, threshold));
    temp = temp->next;
  }
}

// -----------------------------------------------------------
void printSubstitutionRate(conseq * head) {
  conseq * temp = NULL;
  if(head == NULL) {
    exit(1);
  }
  temp = head->next;
  int pos = 1;
  while(temp) {
    printf("%d %.2f\n", pos, substitutionRate(temp->counts));
    temp = temp->next;
    pos++;
  }
}

// -----------------------------------------------------------
void freqToOutput(conseq * head, char *outputFile) {
  FILE *output;
  output = fopen(outputFile, "w+");
  // fprintf(output, "position,A,T,G,C\n");
  int pos = 1;
  conseq * temp = NULL;
  if(head == NULL) {
    exit(1);
  }
  temp = head->next;
  while(temp) {
    // fprintf(output, "%d", pos);
    // Calculate frequencies
    int sum = 0;
    for(int i = 0; i < 4; i++) {
      sum += temp->counts[i];
    }
    float freq[] = {0.0,0.0,0.0,0.0};
    for(int i = 0; i < 4; i++) {
      if(sum != 0) {
	freq[i] = (float)temp->counts[i] / (float)sum;
      }
      // print to output
      if(i != 3) {
	fprintf(output, "%.2f,", freq[i]);
      } else {
	fprintf(output, "%.2f\n", freq[i]);
      }
    }
    // fprintf(output, "\n");
    // Next position
    temp = temp->next;
    pos++;
  }
  fclose(output);
}

// -----------------------------------------------------------
void freeConsSeq(conseq * head) {
  conseq * temp = NULL;
  temp = head;
  while(head) {
    temp = head;
    head = head->next;
    free(temp);
  }
}

// ###########################################################################

int main(int argc, char **argv)
{
  FILE *fp;
  kseq_t *seq;
  char lineSeq[MAX_SEQ_SIZE+1];
  int n = 0;
  float threshold;
  if(argc >= 4) {
    threshold = strtof(argv[3], NULL);
  } else {
    threshold = 0.5;
  }
  // printf("\n\t%.2f", threshold);
  memset( lineSeq, 0, MAX_SEQ_SIZE+1 );

  conseq * head = init();
  head->position = 0;
  conseq * cursor = head;
  conseq ** cs = &cursor;
  
  // printf("\n-----------------------------------------------------\n");
  fp = fopen(argv[1], "r");
  seq = kseq_init(fileno(fp));
	
  while (kseq_read(seq) >= 0) {
    n++;
    memcpy( lineSeq, seq->seq.s, MAX_SEQ_SIZE );
    for(int p = 0; p < MAX_SEQ_SIZE; p++) {
      if(lineSeq[p] == '\0') { break; }
      // printf("%c", lineSeq[p]);
      add(cs, lineSeq[p]);
    }
    // printf("\n");
    // if(n == 100) { break; }
    cursor = head;
  }
  // printf("\n");

  // Print the consensus sequence for a certain threshold value
  // printCons(head, threshold);
  // Print the substition rate for each position in MSA
  // printSubstitutionRate(head);
  
  // Print to file the frequencies of nucleotides for each position
  freqToOutput(head, argv[2]);
  // Freeing the memory of the linked list
  freeConsSeq(head);

  kseq_destroy(seq);
  fclose(fp);
  // printf("\n-----------------------------------------------------\n\n");
  return 0;
}
