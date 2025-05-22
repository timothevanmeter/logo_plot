CC 	= gcc
CS 	= gcc -c
CFLAGS 	= -Wall -pedantic
CFLAGS_CAIRO = `pkg-config --cflags --libs cairo`
RM 	= rm

default: all

all: compile-consensus compile-logo run

compile-consensus: 
	$(CC) $(CFLAGS) -o consensus_seq.o consensus_seq.c

compile-logo: 
	$(CC) $(CFLAGS) -o make_logo.o make_logo.c $(CFLAGS_CAIRO)

run:
	./consensus_seq.o MSA.fa test_freq_mat.csv
	./make_logo.o test_freq_mat.csv logo_plot.png

clean:
	$(RM) *.o
	$(RM) *.csv
	$(RM) *.png
