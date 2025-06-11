#!/bin/bash


subsample_size=1000

# # Filtering the fastq file to keep only sequences of very good quality
# vsearch --fastq_filter $1 --fastq_maxee_rate $threshold --fastaout ${1/.fastq/_er}"${threshold}.fa"

# # Subsampling the fastq file
# ~/scripts/subsampling_seq.o $1 ${1/.fastq/_sub}${subsample_size}".fastq" $subsample_size

# # Remove the line breaks inside the sequences
# awk 'NR==1{print $0;next}$0~/^>/{printf "\n%s\n", $0} $0!~/^>/{printf "%s", $0}END{printf "\n"}' 77_S1_R2_001_er0.001.fa > temp.fa

# # Reverse complement all of the sequences with a pattern:
# # this pattern has to be chosen by hand

# # (1) First select all the sequences to reverse complement
# awk '$0~/^>/{header=$0}$0~/^T/{printf "%s\n%s\n", header, $0}' temp.fa > revcomp.fa

# # (2) Select all the forward sequences to keep separate
# awk '$0~/^>/{header=$0}$0~/^A/{printf "%s\n%s\n", header, $0}' temp.fa > temp-for.fa

# # (3) Reverse complement them one by one
# awk '
# function revcomp(arg) {
#     o = ""
#     for(i = length(arg); i > 0; i--)
#         o = o c[substr(arg, i, 1)]
#     return(o)
# }
# BEGIN {c["A"] = "T"; c["C"] = "G"; c["G"] = "C"; c["T"] = "A"}
# # fasta header
# $0~/^>/{
#     print $0
# }
# # reverse complementing the sequences
# $0!~/^>/{
#     print revcomp($0)
# }
# ' revcomp.fa > temp-rvcp.fa

# # Merge the two files together
# cat temp-for.fa > temp_merged.fa
# cat temp-rvcp.fa >> temp_merged.fa

# # Create a MSA of the subsampled sequences
# mafft --thread 24 --maxiterate 100 temp.fa > ${1/.fastq/_er}"${threshold}.align"

# # Remove the line breaks inside the sequences
# awk 'NR==1{print $0;next}$0~/^>/{printf "\n%s\n", $0} $0!~/^>/{printf "%s", $0}END{printf "\n"}' temp_merged.align > temp_merged-2.align

# # Remove from the alignment the sequence with too much gaps
# # We set an arbitrary threshold at 1/3 of sequence length
# awk 'BEGIN{limit=0.33}$0~/^>/{header=$0}$0!~/^>/{seq=$0;count=0;seqsize=length($0); while (match($0, /-/)) {count++; $0=substr($0, RSTART+RLENGTH)}; if(count/seqsize < limit){printf "%s\n%s\n", header, seq}}' temp_merged-2.align > temp_merged_cleaned.align

# # Make consensus sequences and
# #  calculate the nucleotde's frequencies at each position
# ./cons.o temp_merged_cleaned.align





# Remember to exclude the 7mer in the subsequent analyses!



# Measure the number of mutations with the consensus
#  sequence as reference



# Make a logo plot
