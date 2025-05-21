# Script to draw a logo plot

This script is designed to produce an image of a logo plot using as input a matrix of positions x nucleotides containing the frequencies for each entry.


## Horizontal spacing problems
Problems i and ii are related in the sense that solving i will probably solve ii. However, solving ii might solve i. 

### (i) Large fonts occupy too much space
Increasing the size of the font means increasing its width, so the visual representation is less convincing because each position in the sequence do not occupy the same horizontal space in the representation...
Trying to get a way to scale down the width of the letter so it is constant for all positions in the sequence. But doing this using the cairo library might be challenging.

### (ii) Calculation of the horizontal offset creates additional space
When calculating the horizontal offset to align letters at the center of a position in the sequence, we create extra space between that position and the previous one. That is because the reference width used to calculate the offset is that of the largest font size, but if all the letters at that position have 1/4 freq then all letters have the same offset and it creates a space between the n and n-1 positions.
A trivial solution would be to use as reference width the largest letter present at that position.

### Solved problem for (i)
The cairo_scale() command was affecting the whole image and so comparmentalising the transformation on the image for each letter solved everything!

### Solved problem for (ii)
The horizontal offset was added using the maximum width of the maximum font size instead of the fixed value for the width: SEQ_POSITION_WIDTH.


## Issue: making the width dependent on the sequence length
It was probably the scaling that caused the problem. I switched the scaling directly using the native cairo command and used the scaling at the scale of the whole image at the end, which is much less errorprone.